# move semantics
* the move constructor/assignment takes resources owned by the parameter.
    * the move constructor/assigment should release the data pointers in the source object so the destructor will not free the memory multiple times.

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

    MemoryBlock(MemoryBlock &&other) noexcept : _length(other._length), _data(other._data)
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

    MemoryBlock &operator=(MemoryBlock &&other) noexcept
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

## references
* https://bajamircea.github.io/coding/cpp/2020/05/08/how-vector-copy-move.html
* http://howardhinnant.github.io/container_summary.html
