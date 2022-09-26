#include <iostream>

// https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/list.h?h=v6.0-rc5

struct list_head {
	struct list_head *next, *prev;
};

#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

void __list_add(struct list_head *_new, struct list_head *_prev, struct list_head *_next);
void list_add(struct list_head *_new, struct list_head *_head);
void list_add_tail(struct list_head *_new, struct list_head *_head);
void __list_del(struct list_head *_prev, struct list_head *_next);
void list_del(struct list_head *_entry);
bool list_empty(struct list_head *_head);
bool list_is_head(const struct list_head *_list, const struct list_head *_head);

#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

#define list_first_entry(ptr, type, member) \
	list_entry((ptr)->next, type, member)

#define list_for_each(pos, head) \
    for (pos = (head)->next; !list_is_head(pos, (head)); pos = pos->next)

#define list_entry_is_head(pos, head, member)				\
	(&pos->member == (head))

#define list_next_entry(pos, member)                        \
    list_entry((pos)->member.next, typeof(*(pos)), member)


#define list_for_each_entry(pos, head, member)				\
	for (pos = list_first_entry(head, typeof(*pos), member);	\
	     !list_entry_is_head(pos, head, member);			\
	     pos = list_next_entry(pos, member))

struct people {
    struct list_head head;
    char name[10];
    int age;
};

int main()
{
    LIST_HEAD(lh);
    struct list_head l0;
    struct list_head l1;
    list_add(&l0, &lh);
    list_add_tail(&l1, &lh);

    struct people p0 = {
        {
            nullptr,
            nullptr
        },
        "Alice",
        30
    };

    struct list_head *ph = &(p0.head);
    auto c = container_of(ph, struct people, head);
    std::cout << c << std::endl;
    std::cout << &p0 << std::endl;

    return 0;
}

void __list_add(struct list_head *_new, struct list_head *_prev, struct list_head *_next)
{
    _prev->next = _new;
    _new->prev = _prev;
    _new->next = _next;
    _next->prev = _new;
}

void list_add(struct list_head *_new, struct list_head *_head)
{
    __list_add(_new, _head, _head->next);
}

void list_add_tail(struct list_head *_new, struct list_head *_head)
{
    __list_add(_new, _head->prev, _head);
}

void __list_del(struct list_head *_prev, struct list_head *_next)
{
    _prev->next = _next;
    _next->prev = _prev;
}

void list_del(struct list_head *_entry)
{
    __list_del(_entry->prev, _entry->next);
}

bool list_empty(struct list_head *_head)
{
    return _head->next == _head;
}

bool list_is_head(const struct list_head *_list, const struct list_head *_head)
{
    return _list == _head;
}
