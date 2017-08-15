#include <config.h>
#include <i2c.h>
#include <gpio.h>


void i2c_init(int n)
{
	if(n != 1)
		return;

	
	set_nbit(GPD1CON, 8, 8, 0x22);
	//set_val(I2CLC(n), (1<<2)|(3));
	set_val(I2CCON(n), (1<<5)|15);
	//set_val(I2CCON(n), (1<<6)|(1<<5)|1);
	set_val(I2CSTAT(n), (1<<4));
}

void i2c1_master_send(u8 slave, u8 reg, u8 data)
{
	u8 buf[2] = {reg, data};
	int i = 0;
	while(get_bit(I2CSTAT(1), 5))
		delay(10000);

	set_nbit(I2CSTAT(1), 6, 2, 0x3);
	set_val(I2CDS(1), slave);
	set_val(I2CSTAT(1), 0xf0);
	delay(10000);

	while(1)
	{
		if(get_bit(I2CCON(1), 4))
		{
			if(i == 2)
				break;

			set_val(I2CDS(1), buf[i]);
			set_zero(I2CCON(1), 4);
			i++;
		}
		delay(10000);
	}

	set_val(I2CSTAT(1), 0xd0);
	set_zero(I2CCON(1), 4);	
	delay(10000);
}

void i2c1_master_recv(u8 slave, u8 reg, u8 *data)
{
	///////////////////////写要读的寄存器/////////////////////
	u8 buf[2] = {reg};
	int i = 0;
	while(get_bit(I2CSTAT(1), 5))
		delay(10000);

	set_one(I2CCON(1), 7);
	set_nbit(I2CSTAT(1), 6, 2, 0x3);
	set_val(I2CDS(1), slave);
	set_val(I2CSTAT(1), 0xf0);
	delay(10000);

	while(1)
	{
		if(get_bit(I2CCON(1), 4))
		{
			if(i == 1)
				break;

			set_val(I2CDS(1), buf[i]);
			set_zero(I2CCON(1), 4);
			i++;
		}
		delay(10000);
	}

	set_val(I2CSTAT(1), 0xd0);
	set_zero(I2CCON(1), 4);	
	delay(10000);
	
	//////////////////////读数据返回///////////////////////

	i = 0;
	while(get_bit(I2CSTAT(1), 5))
		delay(10000);
	
	set_nbit(I2CSTAT(1), 6, 2, 2);
	set_val(I2CDS(1), slave);
	set_val(I2CSTAT(1), 0xb0);
	delay(10000);

	while(1)
	{
		if(get_bit(I2CCON(1), 4))
		{
			if(i == 2)
				break;
			
			buf[i] = get_nbit(I2CDS(1), 0, 8);
			i++;
			set_zero(I2CCON(1), 4);
		}
		delay(10000);
	}
		
	set_val(I2CSTAT(1), 0x90);
	set_zero(I2CCON(1), 4);
	delay(10000);

	*data = buf[1];

	set_zero(I2CCON(1), 7);
}

void i2c1_master_recv_buf(u8 slave, u8 *data, int num)
{
	u8 tmp;
	int i = 0;
	//////////////////////读数据返回///////////////////////
	set_one(I2CCON(1), 7);

	i = 0;
	while(get_bit(I2CSTAT(1), 5))
	{
		set_val(I2CSTAT(1), 0x90);
		set_zero(I2CCON(1), 4);
		delay(100);
	}

	set_nbit(I2CSTAT(1), 6, 2, 2);
	set_val(I2CDS(1), slave);
	set_val(I2CSTAT(1), 0xb0);
	delay(100);

	while(!get_bit(I2CCON(1), 4))
		delay(100);

	tmp = get_nbit(I2CDS(1), 0, 8);
	set_zero(I2CCON(1), 4);
	delay(100);

	while(1)
	{
		if(get_bit(I2CCON(1), 4))
		{
			if(i == num)
				break;
			
			data[i] = get_nbit(I2CDS(1), 0, 8);
			i++;
			set_zero(I2CCON(1), 4);
		}
		delay(100);
	}
		
	set_val(I2CSTAT(1), 0x90);
	set_zero(I2CCON(1), 4);
	delay(100);

	set_zero(I2CCON(1), 7);
}

