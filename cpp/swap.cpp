#include <iostream>
#include <mutex>
#include <memory>
#include <cstring>

struct Resource
{
    u_char data[8];
};

class DataManager
{
public:
    struct ResourceImpl
    {
        struct Resource mResource;
        size_t mResourceChanges;
    };

public:
    DataManager() : pImpl(std::make_unique<ResourceImpl>())
    {
        std::memset(reinterpret_cast<void *>(&pImpl->mResource), 0, sizeof(struct Resource));
    }
    ~DataManager() = default;

    DataManager(const DataManager &d)
    {
        pImpl.reset(new ResourceImpl);
        std::memcpy(&(pImpl->mResource), &(d.pImpl->mResource), sizeof(struct Resource));
        pImpl->mResourceChanges = 0;
    }

    DataManager &operator=(DataManager d)
    {
        // use member swap to implement copy assignment
        swap(d);
        return *this;
    }

    // member swap function
    void swap(DataManager &d)
    {
        std::cout << "DataManager::swap" << std::endl;
        pImpl.swap(d.pImpl);
    }

    bool storeData(const std::string &data)
    {
        if (data.size() >= 8)
        {
            return false;
        }
        strcpy(reinterpret_cast<char *>(&(pImpl->mResource.data)), data.c_str());
        pImpl->mResourceChanges += 1;
        return true;
    }

    void printData() const
    {
        std::cout << pImpl->mResource.data << std::endl;
    }

    size_t getResourceChanges() const
    {
        return pImpl->mResourceChanges;
    }

private:
    mutable std::mutex mMutex;
    std::unique_ptr<ResourceImpl> pImpl;
};

namespace std
{
    // provide a std::swap specialization for DataManager
    template <>
    void swap<DataManager>(DataManager &lhs, DataManager &rhs)
    {
        lhs.swap(rhs);
    }
};

void exchange(DataManager &lhs, DataManager &rhs)
{
    // 1. argument dependent lookup to find swap in the namespace that DataManager locates
    // 2. use std::swap the DataManager specialization
    // 3. use std::swap<T>
    using std::swap;
    swap(lhs, rhs);
}

int main()
{
    auto dm = DataManager();
    dm.printData();
    std::cout << dm.getResourceChanges() << std::endl;

    dm.storeData("hello");
    dm.printData();
    std::cout << dm.getResourceChanges() << std::endl;

    dm.storeData("aloha");
    dm.printData();
    std::cout << dm.getResourceChanges() << std::endl;

    dm.storeData("helloworld");
    dm.printData();
    std::cout << dm.getResourceChanges() << std::endl;

    auto dm1 = DataManager();
    dm1.storeData("bye");
    dm = dm1;
    dm.printData();
    dm1.printData();

    auto dm2 = DataManager();
    dm2.storeData("aloha");
    dm1.printData();
    dm2.printData();
    exchange(dm1, dm2);
    dm1.printData();
    dm2.printData();
    return 0;
}
