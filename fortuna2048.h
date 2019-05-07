
#ifndef _INC_FORTUNA2048
#define _INC_FORTUNA2048

#include "lcd.h"

typedef struct { uint16_t data[16]; } grid2048;

void display_start_screen();
void display_grid(grid2048 grid);

#endif
