
#ifndef _INC_FORTUNA2048
#define _INC_FORTUNA2048

#include "lcd.h"

#define UP      0
#define RIGHT   1
#define DOWN    2
#define LEFT    3

// typedef struct { uint16_t data[16]; } grid2048;

void display_start_screen();
void redraw_screen();
// void move_tiles(uint8_t direction);
void init_grid();

#endif
