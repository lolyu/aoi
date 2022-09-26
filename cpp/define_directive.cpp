#include <iostream>

using namespace std;

/**
1. object-like macros
#define identifier token-string
2. function-like macros
#define identifier(identifier, identifier, ..., identifier) token-string

* define directive causes the compiler to substitute token-string for each occurrence of identifier
* one or more whitespaces must separate token-string from identifier
* whitespaces before/after token-string are not considered part of the substituted text
* for the second syntax, the formal parameters in the list are separated by commas.
    * no space between the identifier and the left parenthesis of the parameter list
    * each parameter name must be unique, and the list mux be enclosed in parentheses
 */

#define LOOPCOUNT 10
#define getrandom(min, max) ((rand() % (int)(((max) + 1) - (min))) + (min))
#define SUM(a, b, c) (a) + (b) + (c)
#define BETTER_SUM(a, b, c) ((a) + (b) + (c))
#define container_of(ptr, type, member) ({               \
    const typeof(((type *)nullptr)->member) *mptr = ptr; \
    (type *)((char *)mptr - offsetof(type, member));     \
})

int main()
{
    for (int i = 0; i < 20; ++i)
    {
        cout << getrandom(10, 20) << endl;
    }
    cout << SUM(1, 2, 3) * SUM(1, 2, 3) << endl;
    cout << BETTER_SUM(1, 2, 3) * BETTER_SUM(1, 2, 3) << endl;

    struct person
    {
        char name[20];
        int age;
        int gender;
    };

    struct person p0
    {
        "Alice",
            20,
            0
    };

    auto agep = &(p0.age);
    cout << &p0 << endl;
    cout << container_of(agep, struct person, age) << endl;

    return 0;
}
