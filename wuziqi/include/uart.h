#ifndef	__UART_H__
#define __UART_H__

#include <config.h>

#if	0
#define	ULCON(n)		(0x13800000 + (n) * 0x10000 + 0x0)
#define	UCON(n)			(0x13800000 + (n) * 0x10000 + 0x4)
#define UFCON(n)		(0x13800000 + (n) * 0x10000 + 0x8)
#define UMCON(n)		(0x13800000 + (n) * 0x10000 + 0xc)
#define UTRSTAT(n)		(0x13800000 + (n) * 0x10000 + 0x10)
#define UERSTAT(n)		(0x13800000 + (n) * 0x10000 + 0x14)
#define UFSTAT(n)		(0x13800000 + (n) * 0x10000 + 0x18)
#define UMSTAT(n)		(0x13800000 + (n) * 0x10000 + 0x1c)
#define UTXH(n)			(0x13800000 + (n) * 0x10000 + 0x20)
#define URXH(n)			(0x13800000 + (n) * 0x10000 + 0x24)
#define UBRDIV(n)		(0x13800000 + (n) * 0x10000 + 0x28)
#define UFRACVAL(n)		(0x13800000 + (n) * 0x10000 + 0x2c)
#define UINTP(n)		(0x13800000 + (n) * 0x10000 + 0x30)
#define UINTSP(n)		(0x13800000 + (n) * 0x10000 + 0x34)
#define UINTM(n)		(0x13800000 + (n) * 0x10000 + 0x38)
#else

struct uart{
	u32 ULCON;
	u32 UCON;
	u32 UFCON;
	u32 UMCON;
	u32 UTRSTAT;
	u32 UERSTAT;
	u32 UFSTAT;
	u32 UMSTAT;
	u32 UTXH;
	u32 URXH;
	u32 UBRDIV;
	u32 UFRACVAL;
	u32 UINTP;
	u32 UINTSP;
	u32 UINTM;
};

struct uart* get_uart_addr(int num);

#endif

void uart_init(int n);
void uart_putc(int n, u8 ch);
u8 uart_getc(int n);


#endif
