    .section .rodata
    .align 2
.LC0:
    .string "%d\n"

    .section .text
    .align 2
    .global main
main:
    push {lr}

    mov r0, #0   @ 0 NULL 当前时间
    bl time
    bl srandom

    mov r4, #0
loop:


    bl random
    and r1, r0, #1
    cmp r1, #1
    blt loop

    add r4, r4, #1
    cmp r4,#100
    blgt out

    mov r1, r0
    ldr r0, =.LC0
    bl printf
    b loop

out:
    mov r0, #0
    pop {pc}
