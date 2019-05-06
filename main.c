/* COMP2215: Task 02---MODEL ANSWER */
/* For La Fortuna board.            */


#include <avr/io.h>
#include "lcd.h"


#define BUFFSIZE 256

void init(void);

void main(void) {
    init();
    display_string("Hello Southampton\\!\n");
    display_string("Hello UK!\n");
    display_string("Hello World 1!\n");
    display_string("Hello World 2!\n");
    display_string("Hello World 3!\n");
    display_string("Hello World 4!\n");
    display_string("Hello World 5!\n");
    display_string("Hello World 6!\n");
    display_string("Hello World 7!\n");
    display_string("Hello World 8!\n");
    display_string("Hello World 9!\n");
    display_string("Hello World 10!\n");
    display_string("Hello World 11!\n");
    display_string("Hello World 12!\n");
    display_string("Hello World 13!\n");
    display_string("Hello World 14!\n");
    display_string("Hello World 15!\n");
    display_string("Hello World 16!\n");
    display_string("Hello World 17!\n");
    display_string("Hello World 18!\n");
    display_string("Hello World 19!\n");
    display_string("Hello World 20!\n");
    display_string("Hello World 21!\n");
    display_string("Hello World 22!\n");
    display_string("Hello World 23!\n");
    display_string("Hello World 24!\n");
    display_string("Hello World 25!\n");
    display_string("Hello World 26!\n");
    display_string("Hello World 27!\n");
    display_string("Hello World 28!\n");
    display_string("Hello World 29!\n");
    display_string("Hello World 30!\n");
}


void init(void) {
    /* 8MHz clock, no prescaling (DS, p. 48) */
    CLKPR = (1 << CLKPCE);
    CLKPR = 0;

    init_lcd();
}
