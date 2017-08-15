#include <config.h>
#include <stdio.h>
#include <string.h>
#include <rtc.h>
#include <cmu.h>
#include <asc8x6.h>
#include <wuziqi.h>

extern void __bss_start(void);
extern void __bss_end(void);
extern void vector_start();
extern void vector_end();

extern u32 fb_addr = 0x5fff0000;

void draw_char(u32 addr, u32 x, u32 y, u32 fg, u32 bg, char ch);
void draw_char_fg(u32 addr, u32 x, u32 y, u32 fg, char ch);
void draw_str(u32 addr, u32 x, u32 y, u32 fg, u32 bg, char *str);
void draw_str_fg(u32 addr, u32 x, u32 y, u32 fg, char *str);

int w_size = 5;
u32 *ttb 	= (void *)0x70000000;	//一级映射表首地址; 必须16K地址对齐; 
int main(void)
{
	u32 *p;
	int i;
	int j;
	u32 col;
	u32 x, y, x_int, y_int, r;
	x = 100, y = 100, x_int = 1, y_int = 1, r = 50;
	mmu_disable();
	printf("main start\n");
	memset(__bss_start, 0, __bss_end - __bss_start);
	memset(ttb, 0, 4096 * 4);	//初始化映射表; 清空;

	//enable_cache();
	l1_init(ttb);
	//l1_init_cache(ttb);
	l1_mmap(ttb, 0xffff0000, 0x7fff0000);
	mmu_enable(ttb);

	//设置异常向量表地址为0xffff0000;
	asm __volatile__(
		"mrc p15, 0, r0, c1, c0, 0\n\t"
		"orr r0, r0, #(1<<13)\n\t"
		"mcr p15, 0, r0, c1, c0, 0\n\t"
	);

	//把vector_start开始往后0x100拷贝到0xffff0000;
	memcpy((void *)0xffff0000, vector_start, vector_end - vector_start + 0x100);


	i2c_init(1);
	irq_enable();
	gic_irq_enable(62, 62);
	gic_irq_enable(72, 72);
	start_bl(127);
	tp_irq_init();

    draw_bg(fb_addr, 0xffffff);
    draw_main(fb_addr);
    lcd_init(fb_addr);

	printf("main end\n");
	return 0;
}
void draw_str_fg(u32 addr, u32 x, u32 y, u32 fg, char *str)
{
	while(*str)
	{
		draw_char_fg(addr, x, y, fg, *str);
		if((x + 8 * w_size >= 800) || (*str == '\n'))
		{
			y += 8 * w_size;
			x = 0;
		}
		else
			x += 8 * w_size;
		if(y >= 480)
			y = 0;
		str++;
	}
}

void draw_str(u32 addr, u32 x, u32 y, u32 fg, u32 bg, char *str)
{
	while(*str)
	{
		draw_char(addr, x, y, fg, bg, *str);
		if((x + 8 * w_size >= 800) || (*str == '\n'))
		{
			y += 8 * w_size;
			x = 0;
		}
		else
			x += 8 * w_size;
		if(y >= 480)
			y = 0;
		str++;
	}
}

void draw_char_fg(u32 addr, u32 x, u32 y, u32 fg, char ch)
{
	u32 (*p)[800] = (void *)addr;
	int i, j;
	int m, n;
	u8 tmp;
	for(i = 0; i < 8; i++)
	{
		tmp = asc8x6[ch * 8 + i];
		for(j = 0; j < 8; j++)
		{
			if(tmp & (1<<j))
			{
				for(m = 0; m < w_size; m++)
				{
					for(n = 0; n < w_size; n++)
					{
						p[y+(j*w_size)+m][x+(i*w_size)+n]	= fg;
					}
				}
			}
		}
	}
}
void draw_char(u32 addr, u32 x, u32 y, u32 fg, u32 bg, char ch)
{
	u32 (*p)[800] = (void *)addr;
	int i, j;
	int m, n;
	u8 tmp;
	for(i = 0; i < 8; i++)
	{
		tmp = asc8x6[ch * 8 + i];
		for(j = 0; j < 8; j++)
		{
			if(tmp & (1<<j))
			{
				for(m = 0; m < w_size; m++)
				{
					for(n = 0; n < w_size; n++)
					{
						p[y+(j*w_size)+m][x+(i*w_size)+n]	= fg;
					}
				}
			}
			else
			{
				for(m = 0; m < w_size; m++)
				{
					for(n = 0; n < w_size; n++)
					{
						p[y+(j*w_size)+m][x+(i*w_size)+n] = bg;
					}
				}
			}
		}
	}
}

