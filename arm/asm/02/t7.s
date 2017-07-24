@ 高从1 到 10的三角;      **********
@ ^    ^  ^
@ *
@ **
@ ***
@ ****        高为4 ;
@ ***
@ **
@ *
    .section .rodata
    .align 2
.LC0:
    .string "*"
.LC1:
    .string "\n"

    .section .text
    .align 2
    .global main
main:
    push {r4, r5, r6, lr}

    mov r6, #10

    mov r4, #1
loop1:
    cmp r4, r6
    bge out1

    mov r5, #1
    loop2:
        cmp r5, r4
        bgt loop1_end
        
        ldr r0, =.LC0
        bl printf

        add r5, #1
        b loop2

loop1_end:

    ldr r0, =.LC1
    bl printf

    add r4, #1
    b loop1
out1:

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    mov r4, r6
loop3:
    cmp r4, #1
    blt out2

    mov r5, #1
    loop4:
        cmp r5, r4
        bgt loop3_end
        
        ldr r0, =.LC0
        bl printf

        add r5, #1
        b loop4

loop3_end:

    ldr r0, =.LC1
    bl printf

    sub r4, #1
    b loop3
out2:

    mov r0, #0
    pop {r4, r5, r6, pc}
