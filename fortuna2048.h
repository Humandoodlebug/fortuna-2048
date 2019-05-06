
#ifndef _INC_FORTUNA2048
#define _INC_FORTUNA2048

#include "lcd.h"

typedef struct
{
    uint8_t gridX, gridY;
    uint16_t value;

} block;

void display_grid();

#endif