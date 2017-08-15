#include <config.h>
#include <gpio.h>

void led_init(void)
{
	set_nbit(GPM4CON, 0, 16, 0x1111);
}

void led_alloff(void)
{
	set_nbit(GPM4DAT, 0, 4, 0xf);
}

void led_allon(void)
{
	set_nbit(GPM4DAT, 0, 4, 0);
}

void led_on(unsigned int num)
{
	set_zero(GPM4DAT, num);
}	

void led_off(unsigned int num)
{
	set_one(GPM4DAT, num);
}

void led_set(unsigned int val)
{
	set_nbit(GPM4DAT, 0, 4, val);
}
	
u8 led_get(void)
{
	return get_nbit(GPM4DAT, 0, 4);
}
