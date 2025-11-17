#include "CircularBuffer.h"
#include <fstream>
#include <iostream>

CircularBuffer::CircularBuffer(int maxSize) : head(nullptr), capacity(maxSize), currentSize(0) {}

CircularBuffer::~CircularBuffer() {
    clear();
}

void CircularBuffer::insert(int value) {
    if (currentSize >= capacity) {
        return; // Buffer lleno
    }
    
    Node* newNode = new Node(value);
    
    if (head == nullptr) {
        head = newNode;
        head->next = head;
        head->prev = head;
    } else {
        Node* tail = head->prev;
        tail->next = newNode;
        newNode->prev = tail;
        newNode->next = head;
        head->prev = newNode;
    }
    
    currentSize++;
}

bool CircularBuffer::isFull() const {
    return currentSize >= capacity;
}

void CircularBuffer::clear() {
    if (head == nullptr) return;
    
    Node* current = head;
    Node* nextNode;
    
    for (int i = 0; i < currentSize; i++) {
        nextNode = current->next;
        delete current;
        current = nextNode;
    }
    
    head = nullptr;
    currentSize = 0;
}

int CircularBuffer::getSize() const {
    return currentSize;
}

void CircularBuffer::swapData(Node* a, Node* b) {
    int temp = a->data;
    a->data = b->data;
    b->data = temp;
}

void CircularBuffer::insertionSort() {
    if (head == nullptr || currentSize <= 1) return;
    
    Node* sorted = head;
    Node* current = head->next;
    
    for (int i = 1; i < currentSize; i++) {
        Node* key = current;
        int keyData = key->data;
        Node* j = key->prev;
        
        bool moved = false;
        int count = i;
        
        while (count > 0 && j->data > keyData) {
            j->next->data = j->data;
            j = j->prev;
            count--;
            moved = true;
        }
        
        if (moved) {
            j->next->data = keyData;
        }
        
        current = current->next;
    }
}

void CircularBuffer::sort() {
    insertionSort();
}

void CircularBuffer::writeToFile(const char* filename) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cout << "Error al crear archivo " << filename << std::endl;
        return;
    }
    
    if (head == nullptr) {
        file.close();
        return;
    }
    
    Node* current = head;
    for (int i = 0; i < currentSize; i++) {
        file << current->data << "\n";
        current = current->next;
    }
    
    file.close();
}
