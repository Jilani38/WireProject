#!/bin/bash


#Vérification de l'intégralité des paramètres et messages d'erreurs associé

# Fonction d'aide
help() {
    echo "Usage : $0 <fichier CSV> <type station> <type consommateur> [ID centrale]"
    echo
    echo "Description :"
    echo "Ce script analyse un fichier CSV contenant des données de distribution électrique."
    echo
    echo "Paramètres :"
    echo "  <fichier CSV>       Chemin vers le fichier CSV d'entrée (obligatoire)."
    echo "  <type station>      Type de station à traiter : hvb | hva | lv (obligatoire)."
    echo "  <type consommateur> Type de consommateur à traiter : comp | indiv | all (obligatoire)."
    echo "  [ID centrale]       Identifiant de la centrale (optionnel)."
    echo
    echo "Options :"
    echo "  -h                  Affiche cette aide et ignore les autres options."
    exit 0
}


# Vérification si l'option -h est présente dans les arguments
for arg in "$@"; do
    if [[ "$arg" == "-h" ]]; then
        help
    fi
done

# Vérification du nombre minimum d'arguments
if [[ $# -lt 3 ]]; then
    echo "Erreur : Nombre d'arguments insuffisant."
    echo "Utilisez $0 -h pour l'aide."
    exit 1
fi

# Paramètres
CSV_FILE=$1
STATION_TYPE="$2"
CONSUMER_TYPE="$3"
CENTRAL_ID="${4:-}"  # Optionnel : défaut vide si non fourni

# Vérification de l'existence du fichier CSV
if [[ ! -f "$CSV_FILE" ]]; then
    echo "Erreur : Le fichier $CSV_FILE n'existe pas."
    exit 1
fi

#Vérification de lisibilité du fichier CSV
if [[ ! -r "$CSV_FILE" ]]; then
    echo "Erreur : Le fichier $CSV_FILE existe mais n'est pas lisible."
    exit 1
fi

#Vérification du format du fichier CSV
EXPECTED_COLUMNS="Power plant;HV-B Station;HV-A Station;LV Station;Company;Individual;Capacity;Load"

# Lire la première ligne du fichier
HEADER=$(head -n 1 "$CSV_FILE")

# Vérifier si les colonnes correspondent
if [[ "$HEADER" != "$EXPECTED_COLUMNS" ]]; then
    echo "Erreur : Le fichier $CSV_FILE ne respecte pas le format attendu."
    exit 1
fi


# Vérification du type de station
if [[ "$STATION_TYPE" != "hvb" && "$STATION_TYPE" != "hva" && "$STATION_TYPE" != "lv" ]]; then
    echo "Erreur : Type de station invalide ($STATION_TYPE)."
    echo "Valeurs possibles : hvb, hva, lv."
    echo "Utilisez $0 -h pour l'aide."
    exit 1
fi

# Vérification du type de consommateur
if [[ "$CONSUMER_TYPE" != "comp" && "$CONSUMER_TYPE" != "indiv" && "$CONSUMER_TYPE" != "all" ]]; then
    echo "Erreur : Type de consommateur invalide ($CONSUMER_TYPE)."
    echo "Valeurs possibles : comp, indiv, all."
    echo "Utilisez $0 -h pour l'aide."
    exit 1
fi

# Vérification des combinaisons station-consommateur interdites
if [[ "$STATION_TYPE" == "hvb" && ("$CONSUMER_TYPE" == "all" || "$CONSUMER_TYPE" == "indiv") ]]; then
    echo "Erreur : Les options hvb indiv et hvb all sont interdites."
    exit 1
fi

if [[ "$STATION_TYPE" == "hva" && ("$CONSUMER_TYPE" == "all" || "$CONSUMER_TYPE" == "indiv") ]]; then
    echo "Erreur : Les options hva indiv et hva all sont interdites."
    exit 1
fi

# Tous les paramètres sont valides
echo "Tous les paramètres sont valides."
echo "Fichier CSV : $CSV_FILE"
echo "Type de station : $STATION_TYPE"
echo "Type de consommateur : $CONSUMER_TYPE"
if [[ -n "$CENTRAL_ID" ]]; then
    echo "Identifiant de centrale : $CENTRAL_ID"
else
    echo "Aucun identifiant de centrale spécifié."
fi


echo "Vérification et préparation des dossiers requis..."

# Liste des répertoires à vérifier/créer
required_dirs=("tmp" "graphs" "tests")

# Boucle pour gérer chaque répertoire
for dir in "${required_dirs[@]}"; do
    if [[ ! -d "$dir" ]]; then
        echo "Création du dossier : $dir"
        mkdir -p "$dir" || {
            echo "Erreur : Impossible de créer le dossier $dir."
            exit 1
        }
    else
        echo "Dossier $dir déjà existant."
    fi
done

# Vider le dossier tmp si existant
echo "Nettoyage du dossier tmp..."
rm -rf tmp/* || {
    echo "Erreur : Impossible de nettoyer le dossier tmp."
    exit 1
}

echo "Tous les dossiers requis sont prêts."

# Nom de l'exécutable et des fichiers source
EXECUTABLE="codeC/exeC"

# Vérification de l'existence de l'exécutable
if [[ ! -f "$EXECUTABLE" ]]; then
    echo "L'exécutable '$EXECUTABLE' est introuvable. Compilation en cours..."
    cd codeC
    make clean
    if [[ $? -ne 0 ]]; then
        echo "Erreur lors de la compilation."
        exit 1
    fi
    cd ..
    echo "Compilation réussie. Exécutable généré : "$EXECUTABLE""
else
    echo "L'exécutable '$EXECUTABLE' existe déjà."
fi


#Filtre des données et création d'un CSV avec ces dernières dans le dossier tmp

start_time=$(date +%s)

echo "Filtrage des données..."

STATIONS_FILE="tmp/stations_data.csv"
CONSUMERS_FILE="tmp/consumers_data.csv"

awk -F';' -v station="$STATION_TYPE" -v consumer="$CONSUMER_TYPE" -v central_id="$CENTRAL_ID" '
BEGIN {
    # Déterminer les colonnes d intérêt en fonction du type de station
    if (station == "hvb") station_col = 2;
    else if (station == "hva") station_col = 3;
    else if (station == "lv") station_col = 4;

    # Déterminer les colonnes dintérêt en fonction du type de consommateur
    if (consumer == "comp") consumer_col = 5;
    else if (consumer == "indiv") consumer_col = 6;
    else if (consumer == "all") consumer_col = 0; # Tous les consommateurs
}

{
    # Filtrer les lignes de STATIONS (capacités renseignées et pas de consommation)
    if ($station_col != "-" && $7 != "-" && $(station_col + 1) == "-") {
        if (central_id == "" || $1 == central_id) {
            print $station_col ";" $7 > "'"$STATIONS_FILE"'"
        }
    }

    # Filtrer les lignes de CONSOMMATEURS (consommations renseignées)
    if ($station_col != "-") {
        if ($consumer_col != "-" || consumer_col == 0) {
            if ($7 == "-") {
                if (central_id == "" || $1 == central_id) {
                    print $station_col ";" $8 > "'"$CONSUMERS_FILE"'"
                }
            }
        }
    }
}
' "$CSV_FILE"

echo "Données des stations enregistrées dans $STATIONS_FILE"
echo "Données des consommateurs enregistrées dans $CONSUMERS_FILE"

end_time=$(date +%s)
execution_time=$((end_time - start_time))
echo "Le filtrage a pris $execution_time secondes pour s'exécuter."

# Mesurer le temps d'exécution avec la commande `time`
echo "Exécution du programme..."
start_time=$(date +%s)
cd codeC
./exeC $STATION_TYPE $CONSUMER_TYPE
end_time=$(date +%s)

# Calculer le temps écoulé
execution_time=$((end_time - start_time))
echo "Le programme a pris $execution_time secondes pour s'exécuter."

exit 0
