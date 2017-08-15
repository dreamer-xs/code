#include <config.h>
#include <uart.h>
#include <string.h>
#include <stdarg.h>

int putchar(int c)
{	
	if(c == '\n')
	{
		uart_putc(0, '\r');
		uart_putc(0, '\n');
	}
	else
		uart_putc(0, c);
	return 0;
}

void putint(int d)
{
	if(d == 0)
		return;

	putint(d / 10);
	putchar(d % 10 + '0');
}

void putoct(int d)
{
	if(d == 0)
		return;

	putoct(d / 8);
	putchar(d % 8 + '0');
}

void puthex(unsigned int d)
{
	if(d == 0)
		return;

	puthex(d / 16);
	putchar("0123456789abcdef"[d % 16]);
}

u8 getchar(void)
{
	u8 tmp;
	tmp = uart_getc(0);
	if(tmp == '\n' || tmp == '\r')
	{
		uart_putc(0, '\r');
		uart_putc(0, '\n');
	}
	else if(tmp == '\b')
	{
		uart_putc(0, tmp);
		uart_putc(0, ' ');
		uart_putc(0, tmp);
	}
	else
	{
		uart_putc(0, tmp);
	}
	return tmp;
}

int putline(const char *s)
{
	int i = 0;
	while(s[i])
	{
		uart_putc(0, s[i++]);
	}
	return strlen(s);
}

int puts(const char *s)
{
	int i = 0;
	while(s[i])
	{
		uart_putc(0, s[i++]);
	}
	uart_putc(0, '\r');
	uart_putc(0, '\n');
	return strlen(s);
}

char *gets(char *s)
{
	int i = 0;
	while(1)
	{
		s[i] = getchar();
		if(s[i] == '\r')
		{
			s[i] = 0;
			break;
		}
		else if(s[i] == '\b')
		{
			if(i > 0)
				s[--i] = 0;
		}
		else
		{
			i++;
		}
	}

	return s;
}

int printf(const char *format, ...)
{
	va_list ap;
	int d;
	u32 d2;
	char *s;
	char ch;

	va_start(ap, format);
	while(*format)
	{
		if(*format == '%')
		{
			format++;
			switch(*format)
			{
				case 'c':
					ch = (char)va_arg(ap, int);
					putchar(ch);
					break;
				case 'd':
					d = (int)va_arg(ap, int);
					if(!d)
						putchar(d+'0');
					else
						putint(d);
					break;

				case 'o':
					d = (int)va_arg(ap, int);
					putoct(d);
					break;

				case 'p':
					putline("0x");
				case 'x':
					d2 = (u32)va_arg(ap, u32);
					puthex(d2);
					break;
				case 's':
					s = (char *)va_arg(ap, char *);
					putline(s);
					break;
			}
		}
		else
		{
			putchar(*format);
		}
		format++;
	}

	va_end(ap);
}

