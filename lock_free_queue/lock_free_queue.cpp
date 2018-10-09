//Implementation of non-blocking concurrent queue

#include "lock_free_queue.h"

lock_free_queue::lock_free_queue() {
    shared_ptr <node> dummy(new node(0));
    dummy->next = nullptr;
    Head = Tail = dummy;
}

lock_free_queue::~lock_free_queue() {
    while (Head != Tail)
        dequeue();
}

void lock_free_queue::enqueue(int value) {
    shared_ptr <node> addNode(new node(value));
    shared_ptr <node> tailCopy;
    while (true) {
        tailCopy = Tail;
        auto next = tailCopy->next;
        if (tailCopy == Tail) {
            if (next == nullptr) {
                if (atomic_compare_exchange_weak_explicit(&tailCopy->next, &next, addNode, memory_order_release,
                                                          memory_order_relaxed))
                    break;
            } else {
                atomic_compare_exchange_weak_explicit(&Tail, &tailCopy, next, memory_order_release,
                                                      memory_order_relaxed);
            }
        }
    }
    atomic_compare_exchange_weak_explicit(&Tail, &tailCopy, addNode, memory_order_release, memory_order_relaxed);
}

int lock_free_queue::dequeue() {
    shared_ptr <node> headCopy = Head;
    int returningValue;
    while (true) {
        auto headCopy = Head;
        auto tailCopy = Tail;
        auto next = headCopy->next;
        if (headCopy == Head) {
            if (headCopy == tailCopy) {
                if (next == nullptr)
                    return INFINITY;
                atomic_compare_exchange_weak_explicit(&Tail, &tailCopy, next, memory_order_release,
                                                      memory_order_relaxed);
            } else {
                returningValue = next->value;
                if (atomic_compare_exchange_weak_explicit(&Head, &headCopy, next, memory_order_release,
                                                          memory_order_relaxed))
                    break;
            }
        }
    }
    return returningValue;
}
