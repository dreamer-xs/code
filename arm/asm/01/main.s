    .section .rodata
    .align 2    @2^2字节对齐
.LC0:
    .string "Hello world !"

.LC1:
    .string "sum = %d\n"


    .section .text
    .align 2
    .global main
main:
    push {r4,lr}

    ldr r0, =.LC0
    bl puts

    mov r4, #0
    bl add_

    mov lr, r10
    mov pc, lr

add_:
    mov r9, lr

loop:
    mov r0, #1
    bl sleep
    
    mov r1, r4
    ldr r0, =.LC1
    bl printf

    adds r4, r4, #1
    cmp r4, #5

    bne loop


    pop {r4,lr}
    mov pc, lr

