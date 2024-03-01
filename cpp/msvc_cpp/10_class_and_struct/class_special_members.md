# C++ class special members
* special members are those that compiler can be asked to automatically generated code for:
    * default constructor
    * destructor
    * copy constructor && copy assignment operator
    * move constructor && move assignment operator

* special member functions can be explicitly defaulted with `= default`
* special member functions can be explicitly deleted with `= deleted`

* rule to generate default special members:
![image](https://github.com/lolyu/aoi/assets/35479537/d8215336-5b3d-471c-9db9-c03d21d3c6ee)



## default constructor
* if no other constructors that take parameters are declared, a default constructor is provided
    * the default constructor does member-wise default initialization

## destructor
* If no user-declared prospective(since C++20) destructor is provided for a class type, the compiler will always declare a destructor as an inline public member of its class.

## copy constructor and copy assignment operator

## move constructor and move assignment operator


## references
* https://cplusplus.com/doc/tutorial/classes2/
* https://safecpp.com/RuleOfDesDeMovA.html
* https://accu.org/conf-docs/PDFs_2014/Howard_Hinnant_Accu_2014.pdf
