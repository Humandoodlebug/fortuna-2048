#include "fortuna2048.h"
#include <stdio.h>
#include <math.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include "lcd.h"
#include <util/delay.h>

#define GRID_X 65
#define GRID_Y 40
#define GRID_CELL 45
#define GRID_GAP (GRID_CELL + LINE_THICKNESS)
#define GRID_LEN 16
#define GRID_D 4
#define grid(x,y) (grid[GRID_D * (x) + (y)])

/* Colour definitions RGB565 */
#define COLOUR2     0xEF3B //Original source: https://github.com/gabrielecirulli/2048/blob/master/style/main.css
#define COLOUR4     0xEF19 //Converted using: http://www.barth-dev.de/online/rgb565-color-picker/
#define COLOUR8     0xF58F
#define COLOUR16    0xF4AC
#define COLOUR32    0xF3EB 
#define COLOUR64    0xF2E7 
#define COLOUR128   0xEE6E
#define COLOUR256   0xEE6C
#define COLOUR512   0xEE4A
#define COLOUR1024  0xEE27
#define COLOUR2048  0xEE05

//WARNING - No wear leveling is used!!!!!
#define HIGHSCORE_LOCATION 500

static uint16_t *grid;
uint16_t currentScore;
uint16_t highScore;


void display_score();
void get_eeprom_highscore();
void display_grid();
void display_blocks();
void draw_block(uint8_t x, uint8_t y, uint16_t v);
void increase_score(uint16_t s);
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
    clear_screen();

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

void display_game_over()
{
    display_string_xy("", 0, GRID_Y);
    display_line("   _____   ");
    display_line("  / ____|  ");
    display_line(" | |  __   ");
    display_line(" | | |_ |  ");
    display_line(" | |__| |  ");
    display_line("  \\__/\\_|  ");
    display_line("    /  \\   ");
    display_line("   / /\\ \\  ");
    display_line("  / ____ \\ ");
    display_line(" /_/  __\\_\\");
    display_line(" |  \\/  |  ");
    display_line(" | \\ \\/ |  ");
    display_line(" | |\\/| |  ");
    display_line(" | |  | |  ");
    display_line(" |_|__|_|  ");
    display_line(" |  ____|  ");
    display_line(" | |__     ");
    display_line(" |  __|    ");
    display_line(" | |____   ");
    display_line(" |______|  ");

    display_string_xy("", GRID_X + 190, GRID_Y);
    display_line("   ____");    
    display_line("  / __ \\   ");
    display_line(" | |  | |  ");
    display_line(" | |  | |  ");
    display_line(" | |__| |  ");
    display_line(" _\\____/ __");
    display_line(" \\ \\    / /");
    display_line("  \\ \\  / / ");
    display_line("   \\ \\/ /  ");
    display_line("    \\  /   ");
    display_line("  ___\\/_   ");
    display_line(" |  ____|  ");
    display_line(" | |__     ");
    display_line(" |  __|    ");
    display_line(" | |____   ");
    display_line(" |______|  ");
    display_line(" |  __ \\   ");
    display_line(" | |__) |  ");
    display_line(" |  _  /   ");
    display_line(" | | \\ \\   ");
    display_line(" |_|  \\_\\  ");
}

void draw_screen()
{
    clear_screen();
    display_grid();
    get_eeprom_highscore();
    redraw_screen();
}

void redraw_screen()
{
    display_blocks();
    display_score();
}

void display_score()
{
    uint16_t scoreLength = int2strl(currentScore);
    char scoreString[16 + scoreLength];
    sprintf(scoreString, "Current Score: %u", currentScore);
    display_string_xy(scoreString, 10, 10);

    sprintf(scoreString, "Highscore: %u", highScore);
    display_string_xy(scoreString, 200, 10);
}

void get_eeprom_highscore()
{
    uint16_t read[3];
    eeprom_read_block(&read, (const void *) HIGHSCORE_LOCATION, 3 * sizeof(uint16_t));
    if (read[0] == 2048 && read[2] == 2048)
    {
        highScore = read[1];
    }
    else
    {
        read[0] = 2048;
        read[1] = 0;
        read[2] = 2048;
        eeprom_update_block(&read,(void *) HIGHSCORE_LOCATION, 3 * sizeof(uint16_t));
        highScore = 0;
    }
}

void display_grid()
{
    rectangle rect = {GRID_X, GRID_X + 189, GRID_Y, GRID_Y + 189};
    fill_rectangle(rect, 0xAC8F);
}

void display_blocks()
{
    for (uint8_t i = 0; i < GRID_D; i++)
        for (uint8_t j = 0; j < GRID_D; j++)
            draw_block(i, j, grid(i, j));
}

void draw_block(uint8_t x, uint8_t y, uint16_t v)
{
    //Block color
    uint16_t colour;
    switch(v)
    {
        case 0:
            colour = 0xDE57;
            break;
        case 2:
            colour = COLOUR2;
            break;
        case 4:
            colour = COLOUR4;
            break;
        case 8:
            colour = COLOUR8;
            break;
        case 16:
            colour = COLOUR16;
            break;
        case 32:
            colour = COLOUR32;
            break;
        case 64:
            colour = COLOUR64;
            break;
        case 128:
            colour = COLOUR128;
            break;
        case 256:
            colour = COLOUR256;
            break;
        case 512:
            colour = COLOUR512;
            break;
        case 1024:
            colour = COLOUR1024;
            break;
        default:
            colour = COLOUR2048;
            break;
    }

    uint16_t left, right, top, bottom;
    left = GRID_X + x * GRID_GAP + LINE_THICKNESS;
    top = GRID_Y + y * GRID_GAP + LINE_THICKNESS;
    right = left + GRID_CELL - 1;
    bottom = top + GRID_CELL - 1; 
    rectangle rect = {left, right, top, bottom};
    fill_rectangle(rect, colour);
    //Block text
    uint8_t l = int2strl(v);
    char str[l + 1];    //NUUUUUULLLLL
    int2str(v, str);
    if(v) //No text if 0 block
    {
        display_string_xy_coloured(str, getBlockTextX(x, l), getBlockTextY(y), BLACK, colour);
    }
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
    uint8_t moved = 0;
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
                        moved++;
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
                                // currentScore += grid(x, newY);
                                increase_score(grid(x, newY));
                                grid(x,i) = 0;
                                moved++;
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
                        moved++;
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
                                // currentScore += grid(newX, y);
                                increase_score(grid(newX, y));
                                grid(i,y) = 0;
                                moved++;
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
                        moved++;
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
                                // currentScore += grid(x, newY);
                                increase_score(grid(x, newY));
                                grid(x,i) = 0;
                                moved++;
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
                        moved++;
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
                                // currentScore += grid(newX, y);
                                increase_score(grid(newX, y));
                                grid(i,y) = 0;
                                moved++;
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
    return moved;
}

void increase_score(uint16_t s)
{
    currentScore += s;
    if(highScore < currentScore)
    {
        highScore = currentScore;
    }
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

uint8_t can_move()
{
    for (uint8_t i = 0; i < GRID_D; i++)
        for (uint8_t j = 0; j < GRID_D; j++)
        {
            if (!grid(i,j))
                return 1;
            if (i+1 < GRID_D && grid(i,j) == grid(i+1,j))
                return 1;
            if (j+1 < GRID_D && grid(i,j) == grid(i,j+1))
                return 1;
        }
    return 0;
}

void save_highscore()
{
    uint16_t read[3];
    eeprom_read_block(&read, (const void *) HIGHSCORE_LOCATION, 3 * sizeof(uint16_t));
    if (read[0] == 2048 && read[2] == 2048) //Check if corruption occurred
    {
        if(read[1] < currentScore)
        {
            read[1] = currentScore;
            eeprom_update_block(&read,(void *) HIGHSCORE_LOCATION, 3 * sizeof(uint16_t));
        }       
    }
    else
    {
        display_string_xy("Error saving highscore", 100, 10);
        _delay_ms(5000); //5 second delay
    }
    
}
