# operators
* operators have two properties:
    * precedence: specifies the order of operations in expressions that contain more than one operator
    * associativity: specifies whether, in an expression that contains multiple operators with the same precedence, an operand is grouped with the one on its left or the one on its right.

## `alignof`
* `alignof` returns the alignment in bytes of the specified type

```cpp
struct Demo
{
    int i;
    double d;
    alignas(1024) char arr[3];
};

int main()
{
    using namespace std;
    cout << alignof(Demo) << endl;    // 1024

    void *p0 = malloc(sizeof(Demo) * 10);
    cout << p0 << endl;                                            // default aligned

    void *p1 = aligned_alloc(alignof(Demo), sizeof(Demo) * 10);    // aligned to 1024
    cout << p1 << endl;

    return 0;
}
```

## address-of `&`
* `&` is the address-of operator
    * it returns the address of its operand, the operand can be a function designator or an lvalue


### address of class member: static vs non-static
* the pointer to member data, it is more like declaring a member pointer that points to the member data.
* for any instance, the pointer to member data always points to the member data of this instance
    * https://www.ibm.com/docs/en/zos/2.2.0?topic=only-pointers-members-c
```
class D
{
public:
    int n;
    static int nn;
};

int D::nn = 10;

int main()
{
    using namespace std;
    int *p0 = &D::nn;
    int D::*p1 = &D::n;

    D d0, d1;
    d0.n = 10;
    d1.n = 11;

    cout << (d0.*p1) << endl;
    cout << &(d0.*p1) << endl;

    cout << (d1.*p1) << endl;
    cout << &(d1.*p1) << endl;
    return 0;
}
```

## assignment `=`
* two kinds of assignment operators:
    * simple assignment
    * compound assignment
        * `+=`
        * `-=`
        * `*=`
        * `/=`


## references
* https://stackoverflow.com/questions/670734/pointer-to-class-data-member
