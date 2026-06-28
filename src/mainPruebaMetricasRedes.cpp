#include <iostream>
#include <chrono>
#include "../include/leerDataSet.h"
#include "../include/Metricas.h"
#include "../include/metricasUtils.h"
using namespace std;


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
  
  //Instancias del grafo
  grafoRedes grafoRedes;
  grafoRedes.leerArchivoRedes();
  //Lectura de dataset
  cout << "Tiempo construccion grafo redes: ";
  int aux = medirTiempo("Tiempo construccion grafo redes: ", [&]() {
    return grafoRedes.leerArchivoRedes();
  });
 
  cout <<" Segundos" <<endl;
    cout << "Memoria usada por Grafo Redes: " << grafoRedes.getGrafoR().memoriaUsada() << " bytes\n" << endl;
  for(int i = 0 ; i<100; i++){
    cout<< i << ";" << flush;
    degreeResult = medirTiempo("Centralidad Grado: ",[&]() {
      return metricas.centralidadGrado(grafoRedes.getGrafoR());
    });

    clossenessResult = medirTiempo("Centralidad Cercania: ",[&]() {
      return metricas.centralidadCercania(grafoRedes.getGrafoR());
    });

    harmonicResult = medirTiempo("Centralidad Armonica: ",[&]() {
      return metricas.centralidadArmonica(grafoRedes.getGrafoR());
    });

    ASPathResult = medirTiempo("Centralidad Camino Promedio: ",[&]() {
      return metricas.caminoPromedio(grafoRedes.getGrafoR());
    });

    betweennessResult = medirTiempo("Centralidad Intermediacion: ",[&]() {
      return metricas.centralidadIntermediacion(grafoRedes.getGrafoR());
    });

    percolationResult = medirTiempo("Centralidad Percolacion: ",[&]() {
      return metricas.centralidadPercolacion(
					     grafoRedes.getGrafoR(),
					     grafoRedes.getGrafoR().getEstados()
					     );
    });

    pagerankResult = medirTiempo("Centralidad PageRank: ",[&]() {
      return metricas.pageRank(grafoRedes.getGrafoR());
    });

    cout << endl;
  }
  // Mostramos los resultados de las metricas

  // showInformation(degreeResult, "Centralidad Grado");
  // showInformation(clossenessResult, "Centralidad Cercania");
  //showInformation(harmonicResult, "Centralidad Armonica");
  //cout << "Average Shortest Path: " << ASPathResult << endl;
  //showInformation(betweennessResult, "Centralidad Intermediacion");
  //showInformation(percolationResult, "Centralidad Percolacion");
  //showInformation(pagerankResult, "PageRank");
  
}
 
  
