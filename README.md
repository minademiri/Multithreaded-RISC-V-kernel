# Multithreaded RISC-V Kernel

A lightweight educational operating-system kernel for the RISC-V architecture, implemented in C++ and RISC-V assembly.

The project implements core operating-system mechanisms including dynamic memory management, threads, context switching, scheduling, synchronization, system calls, and user/kernel mode transitions.

## Features

- Custom dynamic memory allocator
    - First-fit allocation
    - Block splitting
    - Adjacent free-block coalescing
- Multithreading support
    - Thread creation and termination
    - Cooperative context switching
    - Thread Control Blocks (TCBs)
- FIFO thread scheduler
- Semaphore-based synchronization
    - `open`
    - `wait`
    - `signal`
    - `close`
- RISC-V system call interface
- C and C++ APIs for kernel services
- Trap and exception handling
- User-mode thread execution
- Context switching implemented in RISC-V assembly
- Custom `new` and `delete` operators backed by the kernel memory allocator

## Project Structure

```text
.
├── h/
│   ├── MemoryAllocator.h
│   ├── Riscv.h
│   ├── Scheduler.h
│   ├── Semaphore.h
│   ├── TCB.h
│   ├── syscall_c.h
│   └── syscall_cpp.hpp
│
├── src/
│   ├── MemoryAllocator.cpp
│   ├── MemoryOperators.cpp
│   ├── Riscv.cpp
│   ├── Scheduler.cpp
│   ├── Semaphore.cpp
│   ├── TCB.cpp
│   ├── contextSwitch.s
│   ├── trap.s
│   ├── main.cpp
│   ├── syscall_c.cpp
│   └── syscall_cpp.cpp
│
├── Makefile
└── kernel.ld
```

## Architecture

The kernel is organized into several layers:

1. **System-call API** – C and C++ interfaces available to user code.
2. **Kernel services** – memory management, threads, scheduling, and semaphores.
3. **RISC-V trap handling** – handles system calls, exceptions, and interrupts.
4. **Low-level context switching** – implemented directly in RISC-V assembly.

User threads execute in unprivileged mode and enter the kernel through system calls.

Thread scheduling is cooperative: a context switch occurs when a thread explicitly dispatches, blocks on synchronization, or terminates.

## Technologies

- C++
- RISC-V Assembly
- RISC-V GNU Toolchain
- QEMU
- GDB