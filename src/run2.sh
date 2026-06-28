#!/bin/bash

# Variables de control
ITERACIONES=30
EXPERIMENTO=0

# 1. Definimos la ruta de la carpeta de destino
# Puedes usar una ruta relativa (ej. "./mis_resultados") o absoluta (ej. "/home/usuario/proyecto/resultados")
RUTA_DESTINO="../Resultados_Tiempos_Ejecucion"

# 2. Creamos la carpeta si no existe (-p evita errores si la carpeta ya está creada)
mkdir -p "$RUTA_DESTINO"

# 3. Concatenamos la ruta con el nombre de los archivos
CSV1="${RUTA_DESTINO}/tiempos_originalRedes_exp${EXPERIMENTO}.csv"
CSV2="${RUTA_DESTINO}/tiempos_modificadoRedes_exp${EXPERIMENTO}.csv"

# Cabeceras
echo "Iteracion;degree;closeness;harmonic;ASP;betweenness;percolation;pagerank" > "$CSV1"
echo "Iteracion;degree;closeness;harmonic;ASP;betweenness;percolation;pagerank" > "$CSV2"

# Ejecutamos el programa pasando ambos argumentos
./redes "$ITERACIONES" "$EXPERIMENTO" | awk \
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
