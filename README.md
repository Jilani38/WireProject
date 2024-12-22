 "Usage: ./c-wire.sh [CSV file] [Station type] [Consumer type] [Central ID]"
    
     "Description:"
     "This script analyzes a CSV file containing electrical distribution data. It processes different information depending on the type of station and type of consumer specified, enabling targeted, customized data analysis."
    
     "Parameters:"
     "  <CSV file>         Path to the input CSV file (mandatory)."
     "  <station type>     Type of station to process: hvb | hva | lv (mandatory)."
     "  <consumer type>    Type of consumer to process: comp | indiv | all (mandatory)."
     "  [Central ID]       Identifier of the central (optional)."

     "Options:"
     "  -h                 Shows this help and ignores other options."

"Example of execution : ./c-wire.sh data.csv hva comp 123"

"This script is designed to run under a Unix-like environment (Linux, macOS). 

Please ensure that the input CSV file is correctly formatted and that the parameters are supplied in the specified order."
