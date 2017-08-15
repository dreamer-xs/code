#include <config.h>
#include <adc.h>
#include <gic.h>

void adc_init()
{
	set_zero(ADC_CFG, 16);
	set_nbit(ADCMUX, 0, 4, 0);
	set_val(ADCCON, (1<<16)|(1<<14)|(99<<6));
}

void adc_start()
{
	set_one(ADCCON, 0);
}

u8 adc_end_flag()
{
	return get_bit(ADCCON, 15);
}

u16 adc_data()
{
	return get_nbit(ADCDAT, 0, 12);
}

void adc_irq_init(void)
{
	set_one(IESR2, 19);
	
	adc_init();
}

void do_adc_irq(void)
{
	if(adc_end_flag())
		pf("adc data: %d\n", adc_data());	
	set_val(CLRINTADC, 0);
}

