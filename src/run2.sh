#!/bin/bash

ITERACIONES=6

CSV1="tiempos_originalRedes.csv"
CSV2="tiempos_modificadoRedes.csv"

# Cabeceras
echo "Iteracion;degree;closeness;harmonic;ASP;betweenness;percolation;pagerank" > "$CSV1"
echo "Iteracion;degree;closeness;harmonic;ASP;betweenness;percolation;pagerank" > "$CSV2"


./redes "$ITERACIONES" | awk \
-v csv1="$CSV1" \
-v csv2="$CSV2" '

/^V1$/ {
    archivo = csv1
    next
}

/^V2$/ {
    archivo = csv2
    next
}

/^[0-9]+;/ {
    sub(/;$/, "")
    print >> archivo
}

'
