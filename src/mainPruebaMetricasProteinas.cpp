#include <iostream>
#include <fstream>
#include <chrono>
#include "../include/leerDataSet.h"
#include "../include/Metricas.h"
#include "../include/metricasUtils.h"
using namespace std;

template <typename nodo, typename valor>
bool exportarCSV(const std::map<nodo, valor>& mapa, 
                     const std::string& nombreArchivo, 
                     const std::string& cabeceraClave = "Nodo", 
                     const std::string& cabeceraValor = "Valor") {
    
    std::ofstream archivo(nombreArchivo);
    
    // Verificar si el archivo se abrió correctamente
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo crear o abrir el archivo " << nombreArchivo << std::endl;
        return false;
    }

    // Escribir la cabecera del CSV
    archivo << cabeceraClave << ";" << cabeceraValor << "\n";

    // Recorrer el mapa y escribir cada par clave-valor
    for (const auto& par : mapa) {
        // Nota: Si tus cadenas contienen comas internas, se deberían envolver en comillas ""
        archivo << par.first << ";" << par.second << "\n";
    }

    archivo.close();
    return true;
}


void eliminarAristas(const std::map<string, float>& mapa, grafoProteinas& g) {
    
    //Copiamos los pares del mapa a un vector
    vector<pair<string, float>> nodosOrdenados(mapa.begin(), mapa.end());

    //Ordenamos los pares por orden ascendente 
    sort(nodosOrdenados.begin(), nodosOrdenados.end(), 
        [](const pair<string, float>& a, const pair<string, float>& b) {
            return a.second > b.second; 
        });

    //Eliminamos los primeros 10 nodos, con mayor valor
    for (int i = 0; i < 10; ++i) {
        string nodoObjetivo = nodosOrdenados[i].first;
        float valor = nodosOrdenados[i].second;

        g.getGrafoP().removeVertice(nodoObjetivo);
    }
    
}


int main(void){
  //Varibles y mapas para los resultados
  float ASPathResult;
  map<string,float> degreeResult;
  map<string,float> clossenessResult;
  map<string,float> harmonicResult;
  map<string,float> betweennessResult;
  map<string,float> percolationResult;
  map<string,float> pagerankResult;

  //Instancias para metricas
  Centralidad<string,float> metricas;
  
  //Instancia de grafos
  grafoProteinas grafoProteinas;


  //Lectura de dataset
  cout << "Tiempo construccion grafo proteinas: ";
  int aux = medirTiempo("Tiempo construccion grafo proteinas: ", [&]() {
   return grafoProteinas.leerArchivoProteinas();
  });
  cout <<" Segundos"<< endl;
  cout << "Memoria usada por Grafo Proteinas: " << grafoProteinas.getGrafoP().memoriaUsada() << " bytes\n" << endl;
  for(int i = 0 ; i<100 ; i++){
    cout<< i << ";" << flush;
    degreeResult = medirTiempo("Centralidad Grado: ",[&]() {
    return metricas.centralidadGrado(grafoProteinas.getGrafoP());
  });

    clossenessResult = medirTiempo("Centralidad Cercania: ",[&]() {
    return metricas.centralidadCercania(grafoProteinas.getGrafoP());
  });

    harmonicResult = medirTiempo("Centralidad Armonica: ",[&]() {
    return metricas.centralidadArmonica(grafoProteinas.getGrafoP());
  });

    ASPathResult = medirTiempo("Centralidad Camino Promedio: ",[&]() {
    return metricas.caminoPromedio(grafoProteinas.getGrafoP());
  });

    betweennessResult = medirTiempo("Centralidad Intermediacion: ",[&]() {
    return metricas.centralidadIntermediacion(grafoProteinas.getGrafoP());
  });

    percolationResult = medirTiempo("Centralidad Percolacion: ",[&]() {
    return metricas.centralidadPercolacion(
					   grafoProteinas.getGrafoP(),
					   grafoProteinas.getGrafoP().getEstados()
					   );
  });

    pagerankResult = medirTiempo("Centralidad PageRank: ",[&]() {
    return metricas.pageRank(grafoProteinas.getGrafoP());
  });

  cout << endl;
  }
  /*
  // Mostramos los resultados de las metricas
  exportarCSV(degreeResult, "DegreeResultV1.csv");
  exportarCSV(clossenessResult, "Centralidad CercaniaV1.csv");
  exportarCSV(harmonicResult, "Centralidad ArmonicaV1.csv");
  cout << "Average Shortest Path: " << ASPathResult << endl;
  exportarCSV(betweennessResult, "Centralidad IntermediacionV1.csv");
  exportarCSV(percolationResult, "Centralidad PercolacionV1.csv");
  exportarCSV(pagerankResult, "PageRankV1.csv");

  eliminarAristas(degreeResult, grafoProteinas);
  eliminarAristas(betweennessResult,grafoProteinas);
  eliminarAristas(harmonicResult, grafoProteinas);

   degreeResult = medirTiempo("Centralidad Grado: ",[&]() {
    return metricas.centralidadGrado(grafoProteinas.getGrafoP());
  });

    clossenessResult = medirTiempo("Centralidad Cercania: ",[&]() {
    return metricas.centralidadCercania(grafoProteinas.getGrafoP());
  });

    harmonicResult = medirTiempo("Centralidad Armonica: ",[&]() {
    return metricas.centralidadArmonica(grafoProteinas.getGrafoP());
  });

    ASPathResult = medirTiempo("Centralidad Camino Promedio: ",[&]() {
    return metricas.caminoPromedio(grafoProteinas.getGrafoP());
  });

    betweennessResult = medirTiempo("Centralidad Intermediacion: ",[&]() {
    return metricas.centralidadIntermediacion(grafoProteinas.getGrafoP());
  });

    percolationResult = medirTiempo("Centralidad Percolacion: ",[&]() {
    return metricas.centralidadPercolacion(
					   grafoProteinas.getGrafoP(),
					   grafoProteinas.getGrafoP().getEstados()
					   );
  });

    pagerankResult = medirTiempo("Centralidad PageRank: ",[&]() {
    return metricas.pageRank(grafoProteinas.getGrafoP());
  });



    exportarCSV(degreeResult, "DegreeResultV2.csv");
    exportarCSV(harmonicResult, "Centralidad ArmonicaV2.csv");
      exportarCSV(betweennessResult, "Centralidad IntermediacionV2.csv");
  */
}
 
  
