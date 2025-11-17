#include "SerialSource.h"
#include <iostream>

SerialSource::SerialSource(const char* portName) : bufferIndex(0), bufferSize(0), connected(false) {
    hSerial = CreateFileA(portName,
                         GENERIC_READ | GENERIC_WRITE,
                         0,
                         NULL,
                         OPEN_EXISTING,
                         FILE_ATTRIBUTE_NORMAL,
                         NULL);
    
    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cout << "Error al abrir puerto " << portName << std::endl;
        return;
    }
    
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cout << "Error obteniendo estado del puerto" << std::endl;
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
        return;
    }
    
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    
    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cout << "Error configurando puerto" << std::endl;
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
        return;
    }
    
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    
    if (!SetCommTimeouts(hSerial, &timeouts)) {
        std::cout << "Error configurando timeouts" << std::endl;
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
        return;
    }
    
    connected = true;
}

SerialSource::~SerialSource() {
    if (hSerial != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial);
    }
}

bool SerialSource::readChar(char& c) {
    if (hSerial == INVALID_HANDLE_VALUE) return false;
    
    DWORD bytesRead;
    if (ReadFile(hSerial, &c, 1, &bytesRead, NULL) && bytesRead == 1) {
        return true;
    }
    return false;
}

int SerialSource::getNext() {
    char line[256];
    int lineIndex = 0;
    char c;
    
    while (lineIndex < 255) {
        if (readChar(c)) {
            if (c == '\n') {
                line[lineIndex] = '\0';
                return atoi(line);
            } else if (c != '\r') {
                line[lineIndex++] = c;
            }
        }
    }
    
    line[lineIndex] = '\0';
    return atoi(line);
}

bool SerialSource::hasMoreData() {
    return connected && (hSerial != INVALID_HANDLE_VALUE);
}

bool SerialSource::isConnected() const {
    return connected;
}
