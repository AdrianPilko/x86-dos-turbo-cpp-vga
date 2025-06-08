/// Adrian Pilkington - 2025
/// BUILT UNDER TURBO C++ 3.0 USING DOSBOX
/// experimenting with graphics.h examples

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <mem.h>
#include <stdlib.h>
#include <dos.h>

typedef unsigned char byte;

void drawSprite_slow(int x0, int y0, byte * spriteData, byte rows,byte cols, int colour,int big);
int getch_noblock();


int main(void)
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
    int missilex = 0;
    int missiley = 0;
    byte missileInFlight = 0;
    byte missileFired = 0;
    int maxx,maxy;


   /* request auto detection */
   int gdriver = DETECT, gmode, errorcode;


   /* initialize graphics and local variables */
   initgraph(&gdriver, &gmode, "c:\\bgi");

   /* read result of initialization */
   errorcode = graphresult();
   if (errorcode != grOk)  /* an error occurred */
   {
      printf("Graphics error: %s\n", grapherrormsg(errorcode));
      printf("Press any key to halt:");
      getch();
      exit(1); /* terminate with an error code */
   }
   maxx = getmaxx();
   maxy = getmaxy();
   x = maxx / 2;
   y = maxy / 2;

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

    drawSprite_slow(x, y, spriteData, 8,8, colour,1);

    while (c != 'q')
    {
        c = getch_noblock();
        oldx = x;
        oldy = y;
        if (c != -1)
        {
          switch (c)
          {
            case 'o' : if (x > 1)
                       {
                        x = x - 1;
                        moved = 1;
                       }
                       break;
            case 'p' : if (x < maxx - 1)
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
            case 'z' : if (y < maxy-1)
                       {
                        y = y + 1;
                        moved = 1;
                       }
                       break;
            case ' ' : missileFired = 1;
                       missileInFlight = 1;
                       break;
            default  : moved = 0;
                       break;
          };
        // only blank sprite if moved flag == 1
        }
        if (moved == 1)
        {
            drawSprite_slow(oldx, oldy, blankSpriteData, 8,8, 0,1);
            drawSprite_slow(x, y, spriteData, 8,8, colour,1);
            moved = 0;
        }

        // initialised missile
        if (missileFired == 1)
        {
            missilex = x;
            missiley = y-8;
            missileFired = 0;
            drawSprite_slow(oldx, oldy, blankSpriteData, 8,8, 0,1);
            drawSprite_slow(x, y, spriteData, 8,8, colour,1);
        }

        if (missileInFlight == 1)
        {
            byte missile[8];
            missile[0] = 0x00;
            missile[1] = 0x00;
            missile[2] = 0x08;
            missile[3] = 0x08;
            missile[4] = 0x08;
            missile[5] = 0x00;
            missile[6] = 0x00;
            drawSprite_slow(missilex,missiley, &missile[0],6,8, 7,1);
            missiley = missiley - 1;


            if (missiley < 4)
            {
                missileInFlight = 0;
                memset(missile,0,8 * sizeof(byte));
                drawSprite_slow(missilex,missiley, &missile[0],6,8, 7,1);
                drawSprite_slow(x, y, spriteData, 8,8, colour,1);
            }
        }
    }

    free(spriteData);
    free(blankSpriteData);




   /* clean up */
   getch();
   closegraph();
   return 0;
}


void drawSprite_slow(int x0, int y0, byte * spriteData, byte rows,byte cols, int colour,int big)
{
    const int bigSize = 4;
    // no limit checking for screen edges
    int x = x0;
    int y = y0;
	byte r = 0;
	byte c = 0;
    byte * ptr = spriteData;
    byte incPtr = 0;
    byte doMaskShift = 0;

    if (big == 1)
    {
        rows = rows * bigSize;
        cols = cols * bigSize;
    }

	for (r = 0; r < rows; r++)
	{
        byte mask = 0x01;
        x = x0;
        for (c = 0; c < cols; c++)
        {
            if (mask & *ptr)
            {
                putpixel(x,y,colour);
            }
            else
            {
                putpixel(x,y,0);
            }
            if (big == 1)
            {
                doMaskShift = doMaskShift + 1;
                if (doMaskShift >= bigSize)
                {
                    mask = mask << 1;
                    doMaskShift = 0;
                }
            }
            else
            {
                mask = mask << 1;
            }
            x = x + 1;
        }
        y = y + 1;

        if (big == 1)
        {
            incPtr++;
        }

        if (incPtr >= bigSize)
        {
            ptr = ptr + 1;
            incPtr = 0;
        }
    }

}


int getch_noblock()
{
    if (kbhit())
       return getch();
    else
       return -1;
}



