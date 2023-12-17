# forward_as_tuple and piecewise_construct

## `std::forward`
* if `arg` is an lvalue reference, return `arg` as it is
* if `arg` is not an lvalue reference, return an rvalue reference
* why we need `std::forward`?
    * it is used in template function to forward the argument to overloaded functions

```
#include <iostream>

void f(const int &i)
{
    std::cout << "lvalue reference" << std::endl;
}

void f(int &&i)
{
    std::cout << "rvalue reference" << std::endl;
}

template <typename T>
void call_f(T &&i)
{
    // T &&i in the template function is a forwarding reference
    // forwarding reference is a parameter of function template declared as rvalue reference to non-const type template parameter
    f(i);
    f(std::forward<T>(i));
}

/**
lvalue reference
lvalue reference
lvalue reference
rvalue reference
*/

int main()
{
    int i = 0;
    call_f(i);
    call_f(0);
    return 0;
}
```

## `std::forward_as_tuple`
* `std::forward_as_tuple` takes its arguments and produces a tuple of corresponding references

## `piecewise_construct`
* creates a pair, the pair first elemnt is construct in-place with the first tuple(`std::forward_as_tuple`) and the pair second element is constructed in-place with the second tuple(`std::forward_as_tuple`)

```cpp
#include <iostream>
#include <string>
#include <map>

class Person
{
public:
    enum Gender
    {
        Male,
        Female
    };

    Person(const std::string &name, int age, Gender gender) : name(name), age(age), gender(gender) {}

    void show() const
    {
        std::cout << name << ", " << age << ", " << (gender == Male ? "Male" : "Female") << std::endl;
    }
private:
    std::string name;
    int age;
    Gender gender;
};

int main()
{
    std::map<std::string, Person> people;
    people.emplace(
        std::piecewise_construct,
        std::forward_as_tuple("Alice"),
        std::forward_as_tuple("Alice", 20, Person::Female)
    );
    people.emplace(
        std::piecewise_construct,
        std::forward_as_tuple("Bob"),
        std::forward_as_tuple("Bob", 21, Person::Male)
    );

    for (const auto &p : people)
    {
        p.second.show();
    }

    return 0;
}
```
