#include <iostream>
#include <vector>

using namespace std;

/*
https://en.cppreference.com/w/cpp/iterator
Kinds of iterators:
    * input iterator
    * output iterator
    * forward iterator
    * bidirectional iterator
    * random access iterator

Those iterators are concepts, as long as a type satisfy the requirements, the type, which is a particular
implementation of a concept, is called a model

All of above iterators supports:
    * dereferencing (*)
    * comparision (=, !=)

Input iterator supports: (single pass, read only, increment)
    * dereferencing (*) - read only
    * comparison (=, !=)
    * self increment (++)

Output iterator supports: (single pass, write only, increment)
    * dereferencing (*) - write only
    * comparison  (=, !=)
    * self increment (++)

Forward iterator supports: (multiple pass, read-write, increment)
    * dereferencing (*) - read write
    * comparison  (=, !=)
    * self increment (++)

Bidirectional iterator supports: (multiple pass, read-write, increment/decrement)
    * dereferencing (*) - read write
    * comparison  (=, !=)
    * self increment (++)
    * self decrement (--)

Random access iterator supports: (multiple pass, read-write, increment/decrement, random access)
    * dereferencing (*) - read write
    * comparison  (=, !=)
    * self increment (++)
    * self decrement (--)
    * subscription ([])
    * add (+)
    * sub (-)
    * comparison (>, <, >=, <=)

*/

int main()
{
    vector<int> vi{0, 1, 2, 3, 4, 5, 6};
    vector<int>::iterator vit = vi.begin();
    cout << *(vit++) << endl;
    cout << vit[3] << endl;
    cout << *(vit + 4) << endl;
    cout << std::boolalpha;
    cout << (vit < vi.end()) << endl;
    cout << std::noboolalpha;
    return 0;
}
