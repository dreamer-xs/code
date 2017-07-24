    .section .rodata
    .align 2
.LC0:
    .string "%2d * %2d = %2d \n"

.LC1:
    .string "\n"

    .section .text
    .align 2
    .global main
main:
    push {lr}

    mov r4, #1
    mov r5, #1
    b loop1

loop2:
    cmp r5, r4
    bge loop1_end

    ldr r0, =.LC0
    mov r1, r5
    mov r2, r4
    mul r3, r4, r5
    bl printf

    add r5, r5, #1
    b loop2

loop1:
    cmp r4, #10
    bgt  out
    bl loop2
loo1_end

    ldr r0, =.LC0
    bl printf
    
    add r4, r4, #1
    b loop1

out:
    mov r0, #0
    pop {pc}
