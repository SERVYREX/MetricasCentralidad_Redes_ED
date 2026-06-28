
#include <iostream>
#include "../include/leerDataSet.h"
#include "../include/Metricas.h"
using namespace std;

int main(){
  std::map<string,double> resultado;
  Centralidad<string, float> metricas;
  grafoProteinas grafoProteinas; // grafo no dirigido
  grafoProteinas.leerArchivoProteinas();
  grafoRedes grafoRedes; // grafo dirigido
  grafoRedes.leerArchivoRedes();
  resultado = metricas.centralidadCercania(grafoProteinas.getGrafoP());
  
  float contador = 0;
  int contador2 = 0;
  string nodoMaxCentralidad;
  double suma = 0.0;
  for(const auto& par : resultado){
    cout << "Nodo: " << par.first << ", cercania: " << par.second << endl;
    suma += par.second;
    if(par.second > contador){
      contador = par.second;
      nodoMaxCentralidad = par.first;
    }
  }
  cout << "Valor máximo de cercania: " << contador << endl;
  cout << "Nodo con mayor centralidad: " << nodoMaxCentralidad << endl;
  cout << suma << endl;
  return 0;
  
}
