# Análisis de Redes mediante Métricas de Centralidad

## Descripción

Este proyecto implementa un conjunto de algoritmos para el análisis de redes mediante métricas de centralidad utilizando un **TDA Grafo basado en listas de adyacencia**.

Se analizan dos conjuntos de datos:

| Dataset | Estructura | Peso |
|---------|------------|------|
| Interacción de proteínas | Grafo no dirigido | 1 |
| Conexiones IP | Digrafo dirigido | Tiempo de conexión |

Además del cálculo de las métricas, el proyecto incorpora experimentos para modificar la estructura de la red y estudiar el efecto sobre las métricas y los tiempos de ejecución.

---

## Contenido

1. Objetivos
2. Estructura del proyecto
3. Datasets
4. Implementación
5. Métricas implementadas
6. Compilación
7. Configuración de experimentos
8. Ejecución
9. Resultados
10. Autores

---

## Estructura del proyecto

```text
Proyecto/
├── DATA/
├── include/
├── ResultadosP/
├── ResultadosPV2/
├── ResultadosR/
├── ResultadosRV2/
├── Resultados_Tiempos_Ejecucion/
└── src/
```

| Carpeta | Descripción |
|----------|-------------|
| `DATA/` | Contiene los datasets utilizados. |
| `include/` | Cabeceras del proyecto y TDA Grafo. |
| `src/` | Programas principales y scripts de ejecución. |
| `ResultadosP/` | Resultados del grafo de proteínas. |
| `ResultadosPV2/` | Resultados del grafo de proteínas modificado. |
| `ResultadosR/` | Resultados del digrafo de redes. |
| `ResultadosRV2/` | Resultados del digrafo modificado. |
| `Resultados_Tiempos_Ejecucion/` | Tiempos promedio de ejecución. |

## DATA

### Dataset de proteínas

Lista de interacciones entre proteínas de levadura. Se modela como un **grafo no dirigido** con peso unitario.

### Dataset de redes

Lista de conexiones entre direcciones IP. Se modela como un **digrafo ponderado**, donde el peso corresponde al tiempo de conexión.

## include

| Archivo | Descripción |
|---------|-------------|
| `GrafoADT.h` | Implementación del TDA Grafo mediante listas de adyacencia. |
| `leerDataSet.h` | Lectura y construcción de grafos desde los datasets. |
| `Metricas.h` | Implementación de las métricas de centralidad. |
| `metricasUtils.h` | Funciones auxiliares para agregar y eliminar aristas, entre otras utilidades. |

### Complejidad del ADT Grafo

| Operación | Complejidad |
|-----------|-------------|
| Agregar vértice | O(1) |
| Agregar arista | O(1) |
| Eliminar arista | O(grado(v)) |
| Eliminar vértice | O(V+E) |
| Obtener vecinos | O(grado(v)) |
| Recorrer el grafo | O(V+E) |

## Métricas implementadas

| Métrica | Descripción |
|----------|-------------|
| Degree Centrality | Importancia según el número de conexiones. |
| Betweenness Centrality | Frecuencia con la que un nodo pertenece a caminos mínimos. |
| Closeness Centrality | Cercanía promedio al resto de la red. |
| Harmonic Centrality | Variante de Closeness para grafos desconectados. |
| PageRank | Importancia iterativa de los nodos. |
| Average Shortest Path | Longitud promedio de los caminos mínimos. |
| Percolation Centrality | Influencia considerando procesos de propagación. |

## src

| Archivo | Descripción |
|---------|-------------|
| `mainPruebaMetricasProteinas.cpp` | Experimentos sobre proteínas. |
| `mainPruebaMetricasRedes.cpp` | Experimentos sobre redes IP. |
| `run.sh` | Automatiza los experimentos de proteínas. |
| `run2.sh` | Automatiza los experimentos de redes. |

## Dependencias

- g++
- Bash

No se requieren bibliotecas externas.

## Compilación

Desde la carpeta `src`:

```bash
g++ mainPruebaMetricasRedes.cpp -o redes
g++ mainPruebaMetricasProteinas.cpp -o proteinas
```

Dar permisos de ejecución:

```bash
chmod +x run.sh
chmod +x run2.sh
```

## Configuración de experimentos

Modificar al inicio de `run.sh` o `run2.sh`:

```bash
ITERACIONES=30
EXPERIMENTO=0
```

| Variable | Descripción |
|----------|-------------|
| `ITERACIONES` | Número de ejecuciones para calcular tiempos promedio. |
| `EXPERIMENTO` | Experimento a realizar. |

### Experimentos

| Valor | Descripción |
|------:|-------------|
| `0` | Eliminación de aristas importantes del grafo. |
| `1` | Incorporación de aristas hacia los nodos más aislados. |

> **Nota:** Modifique estas variables antes de ejecutar los scripts.

## Ejecución

Proteínas:

```bash
./run.sh
```

Redes:

```bash
./run2.sh
```

Los scripts generan automáticamente todos los archivos CSV correspondientes a las métricas y tiempos de ejecución.

## Autores

- Bryan Aguirre Fuentes
- Rodolfo Rubio Reyes
- Javier Castillo Pardo

## Uso académico

Este proyecto fue desarrollado exclusivamente con fines académicos para el estudio de estructuras de datos, teoría de grafos y análisis de redes mediante métricas de centralidad.
