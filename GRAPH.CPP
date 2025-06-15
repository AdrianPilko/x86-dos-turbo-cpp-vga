/// Adrian Pilkington - 2025
/// BUILT UNDER TURBO C++ 3.0 USING DOSBOX
/// experimenting with graphics.h examples
/// invader game - so called because currently only one!!
#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <mem.h>
#include <stdlib.h>
#include <dos.h>

#define NUMBER_OF_ENEMY_ROWS 6
#define SCORE_X_POS 50
#define LEVEL_X_POS 200
#define SCORE_INC 100
#define ENEMY_SPEED_INCREASE 3

typedef unsigned char byte;

void drawSprite_slow(int x0, int y0, byte * spriteData, byte rows,byte cols, int colour,int big);
int getch_noblock();


int main(void)
{
    unsigned long score = 0;
    char scoreStr[7];
    unsigned int level = 0;
    char levelStr[7];
    char c = ' ';
    byte moved = 0;
    int colour = 3;
    byte * spriteData;
    byte * blankSpriteData;
    byte * missile;
    byte * missileBang;
    byte * enemySpriteData;
    int x = 100;
    int y = 100;
    int oldx = x;
    int oldy = y;
    int missilex = 0;
    int missiley = 0;
    int enemyx = 40;
    int enemyxDirection = 1;
    int enemyy = 40;
    byte enemyMove = 40;
    byte enemySpriteOffset = 0;
    byte enemySpeed=2;

    byte missileInFlight = 0;
    byte missileFired = 0;
    int maxx,maxy;
    int tempIndex = 0;
    // this controls the enemys in the grid - if they are displayed or not
    // and if they are "hittable". we have 6 rows of 8
    byte enemyValid[NUMBER_OF_ENEMY_ROWS];
    for (tempIndex = 0; tempIndex < NUMBER_OF_ENEMY_ROWS; tempIndex++)
    {
        enemyValid[tempIndex] = 1;
    }


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
   y = maxy - 256;


   missile = (byte*)malloc(7);
   missileBang = (byte*)malloc(7);

   missile[0] = 0x00; // 00000000
   missile[1] = 0x00; // 00000000
   missile[2] = 0x18; // 00011000
   missile[3] = 0x18; // 00011000
   missile[4] = 0x18; // 00011000
   missile[5] = 0x00; // 00000000
   missile[6] = 0x00; // 00000000
   missileBang[0] = 0x83; // 10000011
   missileBang[1] = 0x28; // 00101000
   missileBang[2] = 0x24; // 00100100
   missileBang[3] = 0x5a; // 01011010
   missileBang[4] = 0x24; // 00100100
   missileBang[5] = 0x92; // 10010010
   missileBang[6] = 0x10; // 00010000
   missileBang[7] = 0x12;
   missileBang[8] = 0x15;



   enemySpriteData = (byte*)malloc(16);
   enemySpriteData[0] = 0x7e;   // 01111110
   enemySpriteData[1] = 0xdb;   // 11011011
   enemySpriteData[2] = 0xff;   // 11111111
   enemySpriteData[3] = 0x66;   // 01100110
   enemySpriteData[4] = 0x3c;   // 00111100
   enemySpriteData[5] = 0x24;   // 00100100
   enemySpriteData[6] = 0x42;   // 01000010
   enemySpriteData[7] = 0x81;   // 10000001

   enemySpriteData[8] = 0x7e;   // 01111110
   enemySpriteData[9] = 0xdb;   // 11011011
   enemySpriteData[10] = 0xff;   // 11111111
   enemySpriteData[11] = 0x42;   // 01000010
   enemySpriteData[12] = 0x3c;   // 00111100
   enemySpriteData[13] = 0x24;   // 00100100
   enemySpriteData[14] = 0x18;   // 00011000
   enemySpriteData[15] = 0x24;   // 00100100


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

    drawSprite_slow(enemyx, enemyy, enemySpriteData, 8,8, colour,1);

    drawSprite_slow(x, y, spriteData, 8,8, colour,1);

    missilex = x;
    missiley = y-24;
    missileFired = 0;
    missileInFlight = 0;

    outtextxy(0,0,"SCORE");
    sprintf(scoreStr,"%06d",score);
    outtextxy(SCORE_X_POS,0,scoreStr);
    outtextxy(150,0,"LEVEL");
    sprintf(levelStr,"%d",level);
    outtextxy(LEVEL_X_POS,0,levelStr);
    outtextxy(230,0,"INVADER BY ADRIAN PILKINGTON (YOUTUBE:BYTEFOREVER");


    while (c != 'q')
    {
        c = getch_noblock();
        oldx = x;
        oldy = y;
        if (c != -1)
        {
          switch (c)
          {
            case 'o' : if (x > 8)
                       {
                        x = x - 8;
                        moved = 1;
                       }
                       break;
            case 'p' : if (x < maxx - 8)
                       {
                        x = x + 8;
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

        if (enemyValid[0] == 1)
        {
            drawSprite_slow(enemyx, enemyy,
                        enemySpriteData+enemySpriteOffset,
                        8,8, WHITE,1);


           if (enemyMove-- == 0)
           {
               enemySpriteOffset = 8 - enemySpriteOffset;
               drawSprite_slow(enemyx, enemyy, blankSpriteData, 8,8, 0,1);
               enemyx = enemyx + (enemySpeed * enemyxDirection);
               drawSprite_slow(enemyx, enemyy,
                            enemySpriteData+enemySpriteOffset,
                            8,8, WHITE,1);
               enemyMove = 10;
               if ((enemyx >= maxx - 40) || (enemyx <= 40))
               {
                  drawSprite_slow(enemyx, enemyy, blankSpriteData, 8,8, 0,1);
                  enemyxDirection = -enemyxDirection;
                  enemyy = enemyy + 8;
               }
           }
        }

        if (moved == 1)
        {
            drawSprite_slow(oldx, oldy, blankSpriteData, 8,8, 0,1);
            drawSprite_slow(x, y, spriteData, 8,8, colour,1);
            moved = 0;
            if (missileInFlight == 0)
               missilex = x;
        }

        // initialised missile
        if ((missileFired == 1) && (missileInFlight == 0))
        {
            missilex = x;
            missiley = y-24;
            missileFired = 0;
            drawSprite_slow(oldx, oldy, blankSpriteData, 8,8, 0,1);
            drawSprite_slow(x, y, spriteData, 8,8, colour,1);
        }

        if (missileInFlight == 1)
        {
            if (enemyValid[0] == 1)
            {
                if (((missilex == enemyx) ||
                    (missilex+1 == enemyx) ||
                    (missilex+2 == enemyx) ||
                    (missilex+3 == enemyx) ||
                    (missilex+4 == enemyx) ||
                    (missilex+5 == enemyx) ||
                    (missilex+6 == enemyx) ||
                    (missilex+7 == enemyx) ||
                    (missilex+8 == enemyx) ||
                    (missilex+9 == enemyx))
                    && ((missiley == enemyy) ||
                       (missiley+1 == enemyy) ||
                       (missiley+2 == enemyy) ||
                       (missiley+3 == enemyy) ||
                       (missiley+4 == enemyy) ||
                       (missiley+5 == enemyy) ||
                       (missiley+6 == enemyy)))

                {
                    drawSprite_slow(enemyx,enemyy, missileBang,8,8, RED,1);
                    //enemyValid[0] = 0; // todo : make work for all rows and enemys within

                    missileInFlight = 0;
                    setcolor(BLACK);
                    outtextxy(SCORE_X_POS,0,scoreStr);
                    score+=SCORE_INC;
                    sprintf(scoreStr,"%06d",score);
                    setcolor(WHITE);
                    outtextxy(SCORE_X_POS,0,scoreStr);
                    setcolor(BLACK);
                    outtextxy(LEVEL_X_POS,0,levelStr);
                    level = level + 1;
                    sprintf(levelStr,"%d",level);
                    setcolor(WHITE);
                    outtextxy(LEVEL_X_POS,0,levelStr);


                    enemySpeed = enemySpeed + ENEMY_SPEED_INCREASE;
                    enemyx = 40;
                    enemyxDirection = 1;
                    enemyy = 40;

                }
                else
                {
                    drawSprite_slow(missilex,missiley, missile,6,8, YELLOW,1);
                }
            }
            missiley = missiley - 4;

            if (missiley < 16)
            {
               missileInFlight = 0;
               drawSprite_slow(missilex,missiley, blankSpriteData,6,8,0,1);
               drawSprite_slow(x, y, spriteData, 8,8, CYAN,1);
            }
        }
    }

     /* clean up */
    free(spriteData);
    free(blankSpriteData);
    free(missile);
    free(missileBang);
    free(enemySpriteData);

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




