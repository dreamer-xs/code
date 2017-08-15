#include <config.h>
#include <gpio.h>

static u8 map[480][800] = {0};
static start= 0;
static stop = 0;
extern fb_addr;
extern curr_player;

void tp_irq_init(void)
{
	set_nbit(GPX1CON, 24, 4, 0xf);
	
	set_nbit(EXT_INT41CON, 24, 4, 0x2);
	set_zero(EXT_INT41_MASK, 6);
}

void do_tp(void)
{
    u8 i, num;
    u8 buf[100] = {};
    u16 pos_x = 0;
    u16 pos_y = 0;
    u32 x, y;

    memset(&buf, 0, 100);
	if(get_bit(EXT_INT41_PEND, 6))
		set_one(EXT_INT41_PEND, 6);

    i2c1_master_recv_buf(0x70, &buf, 32);
    num = buf[2];

    i = 3;
    for(i = 3; i < (6*num); i += 3)
    {
        pos_x = (((buf[i] & 0xf) << 8 ) | buf[++i]);
        pos_y = (((buf[++i] & 0xf) << 8 ) | buf[++i]);
        pf("pos_x[%d] = %d     pos_y[%d] = %d\n",(i-3)/6, pos_x,(i-3)/6, pos_y);
    }

    int ret;
    if(num)
    {
        if(!start)
        {
            ret = check_choice(pos_y, pos_x);
            if(ret == 1)
            {
                start = 1;
                pf("start : %d   %s\n",start, __func__);
                draw_map(fb_addr);
                return ;
            }
            else if(ret == 2)
            {
                stop = 1;
                pf("start : %d   %s\n",start, __func__);
                draw_bg(fb_addr, 0xefefef);
                draw_end(fb_addr);
                return ;
            }
            else
            {
                return;
            }
        }
        for(x = 0; x <= 800 ; x += 40)
        {
            if((pos_x - x) <= 20 || (pos_x -x) > -20)
            {
                if(pos_x >= 780)
                    return;
                else if(pos_x <= 20)
                    return;
                else
                    pos_x = x;
            }
        }

        for(y = 0; y <= 480 ; y += 40)
        {
            if((pos_y - y) <= 20 || (pos_y -y) > -20)
            {
                if(pos_y > 460)
                    return;
                else if(pos_y <= 20)
                    return;
                else
                    pos_y = y;
            }
        }

        if(map[pos_y][pos_x] == 1)
            return;
        else
            map[pos_y][pos_x] = 1;

        pf("pos_x    = %d     pos_y =    %d\n", pos_x, pos_y);
        draw_zi(fb_addr, pos_x, pos_y, 17);
        ret = check_result(fb_addr, pos_y, pos_x);
        if(ret)
        {
            if(curr_player == 2)
                pf("player white win!\n");
            else
                pf("player black win!\n");
            draw_win(fb_addr);
            memset(map, 0, 480*800);
            start= 0;
            stop = 0;
            return ;
        }
    }
}
