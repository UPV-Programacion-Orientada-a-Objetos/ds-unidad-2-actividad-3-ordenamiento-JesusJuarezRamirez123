#include "Phase1.h"
#include <iostream>
#include <conio.h>

Phase1::Phase1(int bufferCapacity) : bufferSize(bufferCapacity), chunkCount(0) {}

void Phase1::generateChunkFilename(int chunkNumber, char* filename) {
    // Genera "chunk_0.tmp", "chunk_1.tmp", etc.
    const char* prefix = "chunk_";
    const char* suffix = ".tmp";
    
    int i = 0;
    while (prefix[i] != '\0') {
        filename[i] = prefix[i];
        i++;
    }
    
    // Convertir número a string
    int num = chunkNumber;
    int digits = 0;
    int temp = num;
    
    if (temp == 0) {
        filename[i++] = '0';
    } else {
        // Contar dígitos
        while (temp > 0) {
            digits++;
            temp /= 10;
        }
        
        // Escribir dígitos
        int start = i;
        for (int j = digits - 1; j >= 0; j--) {
            int digit = num % 10;
            filename[start + j] = '0' + digit;
            num /= 10;
        }
        i += digits;
    }
    
    // Agregar sufijo
    int j = 0;
    while (suffix[j] != '\0') {
        filename[i++] = suffix[j++];
    }
    filename[i] = '\0';
}

int Phase1::runAcquisition(DataSource* source) {
    CircularBuffer buffer(bufferSize);
    int totalRead = 0;
    
    std::cout << "Iniciando Fase 1: Adquisicion de datos..." << std::endl;
    std::cout << "Presione 'Q' para detener y procesar los datos.\n" << std::endl;
    
    while (source->hasMoreData()) {
        // Verificar si se presionó una tecla
        if (_kbhit()) {
            char key = _getch();
            if (key == 'q' || key == 'Q') {
                std::cout << "\n\nDetenido por el usuario. Procesando datos..." << std::endl;
                break;
            }
        }
        
        int value = source->getNext();
        std::cout << "Leyendo -> " << value << std::endl;
        
        buffer.insert(value);
        totalRead++;
        
        if (buffer.isFull()) {
            std::cout << "Buffer lleno. Ordenando internamente..." << std::endl;
            
            buffer.sort();
            std::cout << "Buffer ordenado." << std::endl;
            
            char filename[64];
            generateChunkFilename(chunkCount, filename);
            
            std::cout << "Escribiendo " << filename << "... ";
            buffer.writeToFile(filename);
            std::cout << "OK." << std::endl;
            
            buffer.clear();
            std::cout << "Buffer limpiado.\n" << std::endl;
            
            chunkCount++;
        }
    }
    
    // Si quedan datos en el buffer
    if (buffer.getSize() > 0) {
        std::cout << "Procesando datos restantes en buffer..." << std::endl;
        buffer.sort();
        
        char filename[64];
        generateChunkFilename(chunkCount, filename);
        
        std::cout << "Escribiendo " << filename << "... ";
        buffer.writeToFile(filename);
        std::cout << "OK." << std::endl;
        
        chunkCount++;
    }
    
    std::cout << "\nFase 1 completada. " << chunkCount << " chunks generados." << std::endl;
    
    return totalRead;
}

int Phase1::getChunkCount() const {
    return chunkCount;
}
