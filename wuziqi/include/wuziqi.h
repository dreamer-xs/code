#ifndef __WUZIQI_H__
#define __WUZIQI_H__
#include "config.h"

void draw_map(u32 addr);
void draw_point(u32 addr, u32 x, u32 y,  u32 col);
void draw_zi(u32 addr, u32 x, u32 y, u32 r);
u32 check_choice(u32 pos_y,u32 pos_x);
u32 chech_result(u32 fb_addr, u32 pos_x, u32 pos_y);


#endif
