//
// Created by os on 8/17/26.
//

#ifndef PROJECT_BASE_TCB_H
#define PROJECT_BASE_TCB_H

class Scheduler;
class _sem;

class TCB{
public:
    using Body = void (*) (void*);
    struct Context{
        unsigned long ra;
        unsigned long sp;
        unsigned long t1;
    };
    TCB(Body body, void* arg, void* stack = nullptr);
    static TCB* createThread(Body body, void* arg, void* stack);

    static void dispatch();

    bool isFinished() const;
    void setFinished(bool val);

    bool isBlocked() const;
    void setBlocked(bool value);

    Body getBody() const;
    void* getArg() const;
    void* getStack() const;

    static TCB* running;
    static int exit();




private:
    Body body;
    void* arg;
    void* stack;
    bool finished;
    bool blocked;
    Context context;
    int semWaitResult;
    TCB* next;
    static void threadWrapper();


    friend class Scheduler;
    friend class _sem;

};

extern "C" void contextSwitch(TCB::Context* oldContext, TCB::Context* newContext);

#endif //PROJECT_BASE_TCB_H
