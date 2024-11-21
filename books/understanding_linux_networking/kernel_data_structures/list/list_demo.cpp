#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include "list.h"

struct patent
{
    size_t id;
    struct list_head list;
};

struct person
{
    char name[10];
    size_t age;
    struct list_head list;
    struct list_head patent_list;
};

std::vector<std::string> names = {
    "Alice",
    "Bob",
    "Cat",
    "David"
};

int main()
{
    LIST_HEAD(employees);

    size_t age = 30;
    size_t patent_id = 10010;
    struct person *person_ptr, *person_next_ptr;
    struct patent *patent_ptr, *patent_next_ptr;
    for (const auto &name : names)
    {
        person_ptr = static_cast<struct person *>(std::malloc(sizeof(struct person)));
        strcpy(person_ptr->name, name.c_str());
        person_ptr->age = age++;

        INIT_LIST_HEAD(&person_ptr->list);
        INIT_LIST_HEAD(&person_ptr->patent_list);
        list_add_tail(&person_ptr->list, &employees);

        for (int i = 0; i < 10; ++i, ++patent_id)
        {
            patent_ptr = static_cast<struct patent *>(std::malloc(sizeof(struct patent)));
            patent_ptr->id = patent_id;

            INIT_LIST_HEAD(&patent_ptr->list);
            list_add_tail(&patent_ptr->list, &person_ptr->patent_list);
        }
    }

    list_for_each_entry(person_ptr, &employees, list)
    {
        std::cout << "=================" << std::endl;
        std::cout << "Employee: " << person_ptr->name << ", age " << person_ptr->age << std::endl;
        std::cout << "Patents: ";
        list_for_each_entry(patent_ptr, &person_ptr->patent_list, list)
        {
            std::cout << patent_ptr->id;
            if (patent_ptr->list.next != &person_ptr->patent_list)
            {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }

    list_for_each_entry_safe(person_ptr, person_next_ptr, &employees, list)
    {
        std::cout << "Deleting " << person_ptr->name << std::endl;
        list_for_each_entry_safe(patent_ptr, patent_next_ptr, &person_ptr->patent_list, list)
        {
            std::cout << "Deleting " << patent_ptr->id << std::endl;
            list_del(&patent_ptr->list);
            std::free(patent_ptr);
        }

        list_del(&person_ptr->list);
        std::free(person_ptr);
    }

    return 0;
}
