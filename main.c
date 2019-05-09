/* COMP2215: Task 02---MODEL ANSWER */
/* For La Fortuna board.            */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include <stdio.h>
#include "fortuna2048.h"
#include "ruota.h"
#include "rios.h"

#define BUFFSIZE 256
//WARNING - No wear leveling is used!!!!!
#define HIGHSCORE_LOCATION 500

uint16_t highScore;

void init(void);
void clear_switches();
void get_highscore();
void execute_move(uint8_t direction);

void main(void) 
{
    init();
    start:
    display_start_screen();
    get_highscore();
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

void get_highscore()
{
    uint16_t read[3];
    eeprom_read_block((void *) &read, (const void *) HIGHSCORE_LOCATION, 3 * sizeof(uint16_t));
    if (read[0] == 2048 && read[2] == 2048)
    {
        highScore = read[1];
    }
    else
    {
        //TODO: Write a 0 highscore to EEPROM
        highScore = 0;
    }
    char str[20];
    sprintf(str, "Highscore: %u", highScore);
    display_string_xy(str,0,0);
}

void clear_switches()
{
    while (get_switch_press(_BV(SWN)));
    while (get_switch_press(_BV(SWE)));
    while (get_switch_press(_BV(SWS)));
    while (get_switch_press(_BV(SWW)));
    while (get_switch_press(_BV(SWC)));
}