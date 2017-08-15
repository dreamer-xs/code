#ifndef	__GIC_H__
#define __GIC_H__

#define ICCICR_CPU0     0x10480000

#define ICCPMR_CPU0     0x10480004
#define ICCIAR_CPU0     0x1048000c

#define ICCEOIR_CPU0    0x10480010
#define ICDDCR          0x10490000

#define ICDISER_CPU0(n)	(0x10490100+n*4)
#define ICDICER_CPU0(n)	(0x10490180+n*4)
#define ICDIPR_CPU0(n)	(0x10490400+n*4)
#define ICDIPTR_CPU0(n)	(0x10490800+n*4)
#define ICDSGIR             0x10490f00


#define IESR2		0x10440020
#define IECR2		0x10440024
#define ISTR2		0x10440028
#define IMSR2		0x1044002C


#endif
