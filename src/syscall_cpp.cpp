#include "../h/syscall_cpp.hpp"

Thread::Thread(void (*body)(void *), void *arg) : myHandle(nullptr), body(body), arg(arg){}

Thread::Thread():myHandle(nullptr), body(nullptr), arg(nullptr) {}

Thread::~Thread() noexcept {}

void Thread::threadWrapper(void *arg) {
    Thread* thread = static_cast<Thread*>(arg);

    if(thread != nullptr) thread->run();
}

int Thread::start() {
    if(myHandle != nullptr) return -1;
    if(body != nullptr) return thread_create(&myHandle, body, arg);
    return thread_create(&myHandle, threadWrapper, this);
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t time) {
    return time_sleep(time);
}

Semaphore::Semaphore(unsigned int init) : myHandle(nullptr) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    if(myHandle != nullptr) sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}

