#ifndef MEDICION_TIEMPO_H
#define MEDICION_TIMEPO_H


#include <iostream>
#include <chrono>
#include "leerDataSet.h"
#include "Metricas.h"
#include "GrafoADT.h"
using namespace std;


//Solo para debug

void showInformation(const std::map<string,float>& mapa, string tipoMetrica){
  for(const auto& par : mapa){
    cout << "Nodo: " << par.first <<", " << tipoMetrica << par.second <<endl;
  }
}



//Funcion que ejecuta una funcion (Metrica) y reporta su tiempo de ejecucion
template<typename Funcion>

auto medirTiempo(const string & nombreMetrica,Funcion f){
    
  auto start = chrono::high_resolution_clock::now();
  auto resultado = f();
  auto end = chrono::high_resolution_clock::now();

  // Calculamos el tiempo transcurrido
  double running_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
  running_time *= 1e-9;
  
  cout<< running_time << ";" << flush;
  
  return resultado;
}

//Funcion que exporta un map a un .csv
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

//Funcion que detecta y elimina
void eliminarAristas(const std::map<string, float>& mapa,
                     GrafoAdList<string, float>& g) {

    //Copiamos los pares del mapa a un vector
    vector<pair<string, float>> nodosOrdenados(mapa.begin(), mapa.end());

    //Ordenamos los pares por orden descendente
    sort(nodosOrdenados.begin(), nodosOrdenados.end(),
        [](const pair<string, float>& a, const pair<string, float>& b) {
            return a.second > b.second;
        });

    //Eliminamos los primeros 10 nodos, con mayor valor
    for (int i = 0; i < 10; ++i) {
        string nodoObjetivo = nodosOrdenados[i].first;
        float valor = nodosOrdenados[i].second;

        g.removeVertice(nodoObjetivo);
    }
}

#endif
