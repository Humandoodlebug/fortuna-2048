#include "fortuna2048.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "lcd.h"
// #include <util/delay.h>

#define GRID_X 65
#define GRID_Y 40
#define GRID_CELL 45
#define GRID_GAP (GRID_CELL + LINE_THICKNESS)
#define GRID_LEN 16
#define GRID_D 4
#define grid(x,y) (grid[GRID_D * (x) + (y)])

/* Colour definitions RGB565 */
#define COLOUR2     0x6B4D //Light Gray
#define COLOUR4     0x5228 //Dark Gray
#define COLOUR8     0xFB86 //Light Orange
#define COLOUR16    0xC222 //Dark Orange
#define COLOUR32    0xF248 //Light Red
#define COLOUR64    0xE840 //Dark Red
#define COLOUR128   0xFE6C
#define COLOUR256   0xFE29
#define COLOUR512   0xF5A6
#define COLOUR1024  0xF542
#define COLOUR2048  0xD4A0

static uint16_t *grid;
uint16_t currentScore;

void display_score();
void display_grid();
void display_blocks();
void draw_block(uint8_t x, uint8_t y, uint16_t v);
uint8_t int2strl(uint16_t i);
void int2str(uint16_t i, char *str);
uint16_t getBlockTextX(uint8_t x, uint8_t l);
uint16_t getBlockTextY(uint8_t y);

void init_grid()
{
    grid = calloc(GRID_LEN, sizeof(uint16_t));
    add_tile();
    add_tile();
    currentScore = 0;
}

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

void redraw_screen()
{
    clear_screen();
    display_grid();
    display_blocks();
    display_score();
}

void display_score()
{
    uint16_t scoreLength = int2strl(currentScore);
    char scoreString[16 + scoreLength];
    sprintf(scoreString, "Current Score: %u", currentScore);
    display_string_xy(scoreString, 10, 10);
}

void display_grid()
{
    for (uint16_t y = 0; y < 5; y++)
        display_line_h(GRID_X, GRID_Y + y * GRID_GAP, 190);

    for (uint16_t x = 0; x < 5; x++)
        display_line_v(GRID_X + x * GRID_GAP, GRID_Y, 190);
}

void display_blocks()
{
    for (uint8_t i = 0; i < GRID_D; i++)
        for (uint8_t j = 0; j < GRID_D; j++)
            draw_block(i, j, grid(i, j));
}

void draw_block(uint8_t x, uint8_t y, uint16_t v)
{
    if(!v) return; //blocks with value 0 should not be displayed.
    uint8_t l = int2strl(v);
    char str[l + 1];    //NUUUUUULLLLL
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

// Returns the number of tiles effected.
uint8_t move_tiles(uint8_t direction)
{
    switch (direction)
    {
        case UP:
            for (int x = 0; x < GRID_D; x++)
            {
                int newY = 0;
                for (int y = 0; y < GRID_D; y++)
                {
                    if (!grid(x,y))
                        continue;

                    if (newY != y)
                    {
                        grid(x,newY) = grid(x,y);
                        grid(x,y) = 0;
                    }
                    // redraw_screen();
                    // _delay_ms(5000);

                    for (int i = newY + 1; i < GRID_D; i++)
                    {
                        if (grid(x,i))
                        {
                            if (grid(x,i) == grid(x,newY))
                            {
                                grid(x,newY) += grid(x,i);
                                currentScore += grid(x, newY);
                                grid(x,i) = 0;
                            }
                            break;
                        }
                    }
                    newY++;
                }
            }
            break;
        case RIGHT:
            for (int y = 0; y < GRID_D; y++)
            {
                int newX = GRID_D - 1;
                for (int x = GRID_D - 1; x >= 0; x--)
                {
                    if (!grid(x,y))
                        continue;

                    if (newX != x)
                    {
                        grid(newX, y) = grid(x,y);
                        grid(x,y) = 0;
                    }
                    // redraw_screen();
                    // _delay_ms(5000);

                    for (int i = newX - 1; i >= 0; i--)
                    {
                        if (grid(i,y))
                        {
                            if (grid(i,y) == grid(newX,y))
                            {
                                grid(newX,y) += grid(i,y);
                                currentScore += grid(newX, y);
                                grid(i,y) = 0;
                            }
                            break;
                        }
                    }
                    newX--;
                }
            }
            break;
        case DOWN:
            for (int x = 0; x < GRID_D; x++)
            {
                int newY = GRID_D - 1;
                for (int y = GRID_D - 1; y >= 0; y--)
                {
                    if (!grid(x,y))
                        continue;

                    if (newY != y)
                    {
                        grid(x,newY) = grid(x,y);
                        grid(x,y) = 0;
                    }
                    // redraw_screen();
                    // _delay_ms(5000);

                    for (int i = newY - 1; i >= 0; i--)
                    {
                        if (grid(x,i))
                        {
                            if (grid(x,i) == grid(x,newY))
                            {
                                grid(x,newY) += grid(x,i);
                                currentScore += grid(x, newY);
                                grid(x,i) = 0;
                            }
                            break;
                        }
                    }
                    newY--;
                }
            }
            break;
        case LEFT:
            for (int y = 0; y < GRID_D; y++)
            {
                int newX = 0;
                for (int x = 0; x < GRID_D; x++)
                {
                    if (!grid(x,y))
                        continue;

                    if (newX != x)
                    {
                        grid(newX, y) = grid(x,y);
                        grid(x,y) = 0;
                    }
                    // redraw_screen();
                    // _delay_ms(5000);

                    for (int i = newX + 1; i < GRID_D; i++)
                    {
                        if (grid(i,y))
                        {
                            if (grid(i,y) == grid(newX,y))
                            {
                                grid(newX,y) += grid(i,y);
                                currentScore += grid(newX, y);
                                grid(i,y) = 0;
                            }
                            break;
                        }
                    }
                    newX++;
                }
            }
            break;
        default:
            display_string_xy("Oops! Something went wrong...\n\tUnrecognised direction, integer was out of range!",0,0);
    }
    return 1;   //Needs to be replaced with the number of tiles moved.
}

void add_tile()
{
    uint8_t blankCount = 0;
    for (int i = 0; i < GRID_D; i++)
        for (int j = 0; j < GRID_D; j++)
            if (!grid(i,j))
                blankCount++;

    if (blankCount == 0)
        display_string_xy("Oops! Something went wrong...\n\tNo blank tiles found!", 0,0);

    uint8_t r = rand() % blankCount;

    blankCount = 0;
    for (int i = 0; i < GRID_D; i++)
        for (int j = 0; j < GRID_D; j++)
            if (!grid(i,j) && r == blankCount++)
            {
                grid(i,j) = 2;      // TODO: Give this a small probability of being 4.
                return;
            }
}
