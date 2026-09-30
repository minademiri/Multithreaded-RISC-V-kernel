//
// Created by os on 8/17/26.
//

#ifndef PROJECT_BASE_MEMORYALLOCATOR_H
#define PROJECT_BASE_MEMORYALLOCATOR_H

#include "../lib/hw.h"

class MemoryAllocator {
private:
    struct FreeBlock{
        size_t size;
        FreeBlock* next;
        FreeBlock* prev;
    };
    static FreeBlock* freeHead;
public:
    static void init();
    static void* alloc(size_t blocks);
    static int free(void* ptr);
};
#endif //PROJECT_BASE_MEMORYALLOCATOR_H
