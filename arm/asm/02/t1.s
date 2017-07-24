    .section .rodata
    .align 2
.LC0:
    .string "i > %d && i < %d\n"

.LC1:
    .string "i < %d || i > %d\n"

    .section .text
    .align 2
    .global main
main:
    push {lr}

    mov r1, #70

    subs r2, r1, #10
    blt  _p1

    subs r2, r1, #90
    bgt  _p1

    subs r2, r1, #20
    subgts r2, r1, #60
    blt  _p2

    subs r2, r1, #60
    subgts r2, r1, #90
    blt  _p3

    b out
_p1:
    ldr r0, =.LC1
    mov r1, #10
    mov r2, #90
    bl printf
    b out

_p2:
    ldr r0, =.LC0
    mov r1, #20
    mov r2, #60
    bl printf
    b out

_p3:
    ldr r0, =.LC0
    mov r1, #60
    mov r2, #90
    bl printf
    b out
out:
    mov r0, #0
    pop {pc}

