#include <config.h>
#include <pwm.h>
#include <stdio.h>
#include <gpio.h>

void pwm_timer_init(u32 ch, u32 us)
{
	if(ch > 4)
		return;

	//100MHZ / (49+1) / 2 = 1MHZ
	set_nbit(TCFG0, 0, 8, 49);
	set_nbit(TCFG0, 8, 8, 49);
	set_val(TCFG1, 0x11111);

	set_val(TCNTB(ch), us);
	if(ch == 0)
		set_one(TCON, 1);			//update TCNTB0
	else
		set_one(TCON, 9+(ch-1)*4);
	
	set_one(TINT_CSTAT, ch);
	if(ch == 0)
		set_nbit(TCON, 0, 4, (1<<3)|1);
	else if(ch == 4)
		set_nbit(TCON, 20, 3, (1<<2)|1);
	else
		set_nbit(TCON, 8+(ch-1)*4, 4, (1<<3)|1);
}

void pwm0_out(u32 cntus, u32 cmpus)
{
	//gpio 配置成PWM 输出;
	set_nbit(GPD0CON, 0, 4, 0x2);

	set_nbit(TCFG0, 0, 8, 49);
	set_nbit(TCFG1, 0, 4, 1);	

	set_val(TCNTB(0), cntus);
	set_val(TCMPB(0), cmpus);
	
	set_one(TCON, 1);

	set_nbit(TCON, 0, 4, (1<<3)|1);
}

void set_pwm0(u32 cntus, u32 cmpus)
{
	set_val(TCNTB(0), cntus);
	set_val(TCMPB(0), cmpus);
}
/*
u32 count = 0;
int l = 0;
int g = 0;
*/
void do_pwm(void)
{
	if(get_bit(TINT_CSTAT, 5))
	{
		printf("pwm0 time out\n");
		//led_set(~led_get());
	}
#if 1
	if(get_bit(TINT_CSTAT, 6))
	{
		printf("pwm1 time out\n");
	}
	if(get_bit(TINT_CSTAT, 7))
	{
		printf("pwm2 time out\n");
	}
	if(get_bit(TINT_CSTAT, 8))
	{
		printf("pwm3 time out\n");
	}
	if(get_bit(TINT_CSTAT, 9))
	{
		printf("pwm4 time out\n");
	}
#endif
	set_nbit(TINT_CSTAT, 5, 5, get_nbit(TINT_CSTAT, 5, 5));
}

void do_pwm0(void)
{
	if(get_bit(TINT_CSTAT, 5))
	{
		printf("pwm0 time out\n");
		set_one(TINT_CSTAT, 5);
	}
}
void do_pwm1(void)
{
	if(get_bit(TINT_CSTAT, 6))
	{
		printf("pwm1 time out\n");
		set_one(TINT_CSTAT, 6);
	}
}

void do_pwm2(void)
{
	if(get_bit(TINT_CSTAT, 7))
	{
		printf("pwm2 time out\n");
		set_one(TINT_CSTAT, 7);
	}
}

void do_pwm3(void)
{
	if(get_bit(TINT_CSTAT, 8))
	{
		printf("pwm3 time out\n");
		set_one(TINT_CSTAT, 8);
	}
}

void do_pwm4(void)
{
	if(get_bit(TINT_CSTAT, 9))
	{
		printf("pwm4 time out\n");
		set_one(TINT_CSTAT, 9);
	}
}

