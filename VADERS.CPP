/// Adrian Pilkington - 2025
/// REMAKE OF INVADERS STYLE GAME,
/// COMPATIBLE WITH MSDOS AND VGA 320 x 200 pixels
/// BUILT UNDER TURBO C++ 3.0 USING DOSBOX

/// possibly requireing some assembly to speed things up
/// e.g. sprite draw function

#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <dos.h>
#include <mem.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

#define MODE_13 0x13
#define MODE_TEXT 0x03
#define PLOT_PIXEL(x,y,c) VGA[(y<<8)+(y<<6)+x]=c

typedef unsigned char byte;
byte far *VGA = (byte far*)0xA0000000L;

void set_videomode(int mode);
// some utility functions
void draw_box(int x0, int y0, int x1, int y1, byte colour);
void draw_line(int x0, int y0, int x1, int y1, byte colour);
void plotLineLow(int x0, int y0, int x1, int y1, byte colour);
void plotLineHigh(int x0, int y0, int x1, int y1, byte colour);
void drawSprite_slow(int x0, int y0, byte * spriteData, byte rows,byte cols, int colour);


int main()
{
    char c = ' ';
    byte moved = 0;
    int colour = 3;
    byte * spriteData;
    byte * blankSpriteData;
    int x = 100;
    int y = 100;
    int oldx = x;
    int oldy = y;

    // sprites will be 8 pixels wide by 8 high (for now)
    spriteData = (byte*)malloc(8);
    blankSpriteData = (byte*)malloc(8);
    memset (blankSpriteData, 0xff, sizeof(byte) * 8);

    // this is our litte space ship :)
    spriteData[0] = 0x18;   //  00011000
    spriteData[1] = 0x18;   //  00011000;
    spriteData[2] = 0x3c;   //  00111100;
    spriteData[3] = 0xa5;   //  10100101;
    spriteData[4] = 0xbd;   //  10111101;
    spriteData[5] = 0xff;   //  11111111;
    spriteData[6] = 0x7e;   //  01111110;
    spriteData[7] = 0x3c;   //  00111100;



    if ((spriteData == NULL) || (blankSpriteData == NULL))
    {
    	printf("failed To malloc sprite data\n");
        getch();
        exit(0);
    }

    set_videomode(MODE_13);

    while (c != 'q')
    {
        //testing draw sprite and moving around
        drawSprite_slow(x, y, spriteData, 8,8, colour);
        c = getch();
        oldx = x;
        oldy = y;
        switch (c)
        {
            case 'o' : if (x > 1)
                       {
                        x = x - 1;
                        moved = 1;
                       }
                       break;
            case 'p' : if (x < SCREEN_WIDTH - 1)
                       {
                        x = x + 1;
                        moved = 1;
                       }
                       break;
            case 'a' : if (y > 1)
                       {
                        y = y - 1;
                        moved = 1;
                       }
                       break;
            case 'z' : if (y < SCREEN_HEIGHT-1)
                       {
                        y = y + 1;
                        moved = 1;
                       }
                       break;
            default  : moved = 0;
                       break;
        };
        // only blank sprite if moved flag == 1
        if (moved == 1)
        {
            drawSprite_slow(oldx, oldy, blankSpriteData, 8,8, 0);
            moved = 0;
        }
    }

    free(spriteData);
    free(blankSpriteData);

    set_videomode(MODE_TEXT);
    return 0;
}


void drawSprite_slow(int x0, int y0, byte * spriteData, byte rows,byte cols, int colour)
{
    // no limit checking for screen edges
    int x = x0;
    int y = y0;
	byte r = 0;
	byte c = 0;
    byte * ptr = spriteData;
	for (r = 0; r < rows; r++)
	{
        byte mask = 0x01;
        x = x0;
        for (c = 0; c < cols; c++)
        {
            if (mask & *ptr)
            {
                PLOT_PIXEL(x, y, colour);
            }
            mask = mask << 1;
            x = x + 1;
        }
        y = y + 1;
        ptr = ptr + 1;
    }

}

void draw_box(int x0, int y0, int x1, int y1, byte colour)
{
   // x0,y0--------x0,y1
   // |            |
   // |            |
   // x0,y1--------x1,y1

   // draw box just draws 4 lines!
   draw_line(x0,y0,x1,y0,colour);
   draw_line(x0,y1,x1,y1,colour);
   draw_line(x1,y0,x1,y1,colour);
   draw_line(x0,y0,x0,y1,colour);

  // test  - commented out but could add a test mode
   //draw_line(10,10,100,10,7);
   //draw_line(10,100,100,100,7);
   //draw_line(100,10,100,100,7);
   //draw_line(10,10,10,100,7);
}

void draw_line(int x0, int y0, int x1, int y1, byte colour)
{

    if (abs(y1 - y0) < abs(x1 - x0))
    {
        if (x0 > x1)
        {
            plotLineLow(x1,y1,x0,y0,colour);
        }
        else
        {
            plotLineLow(x0,y0,x1,y1,colour);
        }
    }
    else
    {
        if (y0 > y1)
        {
            plotLineHigh(x1,y1,x0,y0,colour);
        }
	else
        {
            plotLineHigh(x0,y0,x1,y1,colour);
        }
    }
}

void plotLineLow(int x0, int y0, int x1, int y1, byte colour)
{
    int dx, dy, yi, d, x, y;
    dx = x1 - x0;
    dy = y1 - y0;
    yi = 1;

    if (dy < 0)
    {
        yi = -1;
        dy = -dy;
    }

    x = x0;
    y = y0;
    d = (2*dy) - dx;

    for (x = x0; x <= x1; x++)
    {
       PLOT_PIXEL(x,y,colour);
       if (d > 0)
       {
           y = y + yi;
           d = d + (2 * (dy - dx));
       }
       else
       {
           d = d + (2 * dy);
       }
   }
}

void plotLineHigh(int x0, int y0, int x1, int y1, byte colour)
{
    int dx, dy, xi, d, x, y;
    dx = x1 - x0;
    dy = y1 - y0;
    xi = 1;

    if (dx < 0)
    {
        xi = -1;
        dx = -dx;
    }

    x = x0;
    y = y0;
    d = (2*dx) - dy;

    for (y = y0; y <= y1; y++)
    {
       PLOT_PIXEL(x,y,colour);
       if (d > 0)
       {
           x = x + xi;
           d = d + (2 * (dx - dy));
       }
       else
       {
           d = d + (2 * dx);
       }
   }
}

void set_videomode(int mode)
{
    union REGS regs;
    regs.h.ah =0x00;
    regs.h.al =mode;
    int86(0x10,&regs,&regs);
}
