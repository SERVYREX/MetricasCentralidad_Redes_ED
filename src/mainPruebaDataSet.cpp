
#include <iostream>
#include "../include/leerDataSet.h"
using namespace std;

int main(){
    grafoProteinas grafoProteinas; // grafo no dirigido
    grafoProteinas.leerArchivoProteinas();
    grafoRedes grafoRedes; // grafo dirigido
    grafoRedes.leerArchivoRedes();
    cout << "Número de vértices en el grafo de red: " << grafoRedes.getGrafoR().getNumVertices() << endl;
    cout << "Número de aristas en el grafo de red: " << grafoRedes.getGrafoR().getNumAristas() << endl;

    return 0;

}