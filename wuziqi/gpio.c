#include <gpio.h>
#include <config.h>

void gpio_set_cfg(u32 reg, int n, u32 cfg)
{
	set_nbit(reg, 4*n, 4, cfg);
}
