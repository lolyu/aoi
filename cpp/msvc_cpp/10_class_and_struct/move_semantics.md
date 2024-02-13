# move semantics
* the move constructor/assignment takes resources owned by the parameter.
    * the move constructor/assigment should release the data pointers in the source object so the destructor will not free the memory multiple times.
* the move constructor/assignment should do the following:
    * destroy self visible resources
        * delete any local pointers
    * move assign/init all bases and members
        * move the data from other to self
    * if the move assign/init cannot make the other resource-less, make it so
        * assign pointers in other object to `nullptr`

```cpp
class MemoryBlock
{
public:
    explicit MemoryBlock(size_t length) : _length(length), _data(new int[length])
    {
        std::cout << "MemoryBlock(size_t). length = " << _length << "." << std::endl;
    }

    ~MemoryBlock()
    {
        std::cout << "~MemoryBlock(). length = " << _length << "." << std::endl;

        if (_data != nullptr)
        {
            delete[] _data;
        }
    }

    MemoryBlock(const MemoryBlock &other) : MemoryBlock(other._length)
    {
        std::cout << "MemoryBlock(const MemoryBlock &). length = " << _length << ". Copying resource" << std::endl;

        std::copy(other._data, other._data + other._length, _data);
    }

    MemoryBlock(MemoryBlock &&other) noexcept : _length(other._length), _data(other._data)        // [1]
    {
        std::cout << "MemoryBlock(MemoryBlock &&). length = " << _length << ". Moving resource" << std::endl;

        other._data = nullptr;
        other._length = 0;
    }

    void swap(MemoryBlock &other)
    {
        std::swap(_length, other._length);
        std::swap(_data, other._data);
    }

    MemoryBlock &operator=(const MemoryBlock &other)
    {
        std::cout << "MemoryBlock &operator=(const MemoryBlock &). length = " << _length << ". Copying resource" << std::endl;

        if (&other != this)
        {
            // copy and swap
            MemoryBlock temp(other);
            swap(temp);
            return *this;
        }
    }

    MemoryBlock &operator=(MemoryBlock &&other) noexcept                                            // [2]
    {
        std::cout << "MemoryBlock &operator=(MemoryBlock &&). length = " << _length << ". Moving resource" << std::endl;

        if (&other != this)
        {
            delete[] _data;

            _data = other._data;
            _length = other._length;

            other._data = nullptr;
            other._length = 0;
        }

        return *this;
    }

    size_t length() const
    {
        return _length;
    }

private:
    size_t _length;
    int *_data = nullptr;
};

```

## move semantics with STL containers
* as of C++ 11, STL containers like `vector` will use `std::move_if_noexcept` to decide to use move or copy constructor for the resize operations.
* user defined classes should define `noexcept` move constructor so containers like `vector` could move them instead of copying them during capacity resize.

## use swap to implement move semantics
```cpp
    void swap(MemoryBlock &other)
    {
        std::swap(_length, other._length);
        std::swap(_data, other._data);
    }

    MemoryBlock(MemoryBlock &&other) noexcept                        // [3]
    {
        swap(other);
    }

    MemoryBlock &operator=(MemoryBlock &&other) noexcept             // [4]
    {
        swap(other);

        return *this;
    }
```
* **don't use swap to implement move semantics**:
   1. this implementation with `swap` doesn't make `other` resource-less
   2. the move constructor/assignment with `swap`([3] and [4]) is slower than the native implementation([1] and [2])
        * [3] and [4] takes 4 loads and 6 stores
        * [1] and [2] takes 2 loads and 4 stores

## references
* https://bajamircea.github.io/coding/cpp/2020/05/08/how-vector-copy-move.html
* http://howardhinnant.github.io/container_summary.html
* https://stackoverflow.com/questions/6687388/why-do-some-people-use-swap-for-move-assignments
