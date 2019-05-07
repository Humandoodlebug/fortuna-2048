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
void clear_switches();
void execute_move(uint8_t direction);

void main(void) 
{
    init();
    start:
    display_start_screen();
    unsigned int seed = 0;
    clear_switches();
    while(!get_switch_press(_BV(SWC)))
        seed++;
    srand(seed);
    init_grid();
    draw_screen();
    clear_switches();
    for (;;)
    {
        if (!can_move())
            goto start;

        if (get_switch_press(_BV(SWN)))
            execute_move(UP);
        else if (get_switch_press(_BV(SWE)))
            execute_move(RIGHT);
        else if (get_switch_press(_BV(SWS)))
            execute_move(DOWN);
        else if (get_switch_press(_BV(SWW)))
            execute_move(LEFT);
    }
}

void execute_move(uint8_t direction)
{
    if (move_tiles(direction))
    {
        add_tile();
        redraw_screen();
    }
    clear_switches();
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

void clear_switches()
{
    while (get_switch_press(_BV(SWN)));
    while (get_switch_press(_BV(SWE)));
    while (get_switch_press(_BV(SWS)));
    while (get_switch_press(_BV(SWW)));
    while (get_switch_press(_BV(SWC)));
}