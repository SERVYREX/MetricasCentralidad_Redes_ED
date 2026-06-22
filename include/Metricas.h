#ifndef METRICAS_H
#define METRICAS_H

#include "GrafoADT.h"
#include <vector>
#include <queue>
#include <map>
#include <limits>

template <typename TipoVertice, typename TipoPeso = int>
class Centralidad {
public:
    // Estructura auxiliar para la cola de prioridad de Dijkstra
    struct ComparadorDijkstra {
        bool operator()(const std::pair<TipoPeso, TipoVertice>& a, const std::pair<TipoPeso, TipoVertice>& b) const {
            return a.first > b.first; 
        }
    };

    // Degree Centrality. Retorna un mapa con la importancia de todos los vértices en función de su grado
    static std::map<TipoVertice, double> centralidadGrado(const Grafo<TipoVertice, TipoPeso>& g) {
        std::map<TipoVertice, double> resultado;
        int V = g.getNumVertices();
        if (V <= 1) return resultado;

        for (const auto& v : g.getVertices()) {
            // Se normaliza el valor (Diviendo por V-1) para así obtener valores entre 0.0 y 1.0
            resultado[v] = static_cast<double>(g.getGrado(v)) / (V - 1);
        }
        return resultado;
    }

    // Algoritmo de Dijkstra para calcular caminos mínimos utilizados en las siguientes métricas
    static std::map<TipoVertice, TipoPeso> dijkstraDistancias(const Grafo<TipoVertice, TipoPeso>& g, const TipoVertice& inicio) {
        std::map<TipoVertice, TipoPeso> distancias;
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
    static std::map<TipoVertice, double> centralidadCercania(const Grafo<TipoVertice, TipoPeso>& g) {
        std::map<TipoVertice, double> closeness;
        auto vertices = g.getVertices();
        int V = vertices.size();
        TipoPeso INFINITO = std::numeric_limits<TipoPeso>::max();

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
                if (distancias[u] == INFINITO) {
                    alcanzableTodo = false; 
                    break;
                }
     
                sumaDistancias += static_cast<double>(distancias[u]);
            }

	    // Verifica que el vértice alcance a todos los demás, si no lo hace, esta métrica asigna un valor 0 de importancia a ese nodo
            if (alcanzableTodo && sumaDistancias > 0) {
	      // Asigna al vértice su valor de importancia, utilizando la fórmula normalizada
                closeness[v] = static_cast<double>(V - 1) / sumaDistancias;
            } else {
                closeness[v] = 0.0;
            }
        }
        return closeness;
    }

    // Harmonic Centrality. Funciona de manera similar a la métrica por cercanía, sin embargo, permite analizar redes disconexas
    static std::map<TipoVertice, double> centralidadArmónica(const Grafo<TipoVertice, TipoPeso>& g) {
        std::map<TipoVertice, double> harmonic;
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
                if (distancias[u] != INFINITO) {
		  sumaInversa += 1.0 / static_cast<double>(distancias[u]);
		}
            }

	    // Registra el valor aplicando la fórmula normalizada
	    harmonic[v] = sumaInversa / static_cast<double>(V-1);
        }
        return harmonic;
    }

    // Average Shortest Path. Retorna el valor promedio de los caminos más cortos del grafo
    static double caminoPromedio (const Grafo<TipoVertice, TipoPeso>& g){
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
	  
	  // Si un nodo no fue alcanzado, la métrica arroja un valor inválido
	  if (distancias[u] == INFINITO) {
	    return -1.0; 
            }
	  
	  sumaDistancias += static_cast<double>(distancias[u]);
        }
      }
      
      // Aplica la fórmula de la métrica
      return sumaDistancias / static_cast<double>(V * (V - 1));
    }

    
};

#endif
