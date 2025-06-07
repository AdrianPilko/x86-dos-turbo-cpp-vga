#include <conio.h>
#include <stdlib.h>
#include <dos.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

#define MODE_13 0x13
#define MODE_TEXT 0x03
#define PLOT_PIXEL(x,y,c) VGA[(y<<8)+(y<<6)+x]=c

typedef unsigned char byte;
byte far *VGA = (byte far*)0xA0000000L;

void set_videomode(int mode);
void draw_box(int x0, int y0, int x1, int y1, byte colour);
void draw_line(int x0, int y0, int x1, int y1, byte colour);


int main()
{
    int colour,count = 20;

    set_videomode(MODE_13);

    //while(count--)
    {
        int x0 = rand()%SCREEN_WIDTH;
    	int x1 = rand()%SCREEN_WIDTH;
        int y0 = rand()%SCREEN_HEIGHT;
	    int y1 = rand()%SCREEN_HEIGHT;

	    colour = rand()%256;
	    //PLOT_PIXEL(x,y,colour);
        //draw_line(x0,y0,x1,y1,colour);
        //draw_line(10,10,100,100,colour);
        draw_box(10,10,100,100,colour);
    }
    getch();
    set_videomode(MODE_TEXT);
    return 0;
}

void draw_box(int x0, int y0, int x1, int y1, byte colour)
{
    // draw box just draws 4 lines!
  //  draw_line(x0,y0,x1,y0,colour);
   // draw_line(x1,y0,x1,y1,colour);
   // draw_line(x1,y0,x1,y1,colour);
   // draw_line(x0,y0,x1,y0,colour);
   draw_line(10,10,100,10,7);
}

void draw_line(int x0, int y0, int x1, int y1, byte colour)
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

    for (x = x0; x < x1; x++)
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

void set_videomode(int mode)
{
    union REGS regs;
    regs.h.ah =0x00;
    regs.h.al =mode;
    int86(0x10,&regs,&regs);
}
