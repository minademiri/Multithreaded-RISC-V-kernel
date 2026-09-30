#include "../h/TCB.h"
#include "../h/Scheduler.h"
#include "../h/MemoryAllocator.h"
#include "../h/syscall_c.h"

extern "C" void switchToUser();

TCB* TCB::running = nullptr;

TCB::TCB(Body body, void *arg, void *stack): body(body), arg(arg), stack(stack), finished(false), blocked(false), context{
    body != nullptr ? reinterpret_cast<unsigned long>(&TCB::threadWrapper) : 0, stack != nullptr ? reinterpret_cast<unsigned long>(stack) : 0, 0
}, semWaitResult(0), next(nullptr){}

bool TCB::isFinished() const { return finished; }
void TCB::setFinished(bool val) { finished = val; }

bool TCB::isBlocked() const { return blocked; }
void TCB::setBlocked(bool value) { blocked = value; }

TCB::Body TCB::getBody() const { return body; }

void *TCB::getArg() const { return arg; }

void *TCB::getStack() const { return stack; }

void TCB::dispatch() {
    TCB* old = running;
    if(old!= nullptr && !old->isFinished() && !old->isBlocked()) Scheduler::put(old);

    running = Scheduler::get();
    if(running == nullptr){
        running = old;
        return;
    }

    if(running == old) return;

    contextSwitch(&old->context, &running->context);

}

void TCB::threadWrapper(){
    switchToUser();

    running->body(running->arg);
    thread_exit();

    while (true) {}
}

TCB *TCB::createThread(Body body, void *arg, void *stack) {
    size_t blocks = (sizeof(TCB) + MEM_BLOCK_SIZE -1 ) / MEM_BLOCK_SIZE;
    void* space = MemoryAllocator::alloc(blocks);
    if(space == nullptr) return nullptr;
    TCB* thread = static_cast<TCB*>(space);

    thread->body = body;
    thread->arg = arg;
    thread->stack = stack;

    thread->context.ra = body != nullptr ? reinterpret_cast<unsigned long>(&TCB::threadWrapper) : 0;
    thread->context.sp = stack != nullptr ? reinterpret_cast<unsigned long>(stack) : 0;
    thread->context.t1 = 0;

    thread->setFinished(false);
    thread->setBlocked(false);
    thread->semWaitResult = 0;
    thread->next = nullptr;

    if(body != nullptr) Scheduler::put(thread);
    return thread;
}

int TCB::exit() {
    if(running == nullptr) return -1;
    running->setFinished(true);
    dispatch();
    return 0;
}



