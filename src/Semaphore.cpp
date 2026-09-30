#include "../h/Semaphore.h"
#include "../h/MemoryAllocator.h"
#include "../h/TCB.h"
#include "../h/Scheduler.h"

_sem::_sem(unsigned init): value(static_cast<int>(init)), blockedHead(nullptr), blockedTail(nullptr){}

_sem *_sem::create(unsigned init) {
    size_t blocks = (sizeof(_sem) + MEM_BLOCK_SIZE -1)/ MEM_BLOCK_SIZE;
    void* space = MemoryAllocator::alloc(blocks);
    if(space == nullptr) return nullptr;

    _sem* sem = reinterpret_cast<_sem*>(space);

    sem->value = static_cast<int>(init);
    sem->blockedHead = nullptr;
    sem->blockedTail = nullptr;

    return sem;
}

int _sem::wait() {
    if(TCB::running == nullptr) return -1;
    if(value > 0){
        value--;
        return 0;
    }

    TCB* thread = TCB::running;

    thread->blocked = true;
    thread->next = nullptr;

    if(blockedTail == nullptr) blockedHead = blockedTail = thread;
    else{
        blockedTail->next = thread;
        blockedTail = thread;
    }
    thread->semWaitResult = 0;
    TCB::dispatch();
    return TCB::running->semWaitResult;
}

int _sem::signal() {
    if(blockedHead != nullptr){
        TCB* thread = blockedHead;
        blockedHead = blockedHead->next;
        if(blockedHead == nullptr) blockedTail = nullptr;

        thread->next = nullptr;
        thread->blocked = false;
        thread->semWaitResult = 0;

        Scheduler::put(thread);
        return 0;
    }
    value++;
    return 0;
}

int _sem::close() {
    while(blockedHead != nullptr){
        TCB* thread = blockedHead;
        blockedHead = blockedHead->next;

        thread->next = nullptr;
        thread->blocked = false;

        thread->semWaitResult = -1;
        Scheduler::put(thread);
    }

    blockedTail = nullptr;
    return MemoryAllocator::free(this);
}
