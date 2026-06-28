#ifndef METRICAS_H
#define METRICAS_H

#include "GrafoADT.h"
#include <vector>
#include <queue>
#include <map>
#include <unordered_map>
#include <limits>

template <typename TipoVertice, typename TipoPeso = int>
class Centralidad {
private:
    // Función auxiliar común para el algoritmo de Brandes usado en intermediación y percolación
    static void calcularDependenciasBrandes(
        const Grafo<TipoVertice, TipoPeso>& g,
        const TipoVertice& s,
        std::vector<TipoVertice>& S,
        std::unordered_map<TipoVertice, std::vector<TipoVertice>>& predecesores,
        std::unordered_map<TipoVertice, int>& sigma,
        std::unordered_map<TipoVertice, TipoPeso>& distancias
    ) {
        auto vertices = g.getVertices();
        TipoPeso INFINITO = std::numeric_limits<TipoPeso>::max();

        // Se asigna distancia infinita y 0 caminos a todos los nodos
        for (const auto& v : vertices) {
            sigma[v] = 0;
            distancias[v] = INFINITO;
            predecesores[v].clear();
        }

        // Inicializa el nodo de partida
        distancias[s] = 0;
        sigma[s] = 1;

        // Se crea la cola de prioridad, utilizando la estructura auxiliar como comparador
        std::priority_queue<
            std::pair<TipoPeso, TipoVertice>, 
            std::vector<std::pair<TipoPeso, TipoVertice>>, 
            ComparadorDijkstra
        > pq;
        pq.push({0, s});

        // Bucle principal para el cálculo de caminos mínimos
        while (!pq.empty()) {
            // Selecciona el nodo con la menor distancia dentro de la cola
            TipoPeso distActual = pq.top().first;
            TipoVertice u = pq.top().second;
            pq.pop();

            if (distActual > distancias[u]) continue;

            // Guarda el vértice en la lista para el proceso de acumulación posterior
            S.push_back(u);

            // Actualiza las distancias de los vecinos del nodo, comprobando que sea menor a la ya tiene registrada
            for (const auto& arista : g.aristaIncidentes(u)) {
                TipoVertice v = arista.destino;
                TipoPeso pesoArista = arista.peso;

                // Encuentra una ruta más corta hacia el destino 'v'
                if (distancias[u] + pesoArista < distancias[v]) {
                    distancias[v] = distancias[u] + pesoArista;
                    pq.push({distancias[v], v});

                    // Reinicia los predecesores conocidos y hereda la cantidad de caminos de 'u'
                    predecesores[v].clear();
                    predecesores[v].push_back(u);
                    sigma[v] = sigma[u];
                }
                // Encuentra un camino alternativo que empata en costo con la distancia registrada
                else if (distancias[u] + pesoArista == distancias[v]) {
                    predecesores[v].push_back(u);
                    sigma[v] += sigma[u]; 
                }
            }
        }
    }

public:
    // Estructura auxiliar para la cola de prioridad de Dijkstra
    struct ComparadorDijkstra {
        bool operator()(const std::pair<TipoPeso, TipoVertice>& a, const std::pair<TipoPeso, TipoVertice>& b) const {
            return a.first > b.first; 
        }
    };

    // Degree Centrality. Retorna un mapa con la importancia de todos los vértices en función de su grado
    static std::map<TipoVertice, float> centralidadGrado(const Grafo<TipoVertice, TipoPeso>& g) {
        std::map<TipoVertice, float> resultado;
        int V = g.getNumVertices();
        if (V <= 1) return resultado;

        if(g.esDirigido()){
            for (const auto& v : g.getVertices()) {
                // Se normaliza el valor (Diviendo por V-1) para así obtener valores entre 0.0 y 1.0
                resultado[v] = static_cast<float>(g.getInGrado(v)) / (V - 1);
            }
            return resultado;
        }
        
        for (const auto& v : g.getVertices()) {
            // Se normaliza el valor (Diviendo por V-1) para así obtener valores entre 0.0 y 1.0
            resultado[v] = static_cast<float>(g.getGrado(v)) / (V - 1);
        }
        return resultado;
    }

    // Algoritmo de Dijkstra para calcular caminos mínimos utilizados en las siguientes métricas
    static std::unordered_map<TipoVertice, TipoPeso> dijkstraDistancias(const Grafo<TipoVertice, TipoPeso>& g, const TipoVertice& inicio) {
        std::unordered_map<TipoVertice, TipoPeso> distancias;
        TipoPeso INFINITO = std::numeric_limits<TipoPeso>::max();

        // Se asigna distancia infinita a todos los nodos
        for (const auto& v : g.getVertices()) {
            distancias[v] = INFINITO;
        }

        // Se crea la cola de prioridad, utilizando la estructura auxiliar como comparador
        std::priority_queue<
            std::pair<TipoPeso, TipoVertice>, 
            std::vector<std::pair<TipoPeso, TipoVertice>>, 
            ComparadorDijkstra
        > pq;

        distancias[inicio] = 0;
        pq.push({0, inicio});

        // Bucle principal del algoritmo 
        while (!pq.empty()) {
            // Selecciona el nodo con la menor distancia dentro de la cola
            TipoPeso distActual = pq.top().first;
            TipoVertice u = pq.top().second;
            pq.pop();
            
            if (distActual > distancias[u]) continue;
            // Actualiza las distancias de los veciones del nodo, comprobando que sea menor a la ya tiene registrada
            for (const auto& arista : g.aristaIncidentes(u)) {
                TipoVertice v = arista.destino;
                TipoPeso pesoArista = arista.peso;
                if (distancias[u] + pesoArista < distancias[v]) {
                    distancias[v] = distancias[u] + pesoArista;
                    pq.push({distancias[v], v});
                }
            }
        }
        return distancias;
    }
	
    // Closeness Centrality. Retorna un mapa con la importancia de cada nodo en función de su cercanía con los demás nodos del mapa
    static std::map<TipoVertice, float> centralidadCercania(const Grafo<TipoVertice, TipoPeso>& g) {
        std::map<TipoVertice, float> closeness;
        auto vertices = g.getVertices();
        int V = vertices.size();
        TipoPeso INFINITO = std::numeric_limits<TipoPeso>::max();
        int cont = 0;
        
        // Ciclo para iterar sobre todos los vértices
        for (const auto& v : vertices) {
            // Aplica el algoritmo de Dijkstra sobre el nodo para acceder a su distancia hacia todos los demás
            auto distancias = dijkstraDistancias(g, v);
            double sumaDistancias = 0;
            bool alcanzableTodo = true;
            
            // Itera para sumar las distancias obtenidas
            for (const auto& u : vertices) {
                if (u == v) continue;
                // Si un nodo no fue alcanzado (Distancia infinita) rompe el ciclo
                // Búsqueda en O(1)
                auto it = distancias.find(u);
                if (it == distancias.end() || it->second == INFINITO) {
                    alcanzableTodo = false; 
                    break;
                }
         
                sumaDistancias += static_cast<float>(distancias[u]);
            }

            // Verifica que el vértice alcance a todos los demás, si no lo hace, esta métrica asigna un valor 0 de importancia a ese nodo
            if (alcanzableTodo && sumaDistancias > 0) {
                // Asigna al vértice su valor de importancia, utilizando la fórmula normalizada
                closeness[v] = static_cast<float>(V - 1) / sumaDistancias;
            } else {
                closeness[v] = -1.0;
            }
            cont++;
        }
        return closeness;
    }

    // Harmonic Centrality. Funciona de manera similar a la métrica por cercanía, sin embargo, permite analizar redes disconexas
    static std::map<TipoVertice, float> centralidadArmonica(const Grafo<TipoVertice, TipoPeso>& g) {
        std::map<TipoVertice, float> harmonic;
        auto vertices = g.getVertices();
        int V = vertices.size();
        TipoPeso INFINITO = std::numeric_limits<TipoPeso>::max();

        // Ciclo para iterar sobre todos los vértices
        for (const auto& v : vertices) {
            // Aplica el algoritmo de Dijkstra sobre el nodo para acceder a su distancia hacia todos los demás 
            auto distancias = dijkstraDistancias(g, v);
            double sumaInversa = 0.0;

            // Itera para sumar las distancias obtenidas
            for (const auto& u : vertices) {
                if (u == v) continue;
                // Si un nodo no fue alcanzado, es ignorado (suma 0), a diferencia de la centralidad por cercanía
                auto it = distancias.find(u);
                if (it != distancias.end() && it->second != INFINITO) {
                    sumaInversa += 1.0 / static_cast<float>(it->second);
                }
            }

            // Registra el valor aplicando la fórmula normalizada
            harmonic[v] = sumaInversa / static_cast<float>(V - 1);
        }
        return harmonic;
    }

    // Average Shortest Path. Retorna el valor promedio de los caminos más cortos del grafo
    static float caminoPromedio(const Grafo<TipoVertice, TipoPeso>& g){
        auto vertices = g.getVertices();
        int V = vertices.size();

        if (V <= 1) return 0.0; 
        
        double sumaDistancias = 0.0;
        TipoPeso INFINITO = std::numeric_limits<TipoPeso>::max();
        
        // Recorremos cada vértice como origen
        for (const auto& v : vertices) {
            auto distancias = dijkstraDistancias(g, v);
            
            // Recorremos cada vértice como destino
            for (const auto& u : vertices) {
                if (u == v) continue;
                
                auto it = distancias.find(u);
                if (it == distancias.end() || it->second == INFINITO) {
                    return -1;
                }
                
                sumaDistancias += static_cast<float>(distancias[u]);
            }
        }
        
        // Aplica la fórmula de la métrica
        return sumaDistancias / static_cast<double>(V * (V - 1));
    }

    // Betweenness Centrality. Retorna un mapa con la importancia de cada nodo en función de su rol como puente en los caminos más cortos del grafo
    static std::map<TipoVertice, float> centralidadIntermediacion(const Grafo<TipoVertice, TipoPeso>& g) {
        std::map<TipoVertice, float> betweenness;
        auto vertices = g.getVertices();
        int V = vertices.size();

        // Inicializa el valor de intermediación de todos los vértices en 0.0
        for (const auto& v : vertices) {
            betweenness[v] = 0.0;
        }
        
        // Ciclo para iterar sobre todos los vértices 
        for (const auto& s : vertices) {
            
            // Estructuras locales para almacenar los datos del camino desde el origen 's'
            std::vector<TipoVertice> S; 
            std::unordered_map<TipoVertice, std::vector<TipoVertice>> predecesores; 
            std::unordered_map<TipoVertice, int> sigma; // Almacena la cantidad de caminos más cortos que pasan por s
            std::unordered_map<TipoVertice, TipoPeso> distancias; 
            
            // Llamada a la función auxiliar común basada en el algoritmo de Brandes
            calcularDependenciasBrandes(g, s, S, predecesores, sigma, distancias);
          
            // Estructura local para acumular la dependencia de los caminos en reversa
            std::unordered_map<TipoVertice, float> delta;
            for (const auto& v : vertices) delta[v] = 0.0;
          
            // Recorremos la lista S de atrás hacia adelante (Desde los más lejanos a los más cercanos)
            while (!S.empty()) {
                TipoVertice w = S.back();
                S.pop_back();
                
                // Aplica la fórmula acumulativa sobre los predecesores del nodo
                for (const auto& v : predecesores[w]) {
                    delta[v] += (static_cast<float>(sigma[v]) / sigma[w]) * (1.0 + delta[w]);
                }
                
                // Si el nodo actual no es el origen, acumula su valor al score global de intermediación
                if (w != s) {
                    betweenness[w] += delta[w];
                }
            }
        }
        
	// Aplica la fórmula de normalización estándar dividiendo entre los pares totales posibles del grafo, ya sea para digrafo o no dirigido
        if (g.esDirigido() && V > 2) {
            double factorNormalizacion = (static_cast<float>(V - 1) * (V - 2));
            for (const auto& v : vertices) {
                betweenness[v] = (betweenness[v] / factorNormalizacion);
            }
        }
	else if (V > 2){
	  double factorNormalizacion = (static_cast<float>(V - 1) * (V - 2));
            for (const auto& v : vertices) {
                betweenness[v] = 2 * (betweenness[v] / factorNormalizacion);
            }
	}
        
        return betweenness;
    }

    // Percolation Centrality. Determina la importancia de cada nodo en función de la propagación de un estado o infección a través de los caminos más cortos
    static std::map<TipoVertice, float> centralidadPercolacion(const Grafo<TipoVertice, TipoPeso>& g, const std::unordered_map<TipoVertice, double>& estados) {
        std::map<TipoVertice, float> percolation;
        auto vertices = g.getVertices();
        int V = vertices.size();
        
        // Inicializa el valor de percolación de todos los vértices en 0.0
        for (const auto& v : vertices) {
            percolation[v] = 0.0;
        }
        
        // Ciclo para evaluar la sumatoria total del denominador de la fórmula
        float sumaTotalEstados = 0.0;
        for (const auto& v : vertices) {
            // Buscamos el estado del vértice, si no existe asumimos 0.0
            auto itEstado = estados.find(v);
            float estadoV = (itEstado != estados.end()) ? itEstado->second : 0.0;
            sumaTotalEstados += estadoV;
        }
        
        // Ciclo para iterar sobre todos los vértices como origen (s)
        for (const auto& s : vertices) {
          
            // Obtenemos el estado de percolación del nodo origen actual
            auto itEstadoS = estados.find(s);
            float estadoS = (itEstadoS != estados.end()) ? itEstadoS->second : 0.0;
          
            // Estructuras locales para almacenar los datos del camino desde el origen 's'
            std::vector<TipoVertice> S; 
            std::unordered_map<TipoVertice, std::vector<TipoVertice>> predecesores; 
            std::unordered_map<TipoVertice, int> sigma; 
            std::unordered_map<TipoVertice, TipoPeso> distancias; 
          
            // Llamada a la función auxiliar común basada en el algoritmo de Brandes
            calcularDependenciasBrandes(g, s, S, predecesores, sigma, distancias);
          
            // Estructura local para acumular la dependencia de los caminos en reversa (Algoritmo de Brandes modificado para percolación)
            std::unordered_map<TipoVertice, float> delta;
            for (const auto& v : vertices) delta[v] = 0.0;
          
            // Recorremos la lista S de atrás hacia adelante (Desde los más lejanos a los más cercanos)
            while (!S.empty()) {
                TipoVertice w = S.back();
                S.pop_back();
                
                // Aplica la fórmula acumulativa incorporando el estado del origen ponderado por el estado de la red
                for (const auto& v : predecesores[w]) {
                    float factorPercolacion = 0.0;
                    
                    // Buscamos de forma segura en el mapa de estados para evitar excepciones .at()
                    auto itEstadoW = estados.find(w);
                    float estadoW = (itEstadoW != estados.end()) ? itEstadoW->second : 0.0;
                    float denominador = sumaTotalEstados - estadoW;
                  
                    if (denominador > 0.0) {
                        factorPercolacion = estadoS / denominador;
                    }
                  
                    delta[v] += (static_cast<float>(sigma[v]) / sigma[w]) * (1.0 + delta[w]) * factorPercolacion;
                }
                
                // Si el nodo actual no es el origen, acumula su valor al score global de percolación
                if (w != s) {
                    percolation[w] += delta[w];
                }
            }
        }
        
        // Aplica la fórmula de normalización estándar dividiendo entre los pares totales posibles del grafo, ya sea para digrafo o no dirigido
        if (g.esDirigido() && V > 2) {
            float factorNormalizacion = static_cast<float>((V - 1) * (V - 2));
            for (const auto& v : vertices) {
                percolation[v] = percolation[v] / factorNormalizacion;
            }
        }
	else if (V > 2){
	  float factorNormalizacion = static_cast<float>((V - 1) * (V - 2));
            for (const auto& v : vertices) {
                percolation[v] = 2 * percolation[v] / factorNormalizacion;
            }
	  
	}
        
        return percolation;
    }
  
    // PageRank. Determina la importancia de los vértices por la relevancia de sus enlaces entrantes.
    // Se ejecuta de forma iterativa por un número fijo de pasos 
    static std::map<TipoVertice, float> pageRank(const Grafo<TipoVertice, TipoPeso>& g, int iteraciones = 30) {
        std::map<TipoVertice, float> pr;
        auto vertices = g.getVertices();
        int N = vertices.size();
        
        if (N == 0) return pr;
        
        // Todos los nodos empiezan con el mismo valor (1 / N)
        float valorInicial = 1.0 / static_cast<float>(N);
        for (const auto& v : vertices) {
            pr[v] = valorInicial;
        }
        
        float factorD = 0.85;
        
        // Bucle de iteraciones
        for (int i = 0; i < iteraciones; ++i) {
            std::map<TipoVertice, float> nuevoPr;
          
            // Inicialización de factor de atenuación para cada nodo
            for (const auto& v : vertices) {
                nuevoPr[v] = (1.0 - factorD) / static_cast<float>(N);
            }
          
            // Se reparte el PageRank actual de cada nodo hacia sus vecinos
            for (const auto& u : vertices) {
                auto vecinos = g.getVecinos(u);
                int outGrado = vecinos.size();
                
                if (outGrado > 0) {
                    // El nodo 'u' distribuye equitativamente su PageRank entre sus vecinos salientes
                    float prPorVecino = (factorD * pr[u]) / static_cast<float>(outGrado);
                    for (const auto& v : vecinos) {
                        nuevoPr[v] += prPorVecino;
                    }
                } else {
                    // Si 'u' no tiene salidas, su PageRank se reparte a TODOS los nodos por igual
                    float prPorTodos = (factorD * pr[u]) / static_cast<float>(N);
                    for (const auto& v : vertices) {
                        nuevoPr[v] += prPorTodos;
                    }
                }
            }
          
            // Se actualiza el mapa principal con los resultados de cada iteración
            pr = nuevoPr;
        }
        
        return pr;
    }
};

#endif
