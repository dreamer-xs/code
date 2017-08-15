#ifndef	__CONFIG_H__
#define __CONFIG_H__

#define pf		((int (*)(const char *, ...))0x43e11a2c)
#define udelay	((void (*)(unsigned long usec))0x43e26480)

#define debug   pf("%s   %d\n", __func__,  __LINE__);

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef volatile u32 v32;
typedef volatile u16 v16;
typedef volatile u8 v8;


#define set_one(reg, bit)	\
			do {	\
				(*(v32 *)reg) |= (1<<bit);\
			}while(0)
#define set_zero(reg, bit)	\
			do {\
				(*(v32 *)reg) &= ~(1<<bit);\
			}while(0)
#define set_bit(reg, bit, val)	\
			do {\
				(*(v32 *)reg) &= ~(1<<bit);\
				(*(v32 *)reg) |= ((val&1)<<bit);\
			}while(0)
#define set_2bit(reg, bit, val)	\
			do {\
				(*(v32 *)reg) &= ~(3<<bit);\
				(*(v32 *)reg) |= ((val&3)<<bit);\
			}while(0)
#define set_nbit(reg, bit, n, val)	\
			do {\
				(*(v32 *)reg) &= ~(((1<<n)-1) << bit);\
				(*(v32 *)reg) |= ((val & ((1<<n)-1)) << bit);\
			}while(0)
#define set_val(reg, val)	\
			do{	\
				(*(v32 *)reg) = val;	\
			}while(0)


#define get_bit(reg, bit)	\
			((*(v32 *)reg & (1<<bit)) >> bit)
#define get_2bit(reg, bit)	\
			((*(v32 *)reg & (3<<bit)) >> bit)
#define get_nbit(reg, bit, n)	\
			((*(v32 *)reg & (((1<<n)-1) << bit)) >> bit)
#define get_val(reg)	\
			(*(v32 *)reg)

#define and_val(reg, val)	\
			do{	\
				(*(v32 *)reg) &= val;	\
			}while(0)
#define orr_val(reg, val)	\
			do{	\
				(*(v32 *)reg) |= val;	\
			}while(0)
#define eor_val(reg, val)	\
			do{	\
				(*(v32 *)reg) ^= val;	\
			}while(0)

#endif
