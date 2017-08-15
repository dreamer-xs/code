#include <gpio.h>
#include <config.h>

void buzzer_init(void)
{
	set_nbit(GPD0CON, 0, 4, 0x1);
}

void buzzer_on(void)
{
	set_one(GPD0DAT, 0);
}

void buzzer_off(void)
{
	set_zero(GPD0DAT, 0);
}


	
