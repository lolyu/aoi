#include <iostream>
#include "string.h"

// const int String::demoConst0 = 100;          this will break ODR
const int ArSize = 10;
const int MaxLen = 81;

String callMe(String s)
{
    return s;
}

int main()
{
    String name0 = "helloworld";                           // custom constructor
    String name1;                                          // default constructor
    name1 = "helloworld";                                  // assignment, also a temporary String object created
    String name2 = name1;                                  // copy constructor
    callMe(name2);                                         // copy constructor is used both for argument passing and function return
    std::cout << (name2 < "helloworld2") << std::endl;     // constructor with one argument is used to implicit convert the argument type to the class type
    std::cout << static_cast<charp_t>(name2) << std::endl; // conversion operator
    // std::cout << "Hi, what's your name?\n>> ";
    // std::cin >> name;

    // std::cout << name << ", please enter up to " << ArSize << " short says <empty line to quit>:\n";
    // String sayings[ArSize];
    return 0;
}