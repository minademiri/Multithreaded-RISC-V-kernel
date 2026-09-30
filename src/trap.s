# 1 "src/trap.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/riscv64-linux-gnu/include/stdc-predef.h" 1 3
# 32 "<command-line>" 2
# 1 "src/trap.S"
.section .text
.align 2

.global supervisorTrap
.extern handleSupervisorTrap
.extern console_handler

supervisorTrap:
    addi sp, sp, -16
    sd t0, 0(sp)
    sd t1, 8(sp)

    csrr t0, scause

    bltz t0, interruptTrap

    li t1, 8
    beq t0, t1, syscallTrap

    li t1, 9
    beq t0, t1, syscallTrap

    j exceptionTrap


syscallTrap:
    ld t0, 0(sp)
    ld t1, 8(sp)
    addi sp, sp, 16

    addi sp, sp, -32

    sd ra, 24(sp)

    csrr t0, sepc
    addi t0, t0, 4
    sd t0, 16(sp)

    csrr t0, sstatus
    sd t0, 8(sp)

    call handleSupervisorTrap

    ld t0, 8(sp)
    csrw sstatus, t0

    ld t0, 16(sp)
    csrw sepc, t0

    ld ra, 24(sp)

    addi sp, sp, 32

    sret


interruptTrap:
    slli t1, t0, 1
    srli t1, t1, 1

    li t0, 1
    beq t1, t0, timerInterrupt

    li t0, 9
    beq t1, t0, consoleInterrupt

    ld t0, 0(sp)
    ld t1, 8(sp)
    addi sp, sp, 16

    sret


timerInterrupt:
    li t0, 2
    csrc sip, t0

    ld t0, 0(sp)
    ld t1, 8(sp)
    addi sp, sp, 16

    sret


consoleInterrupt:
    ld t0, 0(sp)
    ld t1, 8(sp)
    addi sp, sp, 16

    addi sp, sp, -128

    sd ra, 0(sp)

    sd t0, 8(sp)
    sd t1, 16(sp)
    sd t2, 24(sp)
    sd t3, 32(sp)
    sd t4, 40(sp)
    sd t5, 48(sp)
    sd t6, 56(sp)

    sd a0, 64(sp)
    sd a1, 72(sp)
    sd a2, 80(sp)
    sd a3, 88(sp)
    sd a4, 96(sp)
    sd a5, 104(sp)
    sd a6, 112(sp)
    sd a7, 120(sp)

    call console_handler

    ld ra, 0(sp)

    ld t0, 8(sp)
    ld t1, 16(sp)
    ld t2, 24(sp)
    ld t3, 32(sp)
    ld t4, 40(sp)
    ld t5, 48(sp)
    ld t6, 56(sp)

    ld a0, 64(sp)
    ld a1, 72(sp)
    ld a2, 80(sp)
    ld a3, 88(sp)
    ld a4, 96(sp)
    ld a5, 104(sp)
    ld a6, 112(sp)
    ld a7, 120(sp)

    addi sp, sp, 128

    sret


exceptionTrap:
    j exceptionTrap


.align 2
.global switchToUser

switchToUser:
    li t0, 0x100
    csrc sstatus, t0
    csrw sepc, ra
    sret
