#ifndef PHASE1_H
#define PHASE1_H

#include "DataSource.h"
#include "CircularBuffer.h"

class Phase1 {
private:
    int bufferSize;
    int chunkCount;
    
    void generateChunkFilename(int chunkNumber, char* filename);
    
public:
    Phase1(int bufferCapacity);
    
    int runAcquisition(DataSource* source);
    int getChunkCount() const;
};

#endif
