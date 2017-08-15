#include <gpio.h>
#include <config.h>

void key_init(void)
{
	set_nbit(GPX3CON, 8, 16, 0);
}

unsigned char get_key(void)
{
	return get_nbit(GPX3DAT, 2, 4);
}


void key_irq_init(void)
{
	set_nbit(GPX3CON, 8, 16, 0xffff);

	//配置成下降延触发;
	set_nbit(EXT_INT43CON, 8, 16, 0x2222);

	//使能中断;
	set_nbit(EXT_INT43_MASK, 2, 4, 0x0);

}

void do_key_irq(void)
{
	udelay(200000);
	u8 tmp = get_nbit(EXT_INT43_PEND, 2, 4);
	switch(tmp)
	{
		case 1:
			pf("key1 irq\n");
			break;
		case 1<<1:
			pf("key2 irq\n");
			break;
		case 1<<2:
			pf("key3 irq\n");
			break;
		case 1<<3:
			pf("key4 irq\n");
			break;
	}
	set_nbit(EXT_INT43_PEND, 2, 4, tmp);
}


