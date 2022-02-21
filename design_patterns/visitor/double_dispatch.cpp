#include <iostream>

using namespace std;

class Dot;
class Rectangle;
class Circle;

class Visitor
{
public:
    void visit(const Dot *d) const { cout << "Visit Dot." << endl; }
    void visit(const Rectangle *r) const { cout << "Visit Rectangle." << endl; }
    void visit(const Circle *c) const { cout << "Visit Circle." << endl; }
};

class Shape
{
protected:
    int id;

public:
    Shape(int id) : id(id) {}
    virtual void draw() = 0;
    virtual void accept(const Visitor *visitor) const = 0;
    friend class Visitor;
};

class Dot : public Shape
{
private:
    double x;
    double y;

public:
    Dot(double x, double y, int id) : Shape(id), x(x), y(y) {}
    void draw() { cout << "Draw a Dot." << endl; }
    void accept(const Visitor *visitor) const { visitor->visit(this); }
    friend class Visitor;
};

class Rectangle : public Shape
{
private:
    double width;
    double height;

public:
    Rectangle(double w, double h, int id) : Shape(id), width(w), height(h) {}
    void draw() { cout << "Draw a Rectange." << endl; }
    void accept(const Visitor *visitor) const { visitor->visit(this); }
    friend class Visitor;
};

class Circle : public Shape
{
private:
    double radius;

public:
    Circle(double r, int id) : Shape(id), radius(r) {}
    void draw() { cout << "Draw a Circle." << endl; }
    void accept(const Visitor *visitor) const { visitor->visit(this); }
    friend class Visitor;
};

int main()
{
    auto vp = new Visitor();
    Shape *sp = new Rectangle(10, 10, 10086);
    sp->accept(vp);
    return 0;
}