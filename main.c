/* COMP2215: Task 02---MODEL ANSWER */
/* For La Fortuna board.            */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "fortuna2048.h"
#include "ruota.h"
#include "rios.h"

#define BUFFSIZE 256

void init(void);
uint16_t getRandomInt();

void main(void) 
{
    init();
    unsigned int seed = 0;
    while(!get_switch_press(_BV(SWN)))
        seed++;

    srand(seed);
    uint16_t pos1 = getRandomInt();
    uint16_t pos2 = getRandomInt();
    while(pos2 == pos1){pos2 = getRandomInt();} //Loop until 2 unique numbers
    grid2048 grid = {{0}};
    grid.data[pos1] = 2;
    grid.data[pos2] = 2;
    display_grid(grid);
}


void init(void) 
{
    /* 8MHz clock, no prescaling (DS, p. 48) */
    CLKPR = (1 << CLKPCE);
    CLKPR = 0;

    init_lcd();
    os_init_scheduler();
    os_init_ruota();
    sei();
}

uint16_t getRandomInt()
{
    return rand() % 15;
}
