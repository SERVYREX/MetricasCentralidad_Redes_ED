#include <iostream>
#include <fstream>
#include <chrono>
#include <cstdlib>
#include "../include/leerDataSet.h"
#include "../include/Metricas.h"
#include "../include/metricasUtils.h"

using namespace std;

int main(int argc, char* argv[]) {

    int repeticiones = 1;
    int experimento = 0;
    if (argc > 2) {
        repeticiones = atoi(argv[1]);
	experimento = atoi(argv[2]);
        if (repeticiones <= 0) {
            cerr << "Uso: " << argv[0] << " <numero_repeticiones>" << endl;
            return 1;
        }
    }

    float ASPathResult;

    map<string,float> degreeResult;
    map<string,float> clossenessResult;
    map<string,float> harmonicResult;
    map<string,float> betweennessResult;
    map<string,float> percolationResult;
    map<string,float> pagerankResult;


    Centralidad<string,float> metricas;


    grafoRedes lectorRedes;
    GrafoAdList<string, float> grafoR(false);

    //Medimos y mostramos los datos de interes del grafo
    cerr << "Tiempo construccion grafo redes: ";

    auto start_grafo = chrono::high_resolution_clock::now();
    
    int aux = lectorRedes.leerArchivoRedes(grafoR);
    
    auto end_grafo = chrono::high_resolution_clock::now();
    double tiempo_grafo = chrono::duration_cast<chrono::nanoseconds>(end_grafo - start_grafo).count() * 1e-9;

    cerr << tiempo_grafo << " Segundos" << endl;

    cerr << "Memoria usada por Grafo Redes: " 
         << grafoR.memoriaUsada() << " bytes\n" << endl;


    //Primera iteracion de las metricas sobre el grafo

    cerr << "Procesando Metricas Iniciales..." << endl;
    
   
    cout << "V1" << endl; 

    for (int i = 0; i < repeticiones; i++) {
        
        
        cout << i + 1 << ";";

        degreeResult = medirTiempo("", [&]() {
            return metricas.centralidadGrado(grafoR);
        });

        clossenessResult = medirTiempo("", [&]() {
            return metricas.centralidadCercania(grafoR);
        });

        harmonicResult = medirTiempo("", [&]() {
            return metricas.centralidadArmonica(grafoR);
        });

        ASPathResult = medirTiempo("", [&]() {
            return metricas.caminoPromedio(grafoR);
        });

        betweennessResult = medirTiempo("", [&]() {
            return metricas.centralidadIntermediacion(grafoR);
        });

        percolationResult = medirTiempo("", [&]() {
            return metricas.centralidadPercolacion(
                grafoR,
                grafoR.getEstados()
            );
        });

        pagerankResult = medirTiempo("", [&]() {
            return metricas.pageRank(grafoR);
        });
        
        cout << endl;
    }

    //Exportamos los mapas a formato csv para posterior analisis y mostramos el valor de ASP
    cerr<<endl;
    cerr<< "Average Shortest Path: " <<ASPathResult << endl;
    exportarCSV(degreeResult, "../ResultadosR/DegreeResultV1.csv");
    exportarCSV(clossenessResult, "../ResultadosR/Centralidad CercaniaV1.csv");
    exportarCSV(harmonicResult, "../ResultadosR/Centralidad ArmonicaV1.csv");
    exportarCSV(betweennessResult, "../ResultadosR/Centralidad IntermediacionV1.csv");
    exportarCSV(percolationResult, "../ResultadosR/Centralidad PercolacionV1.csv");
    exportarCSV(pagerankResult, "../ResultadosR/PageRankV1.csv");

    //Ejecutamos el experimento correspondiente
    
    if(experimento == 0) {
    //Eliminamos los vertices y por consecuente las aristas mas importantes reportadas por las metrcias Degree,Betweenness y harmonic
    cerr << "Eliminando aristas y vertices clave. Reprocesando..." << endl;
    
    eliminarAristas(degreeResult, grafoR);
    eliminarAristas(betweennessResult, grafoR);
    eliminarAristas(harmonicResult, grafoR);
    }
    else{
      cerr<<"Agregando Aristas en puntos aislados.." << endl;
      agregarAristas(clossenessResult,grafoR);
    }


    cerr << endl;
    



    cout << "V2" << endl;

    for (int i = 0; i < repeticiones; i++) {
        
        cout << i + 1 << ";";

        degreeResult = medirTiempo("", [&]() {
            return metricas.centralidadGrado(grafoR);
        });

        clossenessResult = medirTiempo("", [&]() {
            return metricas.centralidadCercania(grafoR);
        });

        harmonicResult = medirTiempo("", [&]() {
            return metricas.centralidadArmonica(grafoR);
        });

        ASPathResult = medirTiempo("", [&]() {
            return metricas.caminoPromedio(grafoR);
        });

        betweennessResult = medirTiempo("", [&]() {
            return metricas.centralidadIntermediacion(grafoR);
        });

        percolationResult = medirTiempo("", [&]() {
            return metricas.centralidadPercolacion(
                grafoR,
                grafoR.getEstados()
            );
        });

        pagerankResult = medirTiempo("", [&]() {
            return metricas.pageRank(grafoR);
        });
        
        cout << endl;
    }


    //Finalmente exportamos los resultados luego de la eliminacion y mostramos el valor del ASP
    cerr<<endl;
    cerr<< "Average Shortest Path: " <<ASPathResult << endl;
    exportarCSV(degreeResult, "../ResultadosRV2/DegreeResultV2.csv");
    exportarCSV(clossenessResult, "../ResultadosRV2/Centralidad CercaniaV2.csv");
    exportarCSV(harmonicResult, "../ResultadosRV2/Centralidad ArmonicaV2.csv");
    exportarCSV(betweennessResult, "../ResultadosRV2/Centralidad IntermediacionV2.csv");
    exportarCSV(percolationResult, "../ResultadosRV2/Centralidad PercolacionV2.csv");
    exportarCSV(pagerankResult, "../ResultadosRV2/PageRankV2.csv");

    cerr<<endl;
    cerr << "Proceso Finalizado" << endl;
    return 0;
}
