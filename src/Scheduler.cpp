#include "../h/Scheduler.h"

TCB* Scheduler::head = nullptr;
TCB* Scheduler::tail = nullptr;

void Scheduler::put(TCB *thread) {
    if(thread == nullptr) return;
    thread->next = nullptr;
    if(tail == nullptr) head = tail = thread;
    else{
        tail->next = thread;
        tail = thread;
    }
}
TCB* Scheduler::get() {
    if(head == nullptr) return nullptr;
    TCB* thread = head;
    head = head->next;

    if(head == nullptr) tail = nullptr;
    thread->next = nullptr;
    return thread;
}

