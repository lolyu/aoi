#include "stdio.h"

struct list_head {
	struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

void __list_add(struct list_head *_new, struct list_head *_prev, struct list_head *_next);
void list_add(struct list_head *_new, struct list_head *_head);
void list_add_tail(struct list_head *_new, struct list_head *_head);
void list_del(struct list_head *p);
void list_empty(struct list_head *p);


int main()
{
    LIST_HEAD(lh);
    struct list_head l0;
    struct list_head l1;
    list_add(&l0, &lh);
    list_add_tail(&l1, &lh);
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
