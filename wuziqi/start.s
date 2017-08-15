	.text
	.align	2
	.global	_start
_start:
	stmfd sp!, {lr}
	
	@svc -> undef
	mrs	r0, cpsr
	bic r0, r0, #0x1f
	orr r0, r0, #0x1b
	msr cpsr, r0

	@undef -> svc
	mrs	r0, cpsr
	bic r0, r0, #0x1f
	orr r0, r0, #0x13
	msr cpsr, r0


	bl	led_init
	bl	key_init
	bl	buzzer_init
	bl	adc_init
	mov r0, #0
	bl	uart_init

	bl	main

	ldmfd sp!, {pc}
