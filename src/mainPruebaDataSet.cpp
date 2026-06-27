
#include <iostream>
#include "../include/leerDataSet.h"
#include "../include/Metricas.h"
using namespace std;

int main(){
    std::map<string, double> resultado;
    Centralidad<string, float> metricas;
    grafoProteinas grafoProteinas; // grafo no dirigido
    grafoProteinas.leerArchivoProteinas();
    grafoRedes grafoRedes; // grafo dirigido
    grafoRedes.leerArchivoRedes();
    cout << "Número de vértices en el grafo de red: " << grafoRedes.getGrafoR().getNumVertices() << endl;
    cout << "Número de aristas en el grafo de red: " << grafoRedes.getGrafoR().getNumAristas() << endl;
    resultado = metricas.centralidadCercania(grafoProteinas.getGrafoP());

    float contador = 0;
    int contador2 = 0;
    string nodoMaxCentralidad;
    for(const auto& par : resultado){
        cout << "Nodo: " << par.first << ", cercania: " << par.second << endl;
        if(par.second > contador){
            contador = par.second;
            nodoMaxCentralidad = par.first;
        }
    }
    cout << "Valor máximo de cercania: " << contador << endl;
    cout << "Nodo con mayor centralidad: " << nodoMaxCentralidad << endl;
    return 0;

}