#include <cstdlib>
#include <ctime>
#include <iostream>
#include <typeinfo>

using namespace std;

class NonVirtualBase {
};

class NonVirtualDerived : public NonVirtualBase {
};

class Grand {
private:
    int hold;

public:
    Grand(int h = 0)
        : hold(h) {}
    virtual void Speak() const { cout << "I'm a grand class" << endl; }
    virtual int Value() const { return hold; }
};

class Superb : public Grand {
public:
    Superb(int h = 0)
        : Grand(h) {}
    void Speak() const { cout << "I'm a superb class" << endl; }
    virtual void Say() const { cout << "I'm hold the superb value of " << Value() << endl; }
};

class Magnificent : public Superb {
private:
    char ch;

public:
    Magnificent(int h = 0, char c = 'A')
        : Superb(h), ch(c) {}
    void Speak() const { cout << "I'm a magnificent class" << endl; }
    void Say() const { cout << "I hold the character " << ch << " and the integer " << Value() << endl; }
};

Grand *GetOne()
{
    Grand *p;
    switch (rand() % 3) {
        case 0:
            p = new Grand(rand() % 100);
            break;
        case 1:
            p = new Superb(rand() % 100);
            break;
        case 2:
            p = new Magnificent(rand() % 100, 'A' + rand() % 26);
            break;
    }
    return p;
}

int main()
{
    NonVirtualBase *bp = new NonVirtualDerived();
    if (typeid(*bp) == typeid(NonVirtualBase)) {
        cout << "Class NonVirtualBase" << endl;
    } else if (typeid(*bp) == typeid(NonVirtualDerived)) {
        cout << "Class NonVirtualDerived" << endl;
    }
    cout << endl;
    // rtti only works for classes that have virtual member functions
    // cout << dynamic_cast<NonVirtualDerived *>(bp) << endl;
    delete bp;
    srand(time(NULL));
    Grand *pg;
    Superb *ps;
    for (int i = 0; i < 5; ++i) {
        pg = GetOne();
        // assigning type_info is not allowed
        // auto pgtype = typeid(*pg);

        if (typeid(*pg) == typeid(Grand)) {
            cout << "Class Grand" << endl;
        } else if (typeid(*pg) == typeid(Superb)) {
            cout << "Class Superb" << endl;
        } else if (typeid(*pg) == typeid(Magnificent)) {
            cout << "Class Magnificent" << endl;
        } else {
            cout << "Class unknown!!!" << endl;
        }

        Grand &grnd = *pg;
        grnd.Speak();
        if (ps = dynamic_cast<Superb *>(pg)) {
            ps->Say();
        }
        try {
            Superb &sprb = dynamic_cast<Superb &>(grnd);
            sprb.Say();
        } catch (const bad_cast &e) {
            cout << e.what() << endl;
        };
        cout << endl;
    }
    return 0;
}