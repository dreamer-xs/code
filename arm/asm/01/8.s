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
    push {lr}

   @mov r1, #0xff37
    ldr r1, =0xff37
    ldr r2, =(((1<<10)-1) <<3)

    and r1, r1, r2
    lsr r1,  #3
    


    ldr r0, =.LC1
    bl printf

    mov r0, #0
    pop {pc}

