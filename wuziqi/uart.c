#include <config.h>
#include <gpio.h>
#include <uart.h>

struct uart *get_uart_addr(int n)
{
	return (struct uart *)(0x13800000 + n * 0x10000);
}

void uart_init(int n)
{
	struct uart *reg;
	reg = get_uart_addr(n);

	if(n == 0)
	{
		gpio_set_cfg(GPA0CON, 0, 0x2);	
		gpio_set_cfg(GPA0CON, 1, 0x2);	
	}
	else if(n == 1)
	{
		gpio_set_cfg(GPA0CON, 4, 0x2);	
		gpio_set_cfg(GPA0CON, 5, 0x2);	
	}
	else if(n == 2)
	{
		gpio_set_cfg(GPA1CON, 0, 0x2);	
		gpio_set_cfg(GPA1CON, 1, 0x2);	
	}
	else if(n == 3)
	{
		gpio_set_cfg(GPA1CON, 4, 0x2);	
		gpio_set_cfg(GPA1CON, 5, 0x2);	
	}

	reg->ULCON = 3;
	reg->UCON = 5;
	reg->UBRDIV = 53;
	reg->UFRACVAL = 5;
}

void uart_putc(int n, u8 ch)
{
	struct uart *reg;
	reg = get_uart_addr(n);

	while(!(reg->UTRSTAT & (1<<2)))
		;

	reg->UTXH = ch;
}

u8 uart_getc(int n)
{
	struct uart *reg;
	reg = get_uart_addr(n);

	while(!(reg->UTRSTAT & 1))
		;

	return reg->URXH;
}

