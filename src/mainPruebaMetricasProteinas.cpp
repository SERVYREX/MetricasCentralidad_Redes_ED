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


    grafoProteinas lectorProteinas;
    GrafoAdList<string, float> grafoP(false);

    //Medimos y mostramos los datos de interes del grafo
    cerr << "Tiempo construccion grafo proteinas: ";

    auto start_grafo = chrono::high_resolution_clock::now();
    
    int aux = lectorProteinas.leerArchivoProteinas(grafoP);
    
    auto end_grafo = chrono::high_resolution_clock::now();
    double tiempo_grafo = chrono::duration_cast<chrono::nanoseconds>(end_grafo - start_grafo).count() * 1e-9;

    cerr << tiempo_grafo << " Segundos" << endl;

    cerr << "Memoria usada por Grafo Proteinas: " 
         << grafoP.memoriaUsada() << " bytes\n" << endl;


    //Primera iteracion de las metricas sobre el grafo

    cerr << "Procesando Metricas Iniciales..." << endl;
    
   
    cout << "V1" << endl; 

    for (int i = 0; i < repeticiones; i++) {
        
        
        cout << i + 1 << ";";

        degreeResult = medirTiempo("", [&]() {
            return metricas.centralidadGrado(grafoP);
        });

        clossenessResult = medirTiempo("", [&]() {
            return metricas.centralidadCercania(grafoP);
        });

        harmonicResult = medirTiempo("", [&]() {
            return metricas.centralidadArmonica(grafoP);
        });

        ASPathResult = medirTiempo("", [&]() {
            return metricas.caminoPromedio(grafoP);
        });

        betweennessResult = medirTiempo("", [&]() {
            return metricas.centralidadIntermediacion(grafoP);
        });

        percolationResult = medirTiempo("", [&]() {
            return metricas.centralidadPercolacion(
                grafoP,
                grafoP.getEstados()
            );
        });

        pagerankResult = medirTiempo("", [&]() {
            return metricas.pageRank(grafoP);
        });
        
        cout << endl;
    }

    //Exportamos los mapas a formato csv para posterior analisis
    cerr<<endl;
    cerr<< "Average Shortest Path: " <<ASPathResult << endl;
    exportarCSV(degreeResult, "../ResultadosP/DegreeResultV1.csv");
    exportarCSV(clossenessResult, "../ResultadosP/Centralidad CercaniaV1.csv");
    exportarCSV(harmonicResult, "../ResultadosP/Centralidad ArmonicaV1.csv");
    exportarCSV(betweennessResult, "../ResultadosP/Centralidad IntermediacionV1.csv");
    exportarCSV(percolationResult, "../ResultadosP/Centralidad PercolacionV1.csv");
    exportarCSV(pagerankResult, "../ResultadosP/PageRankV1.csv");


  //Ejecutamos el experimento correspondiente
    
    if(experimento == 0) {
    //Eliminamos los vertices y por consecuente las aristas mas importantes reportadas por las metrcias Degree,Betweenness y harmonic
      
    cerr << "Eliminando aristas y vertices clave. Reprocesando..." << endl;
    eliminarAristas(degreeResult, grafoP);
    eliminarAristas(betweennessResult, grafoP);
    eliminarAristas(harmonicResult, grafoP);
    }
    else{
      //En caso contrario agregamos aristas entre los nodos con menor valor del closseness
      cerr<< "Agregando aristas en puntos clave..."<< endl;
      agregarAristas(clossenessResult,grafoP);
    }



    cerr << endl;
    



    
    cout << "V2" << endl;

    for (int i = 0; i < repeticiones; i++) {
        
        cout << i + 1 << ";";

        degreeResult = medirTiempo("", [&]() {
            return metricas.centralidadGrado(grafoP);
        });

        clossenessResult = medirTiempo("", [&]() {
            return metricas.centralidadCercania(grafoP);
        });

        harmonicResult = medirTiempo("", [&]() {
            return metricas.centralidadArmonica(grafoP);
        });

        ASPathResult = medirTiempo("", [&]() {
            return metricas.caminoPromedio(grafoP);
        });

        betweennessResult = medirTiempo("", [&]() {
            return metricas.centralidadIntermediacion(grafoP);
        });

        percolationResult = medirTiempo("", [&]() {
            return metricas.centralidadPercolacion(
                grafoP,
                grafoP.getEstados()
            );
        });

        pagerankResult = medirTiempo("", [&]() {
            return metricas.pageRank(grafoP);
        });
        
        cout << endl;
    }


    //Finalmente exportamos los resultados luego de la eliminacion
    cerr<<endl;
    cerr<< "Average Shortest Path: " <<ASPathResult << endl;
    exportarCSV(degreeResult, "../ResultadosPV2/DegreeResultV2.csv");
    exportarCSV(clossenessResult, "../ResultadosPV2/Centralidad CercaniaV2.csv");
    exportarCSV(harmonicResult, "../ResultadosPV2/Centralidad ArmonicaV2.csv");
    exportarCSV(betweennessResult, "../ResultadosPV2/Centralidad IntermediacionV2.csv");
    exportarCSV(percolationResult, "../ResultadosPV2/Centralidad PercolacionV2.csv");
    exportarCSV(pagerankResult, "../ResultadosPV2/PageRankV2.csv");

    cerr<<endl;
    cerr << "Proceso Finalizado" << endl;
    return 0;
}
