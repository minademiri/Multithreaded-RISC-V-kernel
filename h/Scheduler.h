//
// Created by os on 8/17/26.
//

#ifndef PROJECT_BASE_SCHEDULER_H
#define PROJECT_BASE_SCHEDULER_H

#include "../h/TCB.h"
class Scheduler{
public:
    static void put(TCB* thread);
    static TCB* get();

private:
    static TCB* head;
    static TCB* tail;
};
#endif //PROJECT_BASE_SCHEDULER_H
