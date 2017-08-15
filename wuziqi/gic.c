#include <config.h>
#include <gic.h>
#include <stdio.h>

void irq_enable(void)
{
	asm __volatile__(
	/*
		"mrs r0, cpsr\n\t"
		"bic r0, #(1<<7)\n\t"			//使能arm核的irq;
		"msr cpsr, r0\n\t"
		::: "r0"
	*/
		"cpsie i"
	);

}

void gic_irq_enable(u32 num, int pri)
{
	set_one(ICCICR_CPU0, 0);			//使能cpu 处理中断;
	set_nbit(ICCPMR_CPU0, 0, 8, 255);	//最低门槛;
	set_one(ICDDCR, 0);					//仲裁器使能;

	set_one(ICDISER_CPU0(num/32), num%32);		
	set_nbit(ICDIPR_CPU0(num/4), (num%4)*8, 8, pri);
	set_nbit(ICDIPTR_CPU0(num/4), (num%4)*8, 8, 1);	//目标cpu0;
	if(num < 16)
		set_val(ICDSGIR, (1<<16)|num);
}

void do_irq()
{
	u32 tmp = get_nbit(ICCIAR_CPU0, 0, 13);
	//printf("CPUID: %d IRQID: %d\n", (tmp >> 10), tmp & 0x3ff);
	switch(tmp & 0x3ff)
	{
		case 75:
			do_wdt();
			break;
		#if 0
		case 73:
			do_pwm4();
			break;
		case 72:
			do_pwm3();
			break;
		case 71:
			do_pwm2();
			break;
		case 70:
			do_pwm1();
			break;
		case 69:
			do_pwm0();
			break;
		#endif
		case 73:
		case 72:
			do_irq_time3();
			break;
		case 71:
		case 70:
		case 69:
			do_pwm();
			break;
		case 76:
			do_rtc_alarm_sec();
			break;
		case 77:
			do_rtc_tick();
			break;
		case 64:
			do_key_irq();
			break;
		case 42:
			do_adc_irq();
			break;
		case 62:
			do_tp();
			break;
	}
	set_nbit(ICCEOIR_CPU0, 0, 13, tmp);
}

