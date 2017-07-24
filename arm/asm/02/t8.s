@ 半径从1 到 10的菱形;
@ 
@        *
@       ***
@      *****
@     *******    半径为4 ;
@      ***** 
@       ***
@        *
    .section .rodata
    .align 2
.LC0:
    .string " "
.LC1:
    .string "*"
.LC2:
    .string "\n"

    .section .text
    .align 2
    .global main
main:
    push {r4, r5, r6, r7, r8, lr}

    @r->r6
    @j->r4
    @i->r5

    mov r6, #3
    mov r4, #1
for:
    mov r7, r6, lsl #1
    sub r7, #1
    cmp r4, r7
    bgt out

    cmp r4, r6
    bgt  else
    if:
        mov r5, #1
        for1:
            sub r7, r6, r4
            cmp r5, r7
            bgt  for2_out

            ldr r0, =.LC0
            bl printf

            add r5, #1
            b for1

        for2_out:
        mov r5, #1
        for2_in:
            mov r7, r4, lsl #1
            sub r7, #1
            cmp r5, r7
            bgt for2_done

            ldr r0, =.LC1
            bl printf

            add r5, #1
            b for2_in
            
        for2_done:
            ldr r0, =.LC2
            bl printf
    if_end:

    cmp r4, r7
    ble for_end 

    else:
        mov r5, #1
        for3:
            sub r7, r4, r6
            cmp r5, r7
            bgt  for4_out

            ldr r0, =.LC0
            bl printf

            add r5, #1
            b for3

        for4_out:
        mov r5, #1
        for4_in:
            mov r7, r6, lsl #1
            sub r7, r7, #1
            sub r8, r4, r6
            mov r8, r8, lsl #1
            sub r7, r7, r8
            cmp r5, r7
            bgt for4_done

            ldr r0, =.LC1
            bl printf

            add r5, #1
            b for4_in
            
        for4_done:
            ldr r0, =.LC2
            bl printf
for_end:
    add r4, #1
    b for
out:
    mov r0, #0
    pop {r4, r5, r6, r7, r8, pc}
