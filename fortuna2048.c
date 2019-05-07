#include "fortuna2048.h"
#include <stdio.h>
#include <math.h>
#include "lcd.h"

#define GRID_X 55
#define GRID_Y 40
#define GRID_CELL 45
#define GRID_GAP (GRID_CELL + LINE_THICKNESS)

void display_grid();
void display_blocks(grid2048);
void draw_block(uint8_t x, uint8_t y, uint16_t v);
uint8_t int2strl(uint16_t i);
void int2str(uint16_t i, char *str);
uint16_t getBlockTextX(uint8_t x, uint8_t l);
uint16_t getBlockTextY(uint8_t y);

uint16_t powI(uint16_t i, uint16_t j)
{
    uint16_t v = 1;
    for (; j > 0; j--)
        v *= i;
    return v;
}

void display_start_screen()
{
    display_string_xy("",40,60);
    display_line("  ______         _                     ");
    display_line(" |  ____|       | |                    ");
    display_line(" | |__ ___  _ __| |_ _   _ _ __   __ _ ");
    display_line(" |  __/ _ \\| '__| __| | | | '_ \\ / _` |");
    display_line(" | | | (_) | |  | |_| |_| | | | | (_| |");
    display_line(" |_|  \\___/|_|   \\__|\\__,_|_| |_|\\__,_|");
    display_line("         ___   ___  _  _   ___         ");   
    display_line("        |__ \\ / _ \\| || | / _ \\        ");   
    display_line("           ) | | | | || || (_) |       ");   
    display_line("          / /| | | |__   _> _ <        ");   
    display_line("         / /_| |_| |  | || (_) |       ");   
    display_line("        |____|\\___/   |_| \\___/        "); 
    display_line("");  
    display_line("          Press Center to Start");
}

void redraw_screen(grid2048 grid)
{
    clear_screen();
    display_grid();
    display_blocks(grid);
}

void display_grid()
{
    for (uint16_t y = 0; y < 5; y++)
        display_line_h(GRID_X, GRID_Y + y * GRID_GAP, 190);

    for (uint16_t x = 0; x < 5; x++)
        display_line_v(GRID_X + x * GRID_GAP, GRID_Y, 190);
}

void display_blocks(grid2048 grid)
{
    for (uint8_t i = 0; i < 4; i++)
        for (uint8_t j = 0; j < 4; j++)
            draw_block(j, i, grid.data[i * 4 + j]);
}

void draw_block(uint8_t x, uint8_t y, uint16_t v)
{
    if(v == 0){return;} //blocks with value 0 should not be displayed.
    int l = int2strl(v);
    char str[l];
    int2str(v, str);
    display_string_xy(str, getBlockTextX(x, l), getBlockTextY(y));
}

uint8_t int2strl(uint16_t i)
{
    return (uint8_t) log10(i) + 1;
}

void int2str(uint16_t i, char *str)
{
    sprintf(str, "%u", i);
}

uint16_t getBlockTextX(uint8_t x, uint8_t l)
{
    return GRID_X + (uint16_t) x * GRID_GAP + LINE_THICKNESS + (GRID_CELL - 6 * (uint16_t) l) / 2;
}

uint16_t getBlockTextY(uint8_t y)
{
    return GRID_Y + (uint16_t) y * GRID_GAP + LINE_THICKNESS + (GRID_CELL - 7) / 2;
}
