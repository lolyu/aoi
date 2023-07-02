#include <iostream>
#include <typeinfo>

using namespace std;

// ./a.out | c++filt -t

int main()
{
    auto s0 = "helloworld"; // narrow string literal, no prefix, double quoted, null terminated
    auto s1 = u8"helloworld";
    auto s2 = L"helloworld";
    auto s3 = u"helloworld";
    auto s4 = U"helloworld";
    auto s5 = R"abc(Hello"\()abc";
    auto s6 = R"(hello\"world\")"; // raw string, escaped characters are not processed

    auto ss0 = "helloworld"s;
    auto ss1 = u8"helloworld"s;
    auto ss2 = L"helloworld"s;
    auto ss3 = u"helloworld"s;
    auto ss4 = U"helloworld"s;
    auto ss5 = R"abc(Hello"\()abc"s;
    auto ss6 = R"(hello\"world\")"s;

    cout << "int literal s0, type " << typeid(s0).name() << ", size " << sizeof(s0) << "." << endl;
    cout << s0 << endl;
    cout << "int literal s1, type " << typeid(s1).name() << ", size " << sizeof(s1) << "." << endl;
    cout << s1 << endl;
    cout << "int literal s2, type " << typeid(s2).name() << ", size " << sizeof(s2) << "." << endl;
    cout << s2 << endl;
    cout << "int literal s3, type " << typeid(s2).name() << ", size " << sizeof(s3) << "." << endl;
    cout << s3 << endl;
    cout << "int literal s4, type " << typeid(s4).name() << ", size " << sizeof(s4) << "." << endl;
    cout << s4 << endl;
    cout << "int literal s5, type " << typeid(s5).name() << ", size " << sizeof(s5) << "." << endl;
    cout << s5 << endl;
    cout << "int literal s6, type " << typeid(s6).name() << ", size " << sizeof(s6) << "." << endl;
    cout << s6 << endl;

    cout << "int literal ss0, type " << typeid(ss0).name() << ", size " << sizeof(ss0) << "." << endl;
    cout << "int literal ss1, type " << typeid(ss1).name() << ", size " << sizeof(ss1) << "." << endl;
    cout << "int literal ss2, type " << typeid(ss2).name() << ", size " << sizeof(ss2) << "." << endl;
    cout << "int literal ss3, type " << typeid(ss3).name() << ", size " << sizeof(ss3) << "." << endl;
    cout << "int literal ss4, type " << typeid(ss4).name() << ", size " << sizeof(ss4) << "." << endl;
    cout << "int literal ss5, type " << typeid(ss5).name() << ", size " << sizeof(ss5) << "." << endl;
    cout << "int literal ss6, type " << typeid(ss6).name() << ", size " << sizeof(ss6) << "." << endl;
    return 0;
}
