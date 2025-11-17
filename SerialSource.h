#ifndef SERIALSOURCE_H
#define SERIALSOURCE_H

#include "DataSource.h"
#include <windows.h>

class SerialSource : public DataSource {
private:
    HANDLE hSerial;
    char buffer[256];
    int bufferIndex;
    int bufferSize;
    bool connected;
    
    bool readChar(char& c);
    
public:
    SerialSource(const char* portName);
    ~SerialSource();
    
    int getNext() override;
    bool hasMoreData() override;
    bool isConnected() const;
};

#endif
