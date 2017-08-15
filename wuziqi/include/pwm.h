#ifndef	__PWM_H__
#define __PWM_H__

#define TCFG0	0x139D0000
#define TCFG1	0x139D0004
#define TCON	0x139D0008
#define TCNTB(n)	(0x139D000C+n*12)
#define TCMPB(n)	((n) == 4 ? -1 : (0x139D0010+(n)*12))
#define TCNTO(n)	((n) == 4 ? 0x139D0040 : 0x139D0014+(n)*12)
#if 0
#define TCNTB0	0x139D000C
#define TCMPB0	0x139D0010
#define TCNTO0	0x139D0014
#define TCNTB1	0x139D0018
#define TCMPB1	0x139D001C
#define TCNTO1	0x139D0020
#define TCNTB2	0x139D0024
#define TCMPB2	0x139D0028
#define TCNTO2	0x139D002C
#define TCNTB3	0x139D0030
#define TCMPB3	0x139D0034
#define TCNTO3	0x139D0038
#define TCNTB4	0x139D003C
#define	TCNTO4	0x139D0040
#endif
#define TINT_CSTAT	0x139D0044



#endif
