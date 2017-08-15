#include <config.h>
#include <wdt.h>
#include <pmu.h>
#include <stdio.h>

void wdt_reset_init(int sec)
{
	//pmu
	set_zero(AUTOMATIC_WDT_RESET_DISABLE, 20);
	set_zero(MASK_WDT_RESET_REQUEST, 20);

	//PCLK/8bit/128
	//100MHZ/(255+1)/128 = 3052HZ
	set_val(WTCNT, 3052 * sec);
	set_val(WTCON, (0xff<<8)|(1<<5)|(3<<3)|1);
}

void wdt_irq_init(int sec)
{
	set_val(WTDAT, 3052 * sec);	
	set_val(WTCNT, 3052 * sec);
	set_val(WTCON, (0xff<<8)|(1<<5)|(3<<3)|(1<<2));
}

void do_wdt(void)
{
	printf("wdt time out\n");
	set_val(WTCLRINT, 0);
}

