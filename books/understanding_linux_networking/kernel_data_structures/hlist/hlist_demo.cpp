#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <cstring>
#include <tuple>
#include "list.h"

#define EHASH_SIZE 32;

struct employee
{
    char name[10];
    size_t age;
    size_t employee_id;
    struct hlist_node node;
};

struct ehash_info
{
    size_t ehash_size;
    struct hlist_head buckets[1];
};

size_t ehashfn(size_t employee_id, const size_t ehash_size)
{
    return employee_id & (ehash_size - 1);
}

struct ehash_info * alloc_ehash_info(const size_t ehash_size)
{
    size_t ehash_info_size = sizeof(struct ehash_info) + (ehash_size - 1) * sizeof(struct hlist_head);
    struct ehash_info *ehash_info_ptr = static_cast<struct ehash_info *>(std::malloc(ehash_info_size));
    ehash_info_ptr->ehash_size = ehash_size;
    for (size_t i = 0; i < ehash_size; ++i)
    {
        INIT_HLIST_HEAD(&ehash_info_ptr->buckets[i]);
    }
    return ehash_info_ptr;
}

void destroy_ehash_info(struct ehash_info *ehash_info_ptr)
{
    std::free(ehash_info_ptr);
}

void print_ehash_info(struct ehash_info *ehash_info_ptr)
{
    struct employee *eptr;
    struct hlist_node *node;
    for (size_t i = 0; i < ehash_info_ptr->ehash_size; ++i)
    {
        struct hlist_head *head = &ehash_info_ptr->buckets[i];
        std::cout << "bucket " << i << ": ";
        hlist_for_each_entry(eptr, node, head, node)
        {
            std::cout << eptr->name << " ";
        }
        std::cout << std::endl;
    }
}

std::vector<std::tuple<std::string, size_t, size_t>> employees =
{
    {"Alice", 20, 2000},
    {"Bob", 20, 2001},
    {"Justin", 31, 2002},
    {"Cat", 31, 2003},
    {"Selina", 31, 2004},
    {"Don", 31, 2005},
    {"Xavier", 31, 2006}
};

int main()
{
    struct ehash_info *ehash_info_ptr;
    struct employee *eptr;
    std::vector<struct employee *> employee_list;

    ehash_info_ptr = alloc_ehash_info(4);

    for (auto &employee : employees)
    {
        eptr = static_cast<struct employee *>(std::malloc(sizeof(struct employee)));

        strcpy(eptr->name, std::get<0>(employee).c_str());
        eptr->age = std::get<1>(employee);
        eptr->employee_id = std::get<2>(employee);
        INIT_HLIST_NODE(&eptr->node);

        struct hlist_head *head = &ehash_info_ptr->buckets[ehashfn(eptr->employee_id, ehash_info_ptr->ehash_size)];
        hlist_add_head(&eptr->node, head);

        employee_list.push_back(eptr);
    }

    std::cout << "The employee hash info:" << std::endl;
    print_ehash_info(ehash_info_ptr);

    while (!employee_list.empty())
    {
        eptr = employee_list.back();
        employee_list.pop_back();
        std::cout << "Remove " << eptr->name  << " from the employee hash info" << std::endl;
        hlist_del(&eptr->node);
        std::cout << "The employee hash info:" << std::endl;
        print_ehash_info(ehash_info_ptr);
        std::free(eptr);
    }

    destroy_ehash_info(ehash_info_ptr);
    return 0;
}
