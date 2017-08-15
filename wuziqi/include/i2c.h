#ifndef	__I2C_H__
#define __I2C_H__

#define I2CCON(n)       (0x13860000+(n*0x10000) + 0x0)
#define I2CSTAT(n)      (0x13860000+(n*0x10000) + 0x4)
#define I2CADD(n)       (0x13860000+(n*0x10000) + 0x8)
#define I2CDS(n)        (0x13860000+(n*0x10000) + 0xC)
#define I2CLC(n)        (0x13860000+(n*0x10000) + 0x10)

#endif
