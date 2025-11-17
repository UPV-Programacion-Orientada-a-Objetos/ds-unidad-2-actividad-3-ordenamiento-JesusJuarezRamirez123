#include "Phase2.h"
#include "FileSource.h"
#include "DataSource.h"
#include <iostream>
#include <fstream>

Phase2::Phase2() {}

void Phase2::generateChunkFilename(int chunkNumber, char* filename) {
    const char* prefix = "chunk_";
    const char* suffix = ".tmp";
    
    int i = 0;
    while (prefix[i] != '\0') {
        filename[i] = prefix[i];
        i++;
    }
    
    int num = chunkNumber;
    int digits = 0;
    int temp = num;
    
    if (temp == 0) {
        filename[i++] = '0';
    } else {
        while (temp > 0) {
            digits++;
            temp /= 10;
        }
        
        int start = i;
        for (int j = digits - 1; j >= 0; j--) {
            int digit = num % 10;
            filename[start + j] = '0' + digit;
            num /= 10;
        }
        i += digits;
    }
    
    int j = 0;
    while (suffix[j] != '\0') {
        filename[i++] = suffix[j++];
    }
    filename[i] = '\0';
}

int Phase2::findMinimumIndex(int* values, bool* hasData, int k) {
    int minIndex = -1;
    int minValue;
    
    for (int i = 0; i < k; i++) {
        if (hasData[i]) {
            if (minIndex == -1 || values[i] < minValue) {
                minIndex = i;
                minValue = values[i];
            }
        }
    }
    
    return minIndex;
}

void Phase2::runMerge(int k, const char* outputFilename) {
    std::cout << "\nIniciando Fase 2: Fusion Externa (K-Way Merge)" << std::endl;
    std::cout << "Abriendo " << k << " archivos fuente..." << std::endl;
    
    // Crear arreglo de punteros a DataSource
    DataSource** sources = new DataSource*[k];
    int* currentValues = new int[k];
    bool* hasData = new bool[k];
    
    // Abrir todos los archivos
    for (int i = 0; i < k; i++) {
        char filename[64];
        generateChunkFilename(i, filename);
        sources[i] = new FileSource(filename);
        
        if (sources[i]->hasMoreData()) {
            currentValues[i] = sources[i]->getNext();
            hasData[i] = true;
        } else {
            hasData[i] = false;
        }
    }
    
    std::cout << "K=" << k << ". Fusion en progreso..." << std::endl;
    
    // Abrir archivo de salida
    std::ofstream outputFile(outputFilename);
    
    if (!outputFile.is_open()) {
        std::cout << "Error al crear archivo de salida" << std::endl;
        
        // Limpiar memoria
        for (int i = 0; i < k; i++) {
            delete sources[i];
        }
        delete[] sources;
        delete[] currentValues;
        delete[] hasData;
        return;
    }
    
    int totalWritten = 0;
    
    // K-Way Merge
    while (true) {
        int minIndex = findMinimumIndex(currentValues, hasData, k);
        
        if (minIndex == -1) {
            break; // No hay más datos
        }
        
        // Escribir el mínimo al archivo
        outputFile << currentValues[minIndex] << "\n";
        totalWritten++;
        
        if (totalWritten % 100 == 0) {
            std::cout << " - Escritos " << totalWritten << " valores..." << std::endl;
        }
        
        // Avanzar en el archivo del cual se extrajo el mínimo
        if (sources[minIndex]->hasMoreData()) {
            currentValues[minIndex] = sources[minIndex]->getNext();
        } else {
            hasData[minIndex] = false;
        }
    }
    
    outputFile.close();
    
    std::cout << "\nFusion completada. Archivo final: " << outputFilename << std::endl;
    std::cout << "Total de valores escritos: " << totalWritten << std::endl;
    
    // Liberar memoria
    for (int i = 0; i < k; i++) {
        delete sources[i];
    }
    delete[] sources;
    delete[] currentValues;
    delete[] hasData;
}
