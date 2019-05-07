
#ifndef _INC_FORTUNA2048
#define _INC_FORTUNA2048

#include "lcd.h"

#define UP      0
#define RIGHT   1
#define DOWN    2
#define LEFT    3

void display_start_screen();
void redraw_screen();
uint8_t move_tiles(uint8_t direction);
void init_grid();

#endif
