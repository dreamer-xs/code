#include <config.h>
#include <cmu.h>
#include <gpio.h>
#include <lcd.h>

void lcd_init(u32 addr)
{
	set_one(LCDBLK_CFG, 1);

	set_val(GPF0CON, 0x22222222);
	set_val(GPF1CON, 0x22222222);
	set_val(GPF2CON, 0x22222222);
	set_val(GPF3CON, 0x2222);

	set_nbit(CLK_DIV_LCD, 0, 4, 0x9);
	set_nbit(CLK_SRC_LCD0, 0, 4, 0x6);

	set_val(VIDCON0, 1<<6);
	set_val(VIDCON1, 7<<5);
	set_val(VIDTCON0, (21<<16)|(6<<8));
	set_val(VIDTCON1, (44<<16)|(15<<8));
	set_val(VIDTCON2, (479<<11)|799);

	set_val(WINCON0, (1<<15)|(0xb<<2)|1);
	
	set_val(VIDOSD0A, 0);
	set_val(VIDOSD0B, (800<<11)|480);
	set_val(VIDOSD0C, 800 * 480);

	set_val(VIDW00ADD0B0, addr);
	set_val(VIDW00ADD1B0, addr + 800*480*4);
	set_val(VIDW00ADD2, 800 * 4);

	set_one(SHADOWCON, 0);

	set_nbit(VIDCON0, 0, 2, 3);
}

void lcd_set_fb(u32 addr)
{
	set_val(VIDW00ADD0B0, addr);
	set_val(VIDW00ADD1B0, addr + 800*480*4);
}

