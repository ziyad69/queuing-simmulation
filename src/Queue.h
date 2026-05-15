#ifndef QUEUE_H
#define QUEUE_H

#include <cassert>
#include <iostream>

template <typename T>
class Queue {
private:
    struct Node {
        T     data;
        Node* next;
        Node(const T& d) : data(d), next(nullptr) {}
    };

    Node* frontNode;
    Node* backNode;
    int   count;

public:
    Queue() : frontNode(nullptr), backNode(nullptr), count(0) {}

    ~Queue() {
        while (!isEmpty()) dequeue();
    }

    bool isEmpty() const { return count == 0; }
    int  size()    const { return count; }

    void enqueue(const T& item) {
        Node* newNode = new Node(item);
        if (backNode == nullptr) {
            frontNode = backNode = newNode;
        } else {
            backNode->next = newNode;
            backNode = newNode;
        }
        count++;
    }

    T front() const {
        assert(!isEmpty());
        return frontNode->data;
    }

    void dequeue() {
        assert(!isEmpty());
        Node* temp = frontNode;
        frontNode  = frontNode->next;
        if (frontNode == nullptr) backNode = nullptr;
        delete temp;
        count--;
    }
};

#endif
