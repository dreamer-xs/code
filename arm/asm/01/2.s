    .section .rodata
    .align 2    @2^2字节对齐
.LC0:
    .string "Hello world !"

.LC1:
    .string "%x\n"


    .section .text
    .align 2
    .global main
main:
    mov r10, lr

    mov r1, #0xff
    ldr r0, =.LC1
    bl printf

    mov r1, #0xff
    mvn r2, #(1<<3)

    and r1, r1, r2

    ldr r0, =.LC1
    bl printf

    mov pc, r10

