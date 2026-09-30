//
// Created by os on 8/17/26.
//

#ifndef PROJECT_BASE_SYSCALL_C_H
#define PROJECT_BASE_SYSCALL_C_H

#include "../lib/hw.h"

void* mem_alloc(size_t size);
int mem_free(void* ptr);

class _thread;
typedef _thread* thread_t;

int thread_create(
        thread_t* handle,
        void (*start_routine)(void*),
        void* arg
        );

int thread_exit();
void thread_dispatch();
int time_sleep(time_t);

char getc();
void putc(char c);

class _sem;
typedef _sem* sem_t;
int sem_open(sem_t* handle, unsigned init);
int sem_wait(sem_t id);
int sem_signal(sem_t id);
int sem_close(sem_t handle);


#endif //PROJECT_BASE_SYSCALL_C_H
