//
// Created by os on 8/17/26.
//

#ifndef PROJECT_BASE_RISCV_H
#define PROJECT_BASE_RISCV_H

class Riscv{
public:
    static unsigned long handleSyscall(unsigned long code, unsigned long arg1, unsigned long arg2, unsigned long arg3, unsigned long arg4);
};

#endif //PROJECT_BASE_RISCV_H
