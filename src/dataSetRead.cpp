#include "../include/GrafoADT.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

int main(){
    GrafoAdList<string, int> grafoProteinas(false); // grafo no dirigido

    ifstream archivo("../DATA/yeast.edgelist");

    
    if(!archivo.is_open()) {
        return 1;
    }

    string vertice1, vertice2;
    int totalAristas = 0;
    while(archivo >> vertice1 >> vertice2) {
        grafoProteinas.addArista(vertice1, vertice2, 1); // Asignamos un peso de 1 a cada arista, ya que estos no tienen en el archivo
        cout << "Agregada arista: " << vertice1 << " - " << vertice2 << endl;
        totalAristas++;
    }
    cout << "Total de aristas agregadas: " << totalAristas << endl;

    archivo.close();

    GrafoAdList<string, float> grafoNetwork(true); // grafo dirigido
    ifstream archivo2("../DATA/train_test_network.csv");
    if(!archivo2.is_open()) {
        return 1;
    }



    string linea;
    if (getline(archivo2, linea)) {} // nos saltamos la primera linea del archivo (la cabecera)
    
    string basura; //para sobreescribir las lineas que no nos sirven
    while(getline(archivo2, linea)){
        stringstream ss(linea);
        // solo nos sirve src_ip(nodo 1), dst_ip(nodo 2), duration(peso), la columna 1, 3 y 7
        string nodo1, nodo2;
        string pesotemp;
        float peso;
        for(int i = 0; i < 7; i++){ //solo recorremos las primeras 7 columnas, ya que en estas estan los datos que nos interesan
            if(i == 0) { // src_ip
                getline(ss, nodo1, ',');
            }
            else if(i == 2){ // dst_ip
                getline(ss, nodo2, ',');
            }
            else if(i == 6){ // duration
                getline(ss, pesotemp, ','); // ignoramos esta columna
                peso = stof(pesotemp);
            }
            else { // las otras columnas que no nos interesan
                getline(ss, basura, ',');
            }
        }
        grafoNetwork.addArista(nodo1, nodo2, peso);
        cout << "Agregada arista: " << nodo1 << " -> " << nodo2 << " con peso: " << peso << endl;

    }
    cout << "Número de vértices en el grafo de red: " << grafoNetwork.getNumVertices() << endl;
    cout << "Número de aristas en el grafo de red: " << grafoNetwork.getNumAristas() << endl;
    cout << "Número de vértices: " << grafoProteinas.getNumVertices() << endl;
    cout << "Número de aristas: " << grafoProteinas.getNumAristas() << endl;

    return 0;

}