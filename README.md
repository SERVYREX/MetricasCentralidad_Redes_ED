# MetricasCentralidad_Redes_ED

## Descripción

Este proyecto implementa un conjunto de algoritmos para el análisis de redes utilizando distintas **métricas de centralidad** sobre grafos y digrafos representados mediante **listas de adyacencia**.

Se trabaja sobre dos conjuntos de datos distintos:

* Una red de interacción entre proteínas de levadura, modelada como un **grafo no dirigido**.
* Una red de conexiones entre direcciones IP, modelada como un **digrafo ponderado**.

Además del cálculo de las métricas, el proyecto incorpora experimentos que permiten modificar la estructura de los grafos mediante la eliminación o incorporación de aristas, evaluando posteriormente el impacto de dichas modificaciones tanto en los valores de las métricas como en sus tiempos de ejecución.

---

# Objetivos

* Implementar un TDA Grafo utilizando listas de adyacencia.
* Implementar distintas métricas de análisis de redes.
* Analizar el comportamiento de dichas métricas sobre redes reales.
* Evaluar el efecto de modificar la estructura del grafo mediante distintos experimentos.
* Medir y comparar los tiempos de ejecución de cada algoritmo.

---

# Métricas implementadas

El proyecto implementa las siguientes métricas:

* Degree Centrality
* Betweenness Centrality
* Closeness Centrality
* Harmonic Centrality
* PageRank
* Average Shortest Path
* Percolation Centrality

---

# Estructura del proyecto

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

---

## DATA

Contiene los conjuntos de datos utilizados durante los experimentos.

### Dataset de interacción de proteínas

Corresponde a una lista de interacciones entre proteínas de levadura.

Cada línea representa una interacción entre dos proteínas, por lo que el conjunto de datos se modela como un:

* Grafo no dirigido.
* Peso unitario para todas las aristas (peso = 1).

---

### Dataset de conexiones IP

Corresponde a una lista de conexiones entre direcciones IP.

Cada línea representa una conexión desde una IP origen hacia una IP destino junto con el tiempo asociado a dicha conexión.

Este conjunto de datos se modela como un:

* Digrafo dirigido.
* Peso igual al tiempo de conexión registrado.

---

## include

Contiene los archivos de cabecera del proyecto.

### GrafoADT.h

Implementa el TDA Grafo mediante listas de adyacencia.

Las operaciones principales presentan las siguientes complejidades temporales:

| Operación         | Complejidad   |
| ----------------- | ------------- |
| Agregar vértice   | O(1) promedio |
| Agregar arista    | O(1) promedio |
| Eliminar arista   | O(grado(v))   |
| Eliminar vértice  | O(V + E)      |
| Obtener vecinos   | O(grado(v))   |
| Recorrer el grafo | O(V + E)      |

---

### leerDataSet.h

Contiene los métodos encargados de leer ambos conjuntos de datos y construir los grafos correspondientes.

---

### Metricas.h

Implementa los algoritmos correspondientes a las siguientes métricas:

* Degree Centrality
* Betweenness Centrality
* Closeness Centrality
* Harmonic Centrality
* PageRank
* Average Shortest Path
* Percolation Centrality

---

### metricasUtils.h

Contiene funciones auxiliares utilizadas durante los experimentos, incluyendo operaciones para agregar o eliminar aristas y otras funciones de apoyo necesarias para modificar la estructura de los grafos.

---

## src

Contiene los programas principales del proyecto.

### mainPruebaMetricasProteinas.cpp

Ejecuta el cálculo de todas las métricas sobre el dataset de interacción de proteínas.

---

### mainPruebaMetricasRedes.cpp

Ejecuta el cálculo de todas las métricas sobre el dataset de conexiones IP.

---

### run.sh

Automatiza la ejecución de los experimentos sobre el grafo de proteínas.

---

### run2.sh

Automatiza la ejecución de los experimentos sobre el digrafo de conexiones IP.

---

## Carpetas de resultados

### ResultadosP

Contiene los valores de las métricas calculadas sobre el grafo original de proteínas.

Los resultados son exportados en formato CSV.

---

### ResultadosPV2

Contiene los valores obtenidos después de aplicar los experimentos sobre el grafo de proteínas.

Los resultados también se almacenan en formato CSV.

---

### ResultadosR

Contiene los valores de las métricas calculadas sobre el digrafo original de conexiones IP.

Los resultados se almacenan en formato CSV.

---

### ResultadosRV2

Contiene los resultados del digrafo una vez aplicadas las modificaciones correspondientes a los experimentos.

Todos los resultados se exportan en formato CSV.

---

### Resultados_Tiempos_Ejecucion

Almacena los tiempos promedio de ejecución de cada una de las métricas implementadas.

Cada experimento genera archivos CSV independientes para facilitar su posterior análisis.

---

# Dependencias

Para compilar y ejecutar el proyecto únicamente es necesario disponer de:

* Compilador **g++**.
* Un entorno compatible con **Bash** (Linux o Windows mediante WSL).

No se requieren bibliotecas externas adicionales.

---

# Compilación

Desde la carpeta `src`, compile ambos programas mediante:

```bash
g++ mainPruebaMetricasRedes.cpp -o redes
g++ mainPruebaMetricasProteinas.cpp -o proteinas
```

Esto generará los ejecutables:

* `redes`
* `proteinas`

---

# Permisos de ejecución

Antes de ejecutar los scripts, otorgue permisos de ejecución:

```bash
chmod +x run.sh
chmod +x run2.sh
```

Este paso únicamente es necesario la primera vez.

---

# Configuración de los experimentos

Los scripts `run.sh` y `run2.sh` permiten configurar el número de iteraciones y el experimento que se desea ejecutar modificando las siguientes variables:

```bash
ITERACIONES=30
EXPERIMENTO=0
```

## Número de iteraciones

La variable `ITERACIONES` indica cuántas veces se ejecutará el programa para calcular el tiempo promedio de ejecución.

Por ejemplo:

```bash
ITERACIONES=50
```

ejecutará cada experimento 50 veces antes de calcular los promedios.

---

## Tipo de experimento

La variable `EXPERIMENTO` controla la modificación aplicada al grafo.

| Valor | Experimento                                                       |
| :---: | ----------------------------------------------------------------- |
| **0** | Eliminación de aristas consideradas importantes dentro del grafo. |
| **1** | Incorporación de aristas hacia los nodos más aislados de la red.  |

Por ejemplo:

```bash
EXPERIMENTO=1
```

ejecutará el experimento de incorporación de nuevas aristas.

---

# Ejecución

Una vez compilado el proyecto y configurado el experimento, ejecute:

Para el conjunto de proteínas:

```bash
./run.sh
```

Para el conjunto de redes:

```bash
./run2.sh
```

---

# Resultados generados

Al finalizar la ejecución se generarán automáticamente todos los archivos CSV correspondientes a:

* Valores de las métricas sobre los grafos originales.
* Valores de las métricas después de aplicar el experimento seleccionado.
* Tiempos promedio de ejecución de cada algoritmo.

Los archivos serán almacenados en las carpetas:

```text
ResultadosP/
ResultadosPV2/
ResultadosR/
ResultadosRV2/
Resultados_Tiempos_Ejecucion/
```

---

# Autores

Este proyecto fue desarrollado por:

* **Bryan Aguirre Fuentes**
* **Rodolfo Rubio Reyes**
* **Javier Castillo Pardo**

---

# Uso académico

Este proyecto fue desarrollado **exclusivamente con fines académicos**, como parte del estudio e implementación de algoritmos de análisis de redes mediante métricas de centralidad.

Su propósito es apoyar actividades de aprendizaje, experimentación e investigación relacionadas con teoría de grafos y análisis de redes complejas.
