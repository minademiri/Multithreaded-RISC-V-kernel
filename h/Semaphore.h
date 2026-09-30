
#ifndef PROJECT_BASE_SEMAPHORE_H
#define PROJECT_BASE_SEMAPHORE_H

#include "../lib/hw.h"

class TCB;
class _sem{
public:
    static _sem* create(unsigned init);

    int wait();
    int signal();
    int close();

private:
    explicit _sem(unsigned init);
    int value;
    TCB* blockedHead;
    TCB* blockedTail;
};

#endif //PROJECT_BASE_SEMAPHORE_H
