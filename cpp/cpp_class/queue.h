#ifndef _QUEUE_H_
#define _QUEUE_H_
#include <iostream>

class Customer
{
private:
    long arrive;
    int processTime;

public:
    Customer();
    void set(long when);
    long when() const { return arrive; }
    int pTime() const { return processTime; }
};

typedef Customer Item;

class Queue
{
public:
    Queue(int qs = QSIZE);
    Queue(const Queue &q);
    ~Queue();
    bool isEmpty() const;
    bool isFull() const;
    int queueCount() const;
    bool enqueue(const Item &item);
    bool dequeue(Item &item);

    friend std::ostream &operator<<(std::ostream &os, const Queue &q);

private:
    enum
    {
        QSIZE = 10
    };
    struct Node
    {
        Item item;
        struct Node *next;
    };
    Node *mFront;
    Node *mRear;
    int mItems;
    const int mSize;
};

#endif
