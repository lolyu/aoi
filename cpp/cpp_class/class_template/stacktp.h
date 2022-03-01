#ifndef _STACKTP_H_
#define _STACKTP_H_
template <typename Type>
class Stack {
private:
    enum { MAX = 10 };
    Type items[MAX];
    int top;

public:
    Stack();
    bool isempty();
    bool isfull();
    bool push(const Type &item);
    bool pop(Type &item);
};

template <typename Type>
Stack<Type>::Stack()
    : top(0)
{
}

template <typename Type>
bool Stack<Type>::isempty()
{
    return top == 0;
}

template <typename Type>
bool Stack<Type>::isfull()
{
    return top == MAX;
}

template <typename Type>
bool Stack<Type>::push(const Type &item)
{
    if (isfull()) {
        return false;
    } else {
        items[top++] = item;
        return true;
    }
}

template <typename Type>
bool Stack<Type>::pop(Type &item)
{
    if (isempty()) {
        return false;
    } else {
        item = items[--top];
        return true;
    }
}

#endif