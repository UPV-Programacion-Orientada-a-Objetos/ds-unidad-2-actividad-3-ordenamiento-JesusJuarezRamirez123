#ifndef FILESOURCE_H
#define FILESOURCE_H

#include "DataSource.h"
#include <fstream>

class FileSource : public DataSource {
private:
    std::ifstream file;
    int nextValue;
    bool hasNext;
    
    void readNextValue();
    
public:
    FileSource(const char* filename);
    ~FileSource();
    
    int getNext() override;
    bool hasMoreData() override;
};

#endif
