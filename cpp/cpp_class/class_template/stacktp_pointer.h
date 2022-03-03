#ifndef _STACKTP_POINTER_H_
#define _STACKTP_POINTER_H_

template <class Type>
class Stack {
private:
    enum { SIZE = 10 };
    int stacksize;
    Type *items;
    int top;

public:
    explicit Stack(int ss = SIZE);
    Stack(const Stack &st);
    ~Stack() { delete[] items; }
    bool isempty() { return top == 0; }
    bool isfull() { return top == stacksize; }
    bool push(const Type &item);
    bool pop(const Type &item);
    Stack &operator=(const Stack &st);
};

template <class Type>
Stack<Type>::Stack(int ss)
    : stacksize(ss), items(new Type[stacksize])
{}

template <class Type>
Stack<Type>::Stack(const Stack &st)
{
    stacksize = st.stacksize;
    items = new Type[stacksize];
    for (top = 0; top < st.top; ++top) {
        items[top] = st.items[top];
    }
}

template <class Type>
bool Stack<Type>::push(const Type &item)
{
    if (isfull()) {
        return false;
    } else {
        items[top++] = item;
        return true;
    }
}

template <class Type>
bool Stack<Type>::pop(const Type &item)
{
    if (isempty()) {
        return false;
    } else {
        item = items[--top];
        return true;
    }
}

template <class Type>
Stack<Type> &Stack<Type>::operator=(const Stack &st)
{
    if (&st != this) {
        delete[] items;
        stacksize = st.stacksize;
        items = new Type[stacksize];
        for (top = 0; top < st.top; ++top) {
            items[top] = st.items[top];
        }
    }
    return *this;
}

#endif
