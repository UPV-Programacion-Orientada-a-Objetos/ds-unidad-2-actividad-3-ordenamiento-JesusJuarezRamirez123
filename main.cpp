#include <iostream>
#include <conio.h>
#include "SerialSource.h"
#include "Phase1.h"
#include "Phase2.h"

int main() {
    std::cout << "====================================================" << std::endl;
    std::cout << "  Sistema de Ordenamiento Externo E-Sort" << std::endl;
    std::cout << "====================================================" << std::endl;
    std::cout << std::endl;
    
    // Solicitar puerto al usuario
    char portInput[10];
    std::cout << "Ingrese el numero de puerto COM (ej: 3, 5, 7): ";
    std::cin >> portInput;
    
    // Construir el nombre del puerto
    char portName[20] = "\\\\.\\COM";
    int i = 7;  // Posición después de "\\.\COM"
    int j = 0;
    while (portInput[j] != '\0') {
        portName[i++] = portInput[j++];
    }
    portName[i] = '\0';
    
    // Configuración
    const int bufferSize = 100;  // Tamaño del buffer circular
    const char* outputFile = "output.sorted.txt";
    
    // Conectar al Arduino
    std::cout << "\nConectando a puerto " << portName << " (Arduino)... ";
    SerialSource* serialSource = new SerialSource(portName);
    
    if (!serialSource->isConnected()) {
        std::cout << "Fallo.\n" << std::endl;
        std::cout << "No se pudo conectar al Arduino." << std::endl;
        std::cout << "Verifique:" << std::endl;
        std::cout << "1. El Arduino esta conectado" << std::endl;
        std::cout << "2. El puerto COM es correcto (actualmente: " << portName << ")" << std::endl;
        std::cout << "3. Ningun otro programa esta usando el puerto" << std::endl;
        delete serialSource;
        return 1;
    }
    
    std::cout << "Conectado.\n" << std::endl;
    
    // Fase 1: Adquisición
    Phase1 phase1(bufferSize);
    
    // Wrapper para permitir interrupción con ESC
    bool userStopped = false;
    int totalRead = 0;
    
    // Nota: En producción, esto requeriría un wrapper más sofisticado
    // Para simplificar, asumimos que el usuario controlará el flujo
    
    int chunkCount = phase1.runAcquisition(serialSource);
    
    delete serialSource;
    
    if (chunkCount == 0) {
        std::cout << "\nNo se generaron chunks. Finalizando." << std::endl;
        return 0;
    }
    
    // Fase 2: Fusión
    Phase2 phase2;
    phase2.runMerge(chunkCount, outputFile);
    
    std::cout << "\nLiberando memoria... Sistema apagado." << std::endl;
    
    return 0;
}
