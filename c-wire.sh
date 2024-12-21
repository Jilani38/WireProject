#!/bin/bash

# Function to display help message
help() {
    echo "Usage: $0 <CSV file> <station type> <consumer type> [Central ID]"
    echo
    echo "Description:"
    echo "This script analyzes a CSV file containing electrical distribution data."
    echo
    echo "Parameters:"
    echo "  <CSV file>         Path to the input CSV file (mandatory)."
    echo "  <station type>     Type of station to process: hvb | hva | lv (mandatory)."
    echo "  <consumer type>    Type of consumer to process: comp | indiv | all (mandatory)."
    echo "  [Central ID]       Identifier of the central (optional)."
    echo
    echo "Options:"
    echo "  -h                 Shows this help and ignores other options."
    exit 0
}


# Check if help option is present
for arg in "$@"; do
    if [[ "$arg" == "-h" ]]; then
        help
    fi
done

# Check for the minimum number of arguments
if [[ $# -lt 3 ]]; then
    echo "Error: Insufficient number of arguments."
    echo "Use $0 -h for help."
    exit 1
fi

# Assign parameters
CSV_FILE="$1"
STATION_TYPE="$2"
CONSUMER_TYPE="$3"
CENTRAL_ID="${4:-}"  # Optional: default empty if not provided

# Check if CSV file exists
if [[ ! -f "$CSV_FILE" ]]; then
    echo "Error: The file $CSV_FILE does not exist."
    exit 1
fi

# Check if CSV file is readable
if [[ ! -r "$CSV_FILE" ]]; then
    echo "Error: The file $CSV_FILE exists but is not readable."
    exit 1
fi

# Expected CSV header
EXPECTED_COLUMNS="Power plant;HV-B Station;HV-A Station;LV Station;Company;Individual;Capacity;Load"

# Read the first line of the CSV file
HEADER=$(head -n 1 "$CSV_FILE")

# Verify if columns match
if [[ "$HEADER" != "$EXPECTED_COLUMNS" ]]; then
    echo "Error: The file $CSV_FILE does not match the expected format."
    exit 1
fi

# Check station type validity
if [[ ! "$STATION_TYPE" =~ ^(hvb|hva|lv)$ ]]; then
    echo "Error: Invalid station type ($STATION_TYPE)."
    echo "Possible values: hvb, hva, lv."
    echo "Use $0 -h for help."
    exit 1
fi

# Check consumer type validity
if [[ ! "$CONSUMER_TYPE" =~ ^(comp|indiv|all)$ ]]; then
    echo "Error: Invalid consumer type ($CONSUMER_TYPE)."
    echo "Possible values: comp, indiv, all."
    echo "Use $0 -h for help."
    exit 1
fi

# Check for forbidden combinations
if [[ "$STATION_TYPE" == "hvb" || "$STATION_TYPE" == "hva" ]]; then
    if [[ "$CONSUMER_TYPE" =~ ^(all|indiv)$ ]]; then
        echo "Error: The options $STATION_TYPE $CONSUMER_TYPE are forbidden."
        exit 1
    fi
fi

# All parameters are valid


echo "CSV File: $CSV_FILE"
echo "Station Type: $STATION_TYPE"
echo "Consumer Type: $CONSUMER_TYPE"




if [[ -n "$CENTRAL_ID" ]]; then
    echo "Central ID: $CENTRAL_ID"
fi

# Prepare required directories
echo "Verifying and preparing required directories..."

required_dirs=("tmp" "graphs" "tests")

for dir in "${required_dirs[@]}"; do
    if [[ ! -d "$dir" ]]; then
        echo "Creating directory: $dir"
        mkdir -p "$dir" || {
            echo "Error: Could not create directory $dir."
            exit 1
        }
    fi
done

# Clean tmp directory
rm -rf tmp/* || {
    echo "Error: Could not clean tmp directory."
    exit 1
}


# Executable and source file names
EXECUTABLE="codeC/exeC"

# Check if executable exists, compile if not
if [[ ! -f "$EXECUTABLE" ]]; then
    echo "Executable '$EXECUTABLE' not found. Compiling..."
    (cd codeC && make > /dev/null && make clean > /dev/null) || {
        echo "Error during compilation."
        exit 1
    }
    echo "Compilation successful. Executable generated: '$EXECUTABLE'"
fi

# Filter data and create CSV files in tmp folder
start_time=$(date +%s)

echo "Filtering data..."

STATIONS_FILE="tmp/stations_data.csv"
CONSUMERS_FILE="tmp/consumers_data.csv"

#Variables use to display progress 
total_lines=$(wc -l < "$CSV_FILE")
processed_lines=0

awk -F';' -v station="$STATION_TYPE" -v consumer="$CONSUMER_TYPE" -v central_id="$CENTRAL_ID" '
BEGIN {
    # Determine the column of interest based on station type
    if (station == "hvb") station_col = 2;
    else if (station == "hva") station_col = 3;
    else if (station == "lv") station_col = 4;

    # Determine the column of interest based on consumer type
    if (consumer == "comp") consumer_col = 5;
    else if (consumer == "indiv") consumer_col = 6;
    else if (consumer == "all") consumer_col = 0;  # All consumers
}

{
    # Filter station data (capacities provided and no consumption)
    if ($station_col != "-" && $7 != "-" && $(station_col + 1) == "-") {
        if (central_id == "" || $1 == central_id) {
            print $station_col ";" $7 > "'"$STATIONS_FILE"'"
        }
    }

    # Filter consumer data (consumptions provided)
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


# Calculate filtering time
end_time=$(date +%s)
execution_time=$(($end_time - $start_time))
echo "Filtering took $execution_time seconds to execute."

# Execute C program
echo "Running program..."
start_time=$(date +%s)
(cd codeC && ./exeC "$STATION_TYPE" "$CONSUMER_TYPE")
end_time=$(date +%s)

# Calculate execution time
execution_time=$(($end_time - $start_time))
echo "The program took $execution_time seconds to execute."

#Generate plot if lv all case
if [[ "$CONSUMER_TYPE" == "all" ]]; then
    gnuplot plot-script.gp
    echo "Chart of filtered results have been genereted to /graphs/station_capacity_load_chart.png"
fi

exit 0