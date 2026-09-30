#include "../h/syscall_c.h"

extern "C" char __getc();
extern "C" void __putc(char);

static unsigned long syscall(unsigned long code, unsigned long arg){
    register unsigned long a0 asm("a0") = code;
    register unsigned long a1 asm("a1") = arg;

    asm volatile(
            "ecall"
            : "+r"(a0)
            :"r"(a1)
            : "memory"
            );

    return a0;
}

static unsigned long syscall4(unsigned long code, unsigned long arg1, unsigned long arg2, unsigned long arg3, unsigned long arg4){
    register unsigned long a0 asm("a0") = code;
    register unsigned long a1 asm("a1") = arg1;
    register unsigned long a2 asm("a2") = arg2;
    register unsigned long a3 asm("a3") = arg3;
    register unsigned long a4 asm("a4") = arg4;

    asm volatile(
    "ecall"
    : "+r"(a0)
    : "r"(a1), "r"(a2), "r"(a3), "r"(a4)
    : "memory"
    );

    return a0;

}

void* mem_alloc(size_t size){
    if(size == 0) return nullptr;
    size_t blocks = (size + MEM_BLOCK_SIZE -1) / MEM_BLOCK_SIZE;
    unsigned long result = syscall(0x01, blocks);
    return reinterpret_cast<void*>(result);
}

int mem_free(void* ptr){
    unsigned long result = syscall(0x02, reinterpret_cast<unsigned long>(ptr));
    return static_cast<int>(result);
}

void thread_dispatch(){
    syscall(0x13, 0);
}

int thread_create(thread_t* handle, void (*start_routine)(void*), void* arg){
    if(handle == nullptr || start_routine == nullptr) return -1;
    void* stack = mem_alloc(DEFAULT_STACK_SIZE);
    if(stack == nullptr) return -1;

    void* stackTop = reinterpret_cast<char*>(stack) + DEFAULT_STACK_SIZE;

    unsigned long result = syscall4(0x11, reinterpret_cast<unsigned long>(handle), reinterpret_cast<unsigned long>(start_routine), reinterpret_cast<unsigned long>(arg),reinterpret_cast<unsigned long>(stackTop) );
    if(static_cast<long>(result)<0) mem_free(stack);

    return static_cast<int>(result);
}

int thread_exit(){
    return static_cast<int>(syscall(0x12,0));
}

char getc(){
    return __getc();
}
void putc(char c){
    __putc(c);
}

static unsigned long syscall2(unsigned long code, unsigned long arg1, unsigned long arg2){
    register unsigned long a0 asm("a0") = code;
    register unsigned long a1 asm("a1") = arg1;
    register unsigned long a2 asm("a2") = arg2;

    asm volatile(
    "ecall"
    : "+r"(a0)
    : "r"(a1), "r"(a2)
    : "memory"
    );

    return a0;
}
int sem_open(sem_t* handle, unsigned init){
    if(handle == nullptr) return -1;
    return static_cast<int>(syscall2(0x21, reinterpret_cast<unsigned long>(handle), static_cast<unsigned long>(init)));
}

int sem_wait(sem_t id){
    if(id == nullptr) return -1;
    return static_cast<int>(syscall(0x23, reinterpret_cast<unsigned long>(id)));
}

int sem_signal(sem_t id){
    if(id == nullptr) return -1;
    return static_cast<int>(syscall(0x24, reinterpret_cast<unsigned long>(id)));
}

int sem_close(sem_t handle){
    if(handle == nullptr) return -1;
    return static_cast<int>(syscall(0x22, reinterpret_cast<unsigned long>(handle)));
}

int time_sleep(time_t time){
    return static_cast<int>(syscall(0x31, static_cast<unsigned long>(time)));
}
