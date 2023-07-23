# standard conversions
* standard conversions:
    * integral promotion
    * integral conversion
    * floating conversion
    * floating and integral conversion
    * arithmetic conversion
    * pointer conversion
    * reference conversion
    * point-to-member conversion

## arithmetic conversion
```
    long double d0 = 0.1;
    double d1 = 0.2;
    float d2 = 0.3;
    unsigned long i0 = 1;
    long i1 = 2;
    unsigned int i2 = 3;
    long i3 = 4;
    unsigned int i4 = 4;
    int i5 = 5;

    auto sum0 = d0 + i5; // long double
    auto sum1 = d1 + i5; // double
    auto sum2 = d2 + i5; // float
    auto sum3 = i0 + i5; // unsigned long
    auto sum4 = i1 + i5; // long
    auto sum5 = i2 + i5; // unsigned int
    auto sum6 = i3 + i5; // long
    auto sum7 = i4 + i5; // unsigned int
    auto sum8 = i5 + i5; // int
    auto sum9 = i1 + i2; // long

    cout << typeid(sum0).name() << endl;
    cout << typeid(sum1).name() << endl;
    cout << typeid(sum2).name() << endl;
    cout << typeid(sum3).name() << endl;
    cout << typeid(sum4).name() << endl;
    cout << typeid(sum5).name() << endl;
    cout << typeid(sum6).name() << endl;
    cout << typeid(sum7).name() << endl;
    cout << typeid(sum8).name() << endl;
    cout << typeid(sum9).name() << endl;
```

* the arithmetic conversion rule of float and integral:
    * Both operands are converted to a common type `C`. Given the types `T1` and `T2` as the promoted type (under the rules of integral promotions) of the operands, the following rules are applied to determine `C`:
        * If `T1` and `T2` are the same type, `C` is that type.
        * Otherwise, if `T1` and `T2` are both signed integer types or both unsigned integer types, `C` is the type of greater integer conversion rank.
        * Otherwise, one type between `T1` and `T2` is a signed integer type `S`, and the other type is an unsigned integer type `U`. Apply the following rules:
            * If the integer conversion rank of `U` is greater than or equal to the integer conversion rank of `S`, `C` is `U`.
            * Otherwise, if `S` can represent all of the values of `U`, `C` is `S`.
            * Otherwise, `C` is the unsigned integer type corresponding to `S`.

## pointer conversion
### pointer to class
![image](https://github.com/lolyu/aoi/assets/35479537/e3581e82-c727-41b1-87c5-7c5b98f34aac)
* is `auto bptr = new B(); A *aptr = bptr;` legal?
    * only if:
        * the base class is accessible
        * the conversion is unambiguous

![image](https://github.com/lolyu/aoi/assets/35479537/2cfcbc09-6f29-46e8-b937-c5bcc234ccf1)

### void pointer conversion
* A pointer to any object that isn't `const` or `volatile` can be implicitly converted to a pointer of type `void *`.
* A pointer to type `void` can be converted to a pointer to any other type, but only with an explicit cast.

### null pointer conversion
### pointer to classes
### pointer to function
### pointer to void
An integral constant expression that evaluates to zero, or such an expression cast to a pointer type, is converted to a pointer called the null pointer.
```cpp
bool func(void *p)
{
    return p == nullptr;
}

func(0); // valid
```

### pointer expression conversion
* Any expression with an array type can be converted to a pointer of the same type. The result of the conversion is a pointer to the first array element. The following example demonstrates such a conversion

### reference conversion
* A reference to a class can be converted to a reference to a base class in these cases:
    * the specified base class is accessible.
    * the conversion is unambiguous

### pointer to class member conversion
A pointer to a member of a base class can be converted to a pointer to a member of a class derived from it when the following conditions are met:
    * The inverse conversion, from pointer to derived class to base-class pointer, is accessible.
    * The derived class doesn't inherit virtually from the base class.

## references
* https://en.cppreference.com/w/cpp/language/usual_arithmetic_conversions

