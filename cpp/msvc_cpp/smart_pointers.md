# smart pointer
* smart pointers are crucial to RAII(resource acquisition is initialization)
* the memory/resource is managed by the C++ scoping rules:
    * resource acquisition is made when the smart pointer is initialized
    * resource release is made when the smart pointer is out-of-scope and destructed

* C++ smart pointers:
    * `unique_ptr`: allows exactly one owner of the underlying pointer, the ownership can be transferred
    * `shared_ptr`: reference-counted
    * `weak_ptr`: provide access to an object owned by one or more `shared_ptr` instances, but not participated in reference counting.
        * when to use:
            * when you want to observe an object, but do not require it to remain alive.
            * break circular references between shared_ptr instances.


## unique_ptr
* **NOTE**: `unique_ptr` cannot be copied to another `unique_ptr`, passed by value to a function, `unique_ptr` can only be moved.
* `unique_ptr` is as efficient as raw pointer in C++.

![image](https://github.com/lolyu/aoi/assets/35479537/e0fa99f5-c9b0-417d-bed3-793132475006)


### unique_ptr vs make_unique

