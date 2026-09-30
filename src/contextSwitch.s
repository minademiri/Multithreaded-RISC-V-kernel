# 1 "src/contextSwitch.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/riscv64-linux-gnu/include/stdc-predef.h" 1 3
# 32 "<command-line>" 2
# 1 "src/contextSwitch.S"
.section .text
.align 2
.global contextSwitch

contextSwitch:
    sd ra, 0(a0)
    sd sp, 8(a0)
    sd t1, 16(a0)

    ld ra, 0(a1)
    ld sp, 8(a1)
    ld t1, 16(a1)

    ret
