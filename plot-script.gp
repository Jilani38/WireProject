# Output configuration
set terminal pngcairo size 1500,800 enhanced font "Arial,12"
set output "graphs/station_capacity_load_chart.png"

# Title and labels
set title "{/Arial,Bold Comparaison Capacité vs Charge par Station}"
set xlabel "Station ID"
set ylabel "Capacity/Load (in kWh)"

# Bar style
set style data histograms
set style histogram clustered gap 1
set style fill solid border -1
set boxwidth 0.8

# Bar colors
set style line 1 lc rgb "blue" lw 2       # Capacity (blue)
set style line 2 lc rgb "green" lw 2      # Load below capacity (green)
set style line 3 lc rgb "red" lw 2        # Load above capacity (red)

# X-axis rotation for readability
set xtics rotate by -45 font ",10"

# Define separator
set datafile separator ":"

# Legend
set key outside top center autotitle columnhead

# Plot data
plot "tests/lv_all_minmax.csv" using 2:xtic(1) with histograms ls 1 title "Capacity", \
     "" using ($3<$2?$3:0):xtic(1) with histograms ls 2 title "Load (below capacity)", \
     "" using ($3>$2?$3:0):xtic(1) with histograms ls 3 title "Load (above capacity)"