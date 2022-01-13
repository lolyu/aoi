#include "queue.h"

Customer::Customer() : arrive(0), processTime(0) {}

void Customer::set(long when)
{
    arrive = when;
}

Queue::Queue(int qs) : mSize(qs), mFront(nullptr), mRear(nullptr), mItems(0) {}

Queue::Queue(const Queue &q) : mSize(q.mSize)
{
    mItems = 0;
    for (Node *nPtr = q.mFront; nPtr && enqueue(nPtr->item); nPtr = nPtr->next)
        ;
}

Queue::~Queue()
{
    Item item;
    while (dequeue(item));
}

bool Queue::isEmpty() const
{
    return mItems == 0;
}

bool Queue::isFull() const
{
    return mItems == mSize;
}

int Queue::queueCount() const
{
    return mItems;
}

bool Queue::enqueue(const Item &item)
{
    if (isFull())
    {
        return false;
    }
    Node *node = new Node();
    node->item = item;
    node->next = nullptr;
    if (isEmpty())
    {
        mFront = node;
    }
    else
    {
        mRear->next = node;
    }
    mRear = node;
    ++mItems;
    return true;
}

bool Queue::dequeue(Item &item)
{
    if (isEmpty())
    {
        return false;
    }
    Node *temp = mFront;
    item = mFront->item;
    mFront = mFront->next;
    delete temp;
    if (--mItems == 0)
    {
        mRear = nullptr;
    }
    return true;
}

std::ostream &operator<<(std::ostream &os, const Queue &q)
{
    for (Queue::Node *nPtr = q.mFront; nPtr; nPtr = nPtr->next)
    {
        os << nPtr << '\t';
    }
    return os;
}
