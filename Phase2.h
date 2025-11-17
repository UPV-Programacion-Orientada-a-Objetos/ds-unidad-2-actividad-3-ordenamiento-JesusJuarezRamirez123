#ifndef PHASE2_H
#define PHASE2_H

class Phase2 {
private:
    void generateChunkFilename(int chunkNumber, char* filename);
    int findMinimumIndex(int* values, bool* hasData, int k);
    
public:
    Phase2();
    
    void runMerge(int k, const char* outputFilename);
};

#endif
