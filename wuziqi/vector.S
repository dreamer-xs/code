
	.section	.rodata
	.align	2
.LC0:
	.string	"swi to svc\n"
	.align	2
.LC1:
	.string	"cpsr: %p\n"
	.align	2
.LC2:
	.string	"undef to undef\n"
	.align	2
.LC3:
	.string	"system call num: %d\n"
	.align	2
.LC4:
	.string	"undef : %p\n"
	.align	2
.LC5:
	.string	"pabort\n"
	.align	2
.LC6:
	.string	"addr: %p\n"

	.text
	.align	2
	.global	vector_start
vector_start:
	b	reset		@0x0
	b	undef		@0x4
	b	swi			@0x8
	b	pabort		@0xc
	b	dabort		@0x10
	b	.			@0x14
	b	irq			@0x18
	b	firq		@0x1c


reset:
	b	reset

undef:
	ldr sp, =0x7f000000
	stmfd sp!, {r0-r12, lr}
	sub r4, lr, #4

	ldr r0, =.LC2
	mov lr, pc
	ldr pc, =0x43e11a2c

	mrs r1, cpsr
	ldr r0, =.LC1
	mov lr, pc
	ldr pc, =0x43e11a2c

	ldr r1, [r4]
	ldr r0, =.LC4
	mov lr, pc
	ldr pc, =0x43e11a2c

	ldmfd sp!, {r0-r12, pc}^

swi:
	stmfd sp!, {r0-r12, lr}
	sub r4, lr, #4

	ldr r0, =.LC0
	mov lr, pc
	ldr pc, =0x43e11a2c	

	mrs r1, cpsr
	ldr r0, =.LC1
	mov lr, pc
	ldr pc, =0x43e11a2c

	ldr r0, =0xffffff
	ldr r2, [r4]
	and r1, r2, r0
	ldr r0, =.LC3
	mov lr, pc
	ldr pc, =0x43e11a2c

	ldmfd sp!, {r0-r12, lr}
	movs pc, lr

pabort:
	mov sp, #0x7f000000	
	stmfd sp!, {r0-r12, lr}

	mov r1, lr
	ldr r0, =.LC6
	mov lr, pc
	ldr pc, =0x43e11a2c

	ldr r0, =.LC5
	mov lr, pc
	ldr pc, =0x43e11a2c

	mrs r1, cpsr
	ldr r0, =.LC1
	mov lr, pc
	ldr pc, =0x43e11a2c

	ldmfd sp!, {r0-r12, lr}
	/*ldr lr, =__main_end
	movs pc, lr*/
	@movs pc, lr

dabort:
	mov sp, #0x7f000000
	stmfd sp!, {r0-r12, lr}

	mrs r1, cpsr
	ldr r0, =.LC1
	mov lr, pc
	ldr pc, =0x43e11a2c
	
	ldmfd sp!, {r0-r12, lr}
	subs pc, lr, #4

irq:
	mov sp, #0x7f000000
	stmfd sp!, {r0-r12, lr}

	//mrs r1, cpsr
	//ldr r0, =.LC1
	//mov lr, pc
	//ldr pc, =0x43e11a2c

	@bl	do_irq
	mov lr, pc
	ldr pc, =do_irq
	
	ldmfd sp!, {r0-r12, lr}
	subs pc, lr, #4

firq:
	b	firq

	.align	2
	.global	vector_end
vector_end:
	nop

