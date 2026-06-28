#include <iostream>
#include "../include/leerDataSet.h"
#include "../include/Metricas.h"
using namespace std;

void showInformation(const std::map<string,float>& mapa, string tipoMetrica){
  for(const auto& par : mapa){
    cout << "Nodo: " << par.first <<", " << tipoMetrica << par.second <<endl;
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
  
  //Instancias de grafos
  grafoProteinas grafoProteinas;
  grafoRedes grafoRedes;

  //Lectura de datasets
  grafoProteinas.leerArchivoProteinas();
  grafoRedes.leerArchivoRedes();

  //Metricas para GrafoRedes
  degreeResult = metricas.centralidadGrado(grafoRedes.getGrafoR());
  clossenessResult = metricas.centralidadCercania(grafoRedes.getGrafoR());
  harmonicResult = metricas.centralidadArmonica(grafoRedes.getGrafoR());
  ASPathResult = metricas.caminoPromedio(grafoRedes.getGrafoR());
  betweennessResult = metricas.centralidadIntermediacion(grafoRedes.getGrafoR());
  percolationResult = metricas.centralidadPercolacion(grafoRedes.getGrafoR(),grafoRedes.getGrafoR().getEstados());
  pagerankResult = metricas.pageRank(grafoRedes.getGrafoR());

  // Mostramos los resultados de las metricas

  showInformation(degreeResult, "Centralidad Grado");
  showInformation(clossenessResult, "Centralidad Cercania");
  showInformation(harmonicResult, "Centralidad Armonica");
  cout << "Average Shortest Path: " << ASPathResult << endl;
  showInformation(betweennessResult, "Centralidad Intermediacion");
  showInformation(percolationResult, "Centralidad Percolacion");
  showInformation(pagerankResult, "PageRank");
}
 
  
