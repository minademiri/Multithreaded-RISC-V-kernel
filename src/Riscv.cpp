#include "../h/MemoryAllocator.h"
#include "../h/Riscv.h"
#include "../h/TCB.h"
#include "../h/Semaphore.h"

unsigned long Riscv::handleSyscall(unsigned long code, unsigned long arg1, unsigned long arg2, unsigned long arg3, unsigned long arg4) {
    switch (code) {
        case 0x01: //mem alloc
        {
            return reinterpret_cast<unsigned long>(MemoryAllocator::alloc(arg1));
        }
        case 0x02: //mem free
        {
            return static_cast<unsigned long>(MemoryAllocator::free(reinterpret_cast<void *>(arg1)));
        }
        case 0x11: {
            //arg1 = thread_t* handle, arg2 = start_routine, arg3 = arg, arg4 = stack top
            TCB *thread = TCB::createThread(reinterpret_cast<TCB::Body>(arg2), reinterpret_cast<void *>(arg3),
                                            reinterpret_cast<void *>(arg4));
            if (thread == nullptr) return static_cast<unsigned long>(-1);
            TCB **handle = reinterpret_cast<TCB **>(arg1);
            *handle = thread;
            return 0;
        }
        case 0x12: // thread exit
        {
            return static_cast<unsigned long>(TCB::exit());
        }
        case 0x13: // thread dispatch
        {
            TCB::dispatch();
            return 0;
        }
        case 0x21:
        {
            if(arg1 == 0) return static_cast<unsigned long>(-1);
            _sem* sem = _sem::create(static_cast<unsigned>(arg2));
            if(sem == nullptr) return static_cast<unsigned long>(-1);
            _sem** handle = reinterpret_cast<_sem**>(arg1);
            *handle = sem;
            return 0;
        }
        case 0x22:
        {
            _sem* sem = reinterpret_cast<_sem*>(arg1);

            if(sem == nullptr) return static_cast<unsigned long>(-1);
            return static_cast<unsigned long>(sem->close());
        }

        case 0x23:
        {
            _sem* sem = reinterpret_cast<_sem*>(arg1);

            if(sem == nullptr) return static_cast<unsigned long>(-1);
            return static_cast<unsigned long>(sem->wait());
        }
        case 0x24:
        {
            _sem* sem = reinterpret_cast<_sem*>(arg1);

            if(sem == nullptr) return static_cast<unsigned long>(-1);
            return static_cast<unsigned long>(sem->signal());
        }
        default: {
            return static_cast<unsigned long>(-1);
        }

    }
}

extern "C" unsigned long handleSupervisorTrap(unsigned long code, unsigned long arg1, unsigned long arg2, unsigned long arg3, unsigned long arg4){
    return Riscv::handleSyscall(code, arg1, arg2, arg3, arg4);
}
