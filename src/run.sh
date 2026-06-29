#!/bin/bash

# Variables de control
ITERACIONES=30
EXPERIMENTO=0 # Cambia este valor a 0 o 1 según el experimento que desees ejecutar

# 1. Definimos la ruta de la carpeta de destino
RUTA_DESTINO="../Resultados_Tiempos_Ejecucion"

# 2. Creamos la carpeta si no existe 
mkdir -p "$RUTA_DESTINO"

# 3. Archivos de salida dinámicos concatenando la ruta y el tipo de experimento
CSV1="${RUTA_DESTINO}/tiempos_originalProteinas_exp${EXPERIMENTO}.csv"
CSV2="${RUTA_DESTINO}/tiempos_modificadoProteinas_exp${EXPERIMENTO}.csv"

# Cabeceras
echo "Iteracion;degree;closeness;harmonic;ASP;betweenness;percolation;pagerank" > "$CSV1"
echo "Iteracion;degree;closeness;harmonic;ASP;betweenness;percolation;pagerank" > "$CSV2"

# Ejecutamos el programa pasando ambos argumentos
./proteinas "$ITERACIONES" "$EXPERIMENTO" | awk \
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
