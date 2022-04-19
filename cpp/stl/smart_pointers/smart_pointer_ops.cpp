#include <iostream>
#include <memory>
#include <string>

/*
shared_ptr vs unique_ptr
shared_ptr: reference counting
unique_ptr: ownership
*/

using namespace std;

unique_ptr<string> create_string(const char *s)
{
    return unique_ptr<string>(new string(s));
}

int main()
{
    // NOTE: only allows unique_ptr assignment/copy constructor only if the source unique_ptr is a temporary rvalue
    unique_ptr<string> up0(new string("automatic"));
    // unique_ptr<string> p1 = p0;
    unique_ptr<string> up1 = create_string("beautiful world");
    // unique_ptr<string> p2(p1);
    unique_ptr<string> up2(create_string("first love"));
    cout << *up0 << endl
         << *up1 << endl
         << *up2 << endl;

    // NOTE: unique_ptr allows space allocated by new[]
    unique_ptr<double[]> up3(new double[4]);
    up3[0] = 100.0;
    up3[1] = 200.0;
    up3[2] = 300.0;
    up3[3] = 400.0;
    for (size_t i = 0; i < 4; ++i) {
        cout << up3[i] << endl;
    }

    // NOTE: only allows unique_ptr assignment/copy constructor only if the source unique_ptr is a temporary rvalue
    // shared_ptr<string> sp0 = up1;
    shared_ptr<string> sp0 = create_string("love can");
    // shared_ptr<string> sp1(up0);
    shared_ptr<string> sp1(create_string("all the good things"));
    return 0;
}