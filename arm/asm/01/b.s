.global main
main:
    mov r10, lr
    mov r0, #1

    mov lr, pc
    b sleep

    mov r0, #111
    mov lr, r10
    mov pc, lr
