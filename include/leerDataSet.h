#ifndef LEERDATASET_H
#define LEERDATASET_H

#include "GrafoADT.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

class grafoProteinas
{
public:
    int leerArchivoProteinas(GrafoAdList<std::string, float>& grafoP){
        std::ifstream archivo("../DATA/yeast.edgelist");

        if (!archivo.is_open()){
            return 1;
        }

        int peso;
        std::string vertice1, vertice2;
        while (archivo >> vertice1 >> vertice2 >> peso){ // lee de 2 en 2 palabras
            grafoP.addArista(vertice1, vertice2, peso);
        }

        archivo.close();
        return 0;
    }
};

class grafoRedes{
public:
    int leerArchivoRedes(GrafoAdList<std::string, float>& grafoR){
        std::ifstream archivo2("../DATA/train_test_network.csv");

        if (!archivo2.is_open()){
            return 1;
        }

        std::string linea;
        if (getline(archivo2, linea)){
        } // nos saltamos la primera linea del archivo (la cabecera)

        std::string basura; // para sobreescribir las lineas que no nos sirven
        while (getline(archivo2, linea))
        {
            std::stringstream ss(linea);
            // solo nos sirve src_ip(nodo 1), dst_ip(nodo 2), duration(peso), la columna 1, 3 y 7
            std::string nodo1, nodo2;
            std::string pesotemp;
            float peso;
            for (int i = 0; i < 7; i++)
            { // solo recorremos las primeras 7 columnas, ya que en estas estan los datos que nos interesan
                if (i == 0){ // src_ip
                    getline(ss, nodo1, ',');
                }
                else if (i == 2){ // dst_ip
                    getline(ss, nodo2, ',');
                }
                else if (i == 6){ // duration
                    getline(ss, pesotemp, ',');
                    peso = stof(pesotemp);
                }
                else{ // las otras columnas que no nos interesan
                    getline(ss, basura, ',');
                }
            }
            grafoR.addArista(nodo1, nodo2, peso);
            //std::cout << "Agregada arista: " << nodo1 << " -> " << nodo2 << " con peso: " << peso << std::endl;
        }

        archivo2.close();
        return 0;
    }
};

#endif
