//Non-blocking concurrent queue
#pragma once

#include <memory>

using namespace std;

class lock_free_queue {
public:
    struct node {
        int value;
        shared_ptr <node> next;

        node(int val)
                : value(val), next(nullptr) {}
    };

    lock_free_queue();

    ~lock_free_queue();

    void enqueue(int data);

    int dequeue();

private:
    shared_ptr <node> Head;
    shared_ptr <node> Tail;
};

