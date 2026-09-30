#include "../h/MemoryAllocator.h"
#include "../h/TCB.h"

extern "C" void supervisorTrap();
extern void userMain();

void main()
{
    MemoryAllocator::init();

    TCB mainThread(nullptr, nullptr, nullptr);
    TCB::running = &mainThread;

    unsigned long trapAddress =
            reinterpret_cast<unsigned long>(&supervisorTrap);

    asm volatile(
    "csrw stvec, %0"
    :
    : "r"(trapAddress)
    );

    userMain();

    while (1) {}
}