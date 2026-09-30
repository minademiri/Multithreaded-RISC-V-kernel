#include "../h/MemoryAllocator.h"
MemoryAllocator::FreeBlock* MemoryAllocator::freeHead = nullptr;
void MemoryAllocator::init() {
    freeHead = reinterpret_cast<FreeBlock*>(const_cast<void*>(HEAP_START_ADDR));
    size_t heapSize = reinterpret_cast<char*>(const_cast<void*>(HEAP_END_ADDR)) -
            reinterpret_cast<char*>(const_cast<void*>(HEAP_START_ADDR));

    freeHead->size = heapSize / MEM_BLOCK_SIZE;
    freeHead->next = nullptr;
    freeHead->prev = nullptr;

}

void* MemoryAllocator::alloc(size_t blocks) {
    if(blocks == 0) return nullptr;
    size_t reqBlocks = blocks + 1;
    //first fit
    FreeBlock* cur = freeHead;
    while(cur != nullptr && cur->size < reqBlocks){
        cur = cur->next;
    }
    //no mem

    if(cur == nullptr) return nullptr;
    size_t remainingBlocks = cur->size - reqBlocks;

    //remainder into free seg

    if(remainingBlocks>0){
        FreeBlock* newFree = reinterpret_cast<FreeBlock*>(
                reinterpret_cast<char*>(cur)
                + reqBlocks * MEM_BLOCK_SIZE
        );

        newFree->size = remainingBlocks;
        newFree->prev = cur->prev;
        newFree->next = cur->next;

        if(cur->prev != nullptr) cur->prev->next = newFree;
        else freeHead = newFree;

        if(cur->next != nullptr) cur->next->prev = newFree;

        cur->size = reqBlocks;
    }
    else{
        if(cur->prev != nullptr) cur->prev->next = cur->next;
        else freeHead = cur->next;

        if(cur->next != nullptr) cur->next->prev = cur->prev;
    }

    cur->next = nullptr;
    cur->prev = nullptr;

    return reinterpret_cast<char*>(cur) + MEM_BLOCK_SIZE;

}

int MemoryAllocator::free(void *ptr) {
    if(ptr == nullptr) return -1;

    FreeBlock* block = reinterpret_cast<FreeBlock*>(reinterpret_cast<char*>(ptr) - MEM_BLOCK_SIZE);

    if(reinterpret_cast<char*>(block) < reinterpret_cast<char*>(const_cast<void*>(HEAP_START_ADDR))
        || reinterpret_cast<char*>(block) >= reinterpret_cast<char*>(const_cast<void*>(HEAP_END_ADDR)) ) return -1;

    FreeBlock* cur = freeHead;
    FreeBlock* previous = nullptr;

    while(cur != nullptr && reinterpret_cast<unsigned long>(cur) < reinterpret_cast<unsigned long>(block)){
        previous = cur;
        cur = cur->next;
    }

    block->prev = previous;
    block->next = cur;

    if(previous != nullptr) previous->next = block;
    else freeHead = block;

    if(cur != nullptr) cur->prev = block;

    //joining prev

    if(block->prev != nullptr){
        FreeBlock* previousBlock = block->prev;
        char* prevEnd = reinterpret_cast<char*>(previousBlock) + previousBlock->size * MEM_BLOCK_SIZE;

        if(prevEnd == reinterpret_cast<char*>(block)){
            previousBlock->size += block->size;
            previousBlock->next = block->next;

            if(block->next != nullptr){
                block->next->prev = previousBlock;
            }

            block = previousBlock;
        }
    }

    //joining next

    if(block->next != nullptr) {
        FreeBlock* nextBlock = block->next;
        char* blockEnd = reinterpret_cast<char*>(block) + block->size * MEM_BLOCK_SIZE;

        if(blockEnd == reinterpret_cast<char*>(nextBlock)){
            block->size += nextBlock->size;
            block->next = nextBlock->next;

            if(nextBlock->next != nullptr) nextBlock->next->prev = block;
        }
    }

    return 0;

}


