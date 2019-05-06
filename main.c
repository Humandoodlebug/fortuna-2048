/* COMP2215: Task 02---MODEL ANSWER */
/* For La Fortuna board.            */


#include <avr/io.h>
#include "fortuna2048.h"


#define BUFFSIZE 256

void init(void);

void main(void) 
{
    init();
    grid2048 grid = {{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}};
    // grid.data = ;
    display_grid(grid);
}


void init(void) 
{
    /* 8MHz clock, no prescaling (DS, p. 48) */
    CLKPR = (1 << CLKPCE);
    CLKPR = 0;

    init_lcd();
}
