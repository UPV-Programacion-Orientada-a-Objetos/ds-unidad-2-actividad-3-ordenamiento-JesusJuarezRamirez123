#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

struct Node {
    int data;
    Node* next;
    Node* prev;
    
    Node(int value) : data(value), next(nullptr), prev(nullptr) {}
};

class CircularBuffer {
private:
    Node* head;
    int capacity;
    int currentSize;
    
    void insertionSort();
    void swapData(Node* a, Node* b);
    
public:
    CircularBuffer(int maxSize);
    ~CircularBuffer();
    
    void insert(int value);
    bool isFull() const;
    void clear();
    int getSize() const;
    
    void sort();
    void writeToFile(const char* filename);
};

#endif
