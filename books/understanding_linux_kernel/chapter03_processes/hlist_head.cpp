#include <iostream>

using namespace std;

/*
https://lwn.net/Articles/336255/

Why we need hlist_head?

In some use cases, finding the end of the list is not needed, and being able to halve the size of the head of the list is very valuable.
One typical use case of that kind is a hash table where all these heads need to go in an array.

*/

#define container_of(ptr, type, member) ({               \
    const typeof(((type *)nullptr)->member) *mptr = ptr; \
    (type *)((char *)mptr - offsetof(type, member));     \
})

#define pidhash_shift 11
#define pid_hashfn(x) hash_long((unsigned long)x, pidhash_shift)

struct hlist_head
{
    struct hlist_node *first;
};

// pprev stores the address of the next field of previous node
struct hlist_node
{
    struct hlist_node *next, **pprev;
};

struct process
{
    struct hlist_node node;
    unsigned int pid;
};

// add new node as the first element in the hash list
void hlist_add_head(struct hlist_node *n, struct hlist_head *h)
{
    struct hlist_node *first = h->first;
    n->next = first;
    if (first)
    {
        first->pprev = &n->next;
    }
    h->first = n;
    n->pprev = &h->first;
}

void hlist_del(struct hlist_node *n)
{
    struct hlist_node *next = n->next;
    struct hlist_node **pprev = n->pprev;

    *pprev = next;
    if (next)
    {
        next->pprev = pprev;
    }
}

struct hlist_head hash_table[2048];

unsigned long hash_long(unsigned int val, unsigned int bits)
{
    unsigned int hash = val * 0x9e370001UL;
    return hash >> (sizeof(hash) - bits);
}

void create_process(unsigned int pid)
{
    struct process *p = new struct process;
    p->pid = pid;

    unsigned int pid_hash = pid_hashfn(pid);
    hlist_add_head(&p->node, &hash_table[pid_hash]);
}

struct process *find_process(unsigned int pid)
{
    unsigned int pid_hash = pid_hashfn(pid);
    struct hlist_head *h = &hash_table[pid_hash];
    for (struct hlist_node *n = h->first; n != nullptr; n = n->next)
    {
        struct process *p = container_of(n, struct process, node);
        if (p->pid == pid)
        {
            return p;
        }
    }
    return nullptr;
}

void del_process(unsigned int pid)
{
    struct process *p = find_process(pid);
    if (p)
    {
        hlist_del(&p->node);
        delete p;
    }
}

void view_table()
{
    for (unsigned int i = 0; i < 2048; ++i)
    {
        struct hlist_head *h = &hash_table[i];
        if (h->first)
        {
            cout << i << ": ";
        }
        for (struct hlist_node *n = h->first; n != nullptr; n = n->next)
        {
            struct process *p = container_of(n, struct process, node);
            cout << p->pid << " ";
        }
        if (h->first)
        {
            cout << endl;
        }
    }
}

void free_table()
{
    for (unsigned int i = 0; i < 2048; ++i)
    {
        struct hlist_head *h = &hash_table[i];
        for (struct hlist_node *n = h->first, *nn = nullptr; n != nullptr; n = nn)
        {
            nn = n->next;
            struct process *p = container_of(n, struct process, node);
            hlist_del(&p->node);
            delete p;
        }
    }
}

int main()
{
    unsigned int pid;
    for (pid = 0; pid <= 30000; pid += (rand() % 500))
    {
        create_process(pid);
    }

    view_table();
    cout << endl;

    del_process(0);
    view_table();
    cout << endl;

    free_table();
    return 0;
}
