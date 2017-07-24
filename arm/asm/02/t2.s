    .section .rodata
    .align 2
.LC0:
    .string "%d\n"


    .section .text
    .align 2
    .global main
main:
    push {lr}
    mov r4, #0
loop:
    add r4, r4, #1
    cmp r4, #100
    bgt out

    and r6, r4, #1
    cmp r6, #1
    blge _p
    b loop
_p:
    ldr r0, =.LC0
    mov r1, r4
    bl printf
    b loop

 out:
     mov r0, #0
     pop {pc}


