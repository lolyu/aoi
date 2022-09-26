
## list_head vs hlist_head
* `list_head` is a typical doubly linked list
* `list_head` could be stored in user-defined structure with any name
* `list_head` is used to implement queues in the kernel
* with `list_head` structure, it is easy to iterate the list from the list head
* with `list_head` structure, it is easy to iterate the list reversely from the list head
* `hlist_head` is used to implement chaining hash tables in the kernel
    * the list won't be too long and there is no need to iterate the list reversely
* `hlist_head` is half size of `list_head` structure, thus saves memory compared to hash table with `list_head`


## references
* https://tupelo-shen.github.io/2020/02/29/Linux%E5%86%85%E6%A0%B810-list_head%E5%92%8Chlist_head%E7%9A%84%E7%90%86%E8%A7%A3/
