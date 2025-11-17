#include "FileSource.h"

FileSource::FileSource(const char* filename) : hasNext(false) {
    file.open(filename);
    if (file.is_open()) {
        readNextValue();
    }
}

FileSource::~FileSource() {
    if (file.is_open()) {
        file.close();
    }
}

void FileSource::readNextValue() {
    if (file >> nextValue) {
        hasNext = true;
    } else {
        hasNext = false;
    }
}

int FileSource::getNext() {
    int value = nextValue;
    readNextValue();
    return value;
}

bool FileSource::hasMoreData() {
    return hasNext;
}
