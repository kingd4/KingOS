#include "console.h"
#include "util.h"

int row = 0;
int col = 0;
static short cursor = 0;

void console_putc(char c)
{
    char *text = (char *)(0xb8000+(160*row)+(2*col));
    int cc = 0;



    if(c == 9) //tab
    {
        for(cc = 0;cc < 8;cc++)
        {
            if(col < (8*cc))
            {
                col = 8*cc;
                break;
            }
        }
    }

    else if(c == 10) //new line
    {
        if(row == 24)
        {
            kmemcpy((char*)0xb8000, (char*)(0xb8000+160), 80*24*2);
            char *erase = (char *)(0xb8000+(row*160));
            for(cc = 0;cc < 80; cc++)
            {
                *erase = ' ';
                erase++;
                *erase = console_get_color(BLACK, WHITE);
                erase++;
            }
            //row=23;
        }
        else
        {
            row++;
        }
        col = 0;
    }
    else if(c == 127)
    {
    	if(col == 1)
    	{
    		char *erase = (char *)(0xb8000+(row*160));
    		*erase = ' ';
    		row--;
    		col = 159;
    	}
    	else
    	{
    		char *erase = (char *)(0xb8000+(row*160)+((col*2)-2));
    		*erase = ' ';
    		col--;
    	}
    }
    else
    {
        *text = c;
        text++;
        *text = console_get_color(BLACK, WHITE);
        ++text;
        ++col;
    }

    if(col >= 80)
    {
        if(row == 24)
        {
            kmemcpy((char*)0xb8000, (char*)(0xb8000+160), 80*24*2);
            char *erase = (char *)(0xb8000+(row*160));
            for(cc = 0;cc < 80; cc++)
            {
                *erase = ' ';
                ++erase;
                *erase = console_get_color(BLACK, WHITE);
                ++erase;
            }
            row=23;
        }
        ++row;
        col = 0;
    }

	cursor = row*80 + col;
	outb(15,0x3d4);
	outb(cursor&0x00ff,0x3d5);
	outb(14,0x3d4);
	outb(cursor>>8,0x3d5);


}

//kmemcpy( (char*)0xb8000, (char*)(0xb8000+160), 80*24*2);

void console_init()
{
	console_clear();

	cursor = row*80 + col;
	outb(15,0x3d4);
	outb(cursor&0x00ff,0x3d5);
	outb(14,0x3d4);
	outb(cursor>>8,0x3d5);

}

void console_clear()
{
	char * memloc = (char*) 0xb8000;

	int i;

	for(i = 0; i < (0xb8000 + (80*27*2)); i++ )
	{
		*memloc++ = ' ';
		char x = console_get_color(BLACK, WHITE);
		*memloc++ = x;
	}
}

char console_get_color(Color fg, Color bg)
{
	int f = (int)fg;
	int b = (int)bg;
	char x = (b << 4)|f;

	return x;

}

void set_screen_location(int r, int c)
{
    row = r;
    col = c;

	cursor = row*80 + col;
	outb(15,0x3d4);
	outb(cursor&0x00ff,0x3d5);
	outb(14,0x3d4);
	outb(cursor>>8,0x3d5);

}

