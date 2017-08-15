#include "wuziqi.h"

extern fb_addr;
static u32 flag = 0;
static u32 result[480][800] = {0};
//black first 
extern u32 black = 1;
extern u32 white = 2;
extern u32 curr_player = 0;

void draw_bg(u32 addr, u32 col)
{
    u32 x, y;
	u32 (*p)[800] = (void *)addr;
	int i, j;
	for(i = 0; i < 480; i++)
	{
		for(j = 0; j < 800; j++)
		{
            p[i][j] = col;
		}
	}

}
void draw_map(u32 addr)
{
    u32 x, y;
	u32 (*p)[800] = (void *)addr;
    draw_bg(fb_addr, 0xfed650);

    for(x = 0; x < 480 ; x += 40)
    {
        for( y = 0; y < 800; y++)
            draw_point(fb_addr, x, y, 0x0);
    }

    for(y = 0; y < 800 ; y += 40)
    {
        for( x = 0; x < 480; x++)
            draw_point(fb_addr, x, y, 0x0);
    }
    flag = 0;
    memset(result, 0, 480*800);
}

void draw_point(u32 addr, u32 x, u32 y,  u32 col)
{
	u32 (*p)[800] = (void *)addr;
    p[x][y] = col;
}

void draw_zi(u32 addr, u32 x, u32 y, u32 r)
{
	u32 (*p)[800] = (void *)addr;
	int i, j;
    u32 curr_col;
    u32 black_col = 0x0;
    u32 white_col= 0xeeeeee;

    flag ++;
	for(i = 0; i < 480; i++)
	{
		for(j = 0; j < 800; j++)
		{
			if((i-y)*(i-y) + (j-x)*(j-x) <= r*r)
            {
                //黑子先走
                curr_col = ((flag%2 == 1) ? black_col : white_col);
				p[i][j] = curr_col;
            }
		}
	}
}
void draw_main(u32 fb_addr)
{
    //                     y,  x
    draw_str_fg(fb_addr, 160, 120, 0xff0000, "Five In A Row");
    draw_str_fg(fb_addr, 120, 240, 0x0000ff, "Are You Ready?");
    //draw_str_fg(fb_addr, 200, 320, 0x0ffff0, "Yes");
    //draw_str_fg(fb_addr, 480, 320, 0xf0ff0f, "No");
    draw_str_fg(fb_addr, 220, 320, 0x0ffff0, "Start Game");
}

void draw_win(u32 fb_addr)
{
    //                     y,  x
    draw_str_fg(fb_addr, 220, 120, 0xff0000, "Game Over");
    if(curr_player == 2)
        draw_str_fg(fb_addr, 120, 240, 0x0000ff, "White Player Win!");
    else
        draw_str_fg(fb_addr, 80, 240, 0x0000ff, "Black Player Win!");

    draw_str_fg(fb_addr, 200, 320, 0x0ffff0, "Restart Game");
}
void draw_end(u32 fb_addr)
{
    //                     y,  x
    draw_str_fg(fb_addr, 160, 120, 0xff0000, "Five In A Row");
    draw_str_fg(fb_addr, 280, 280, 0x0000ff, "Goodbye");
}

u32 check_choice(u32 pos_x,u32 pos_y)
{
    u32 x, y;
    u32 start = 0;
    u32 stop = 0;

    if(((pos_x >= 320) && (pos_x <= 360) ) && ((pos_y >= 220) && (pos_y <= 560)))
    {
        pf("pos_x: %d  pos_y: %d    %s\n", pos_x, pos_y, __func__);
        start = 1;
    }
    /*
    if(((pos_x >= 320) && (pos_x <= 360) ) && ((pos_y >= 480) && (pos_y <= 560)))
    {
        stop = 2;
    }
    */

    return (start + stop);
}


u32 check_result(u32 fb_addr, u32 pos_x, u32 pos_y)
{
	u32 (*p)[800] = (void *)fb_addr;
    u32 i, j, count, off_set = 160;

    pf("pos_x    = %d     pos_y =    %d  position\n", pos_x, pos_y);

    //black = 1; white = 2
    curr_player = ((flag%2 == 1) ? black: white);

    pf("curr_player: %d\n", curr_player);

    result[pos_x][pos_y] = curr_player;

    count = 0;
    for(i = pos_x, j = (pos_y - off_set); j <= pos_y + off_set; j += 40)
    {
        if((i < 0) || (i > 480) || (j < 0) || (j > 800))
        {
            count = 0;
            continue;
        }
        if(result[i][j] == curr_player)
        {
            count++;
        }
        else
        {
            count = 0;
        }
        if(count == 5)
            return 1;

        pf("i   = %d       j =    %d    count = %d    %s\n", i, j, count, __func__);
    }
    pf("______________________\n");
    count = 0;
    for(j = pos_y, i = (pos_x - off_set); i <= pos_x + off_set; i += 40)
    {
        if((i < 0) || (i > 480) || (j < 0) || (j > 800))
        {
            count = 0;
            continue;
        }
        if(result[i][j] == curr_player)
        {
            count++;
        }
        else
        {
            count = 0;
        }
        if(count == 5)
            return 1;

        pf("i   = %d       j =    %d    count = %d    %s\n", i, j, count, __func__);
    }

    count = 0;
    for(i = (pos_x - off_set), j = (pos_y - off_set); (i <= pos_x + off_set) && (j <= pos_y + off_set); i += 40, j += 40)
    {
        if((i < 0) || (i > 480) || (j < 0) || (j > 800))
        {
            count = 0;
            continue;
        }
        if(result[i][j] == curr_player)
        {
            count++;
        }
        else
        {
            count = 0;
        }
        if(count == 5)
            return 1;

        pf("i   = %d       j =    %d    count = %d    %s\n", i, j, count, __func__);
    }

    count = 0;
    for(i = (pos_x + off_set), j = (pos_y - off_set); (i <= pos_x + off_set) && (j <= pos_y + off_set); i -= 40, j += 40)
    {
        if((i < 0) || (i > 480) || (j < 0) || (j > 800))
        {
            count = 0;
            continue;
        }
        if(result[i][j] == curr_player)
        {
            count++;
        }
        else
        {
            count = 0;
        }
        if(count == 5)
            return 1;

        pf("i   = %d       j =    %d    count = %d    %s\n", i, j, count, __func__);
    }
    return 0;
}

