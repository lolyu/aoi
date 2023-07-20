# declarations and definitions
* A `declaration` introduces an identifier and describes its type.
    * `declaration` doesn't allocate storage for it.
    * `declaration` is what the compiler needs to accept references to the identifier
    * **`declaration` tells the compiler to accept a name that is legal**

* A `definition` actually instantiates/implements the identifier
    * a `declaration` of built-in types is a definition because the compiler knows how much space to allocate for it.
    * **`definition` tells the linker to link a name reference to the content of the name**
    * `static` class variable/function is declared inside the class, but should be defined/initialized out of the class.
```cpp
// declare and define int variables i and j.
int i;
int j = 10;

// enum declaration, also a enum definition
enum suits { Spades = 1, Clubs, Hearts, Diamonds };

// class declaration, also a class definition
class CheckBox : public Control
{
public:
    Boolean IsChecked();
    virtual int     ChangeState() = 0;
};
```

## alias
* `typedef` and `using` statements could give alias to another name(should be defined)

## storage classes

## references
* https://stackoverflow.com/questions/1410563/what-is-the-difference-between-a-definition-and-a-declaration
