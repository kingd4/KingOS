#include "keyboard.h"
#include "util.h"
#include "console.h"

unsigned int scantable[59] = {0,27,'1','2','3','4','5','6','7','8'
					,'9','0','-','=',127,'\t','q','w','e','r'
					,'t','y','u','i','o','p','[',']','\n',LCTRL
					,'a','s','d','f','g','h','j','k','l',';'
					,96,39,LSHIFT,92,'z','x','c','v','b','n'
					,'m',',','.','/',RSHIFT,PRNTSCREEN,LALT,' ',CAPLK};

unsigned int sscantable[59] = {0,27,'!','@','#','$','%','^','&','*'
				    ,'(',')','_','+',127,'\t','Q','W','E','R'
				    ,'T','Y','U','I','O','P','{','}','\n',LCTRL
					,'A','S','D','F','G','H','J','K','L',':'
					,'"','~',LSHIFT,'|','Z','X','C','V','B','N'
					,'M','<','>','?',RSHIFT,PRNTSCREEN,LALT,' ',CAPLK};

unsigned int cscantable[59] = {0,27,'1','2','3','4','5','6','7','8'
					,'9','0','-','=',127,'\t','Q','W','E','R'
					,'T','Y','U','I','O','P','[',']','\n',LCTRL
					,'A','S','D','F','G','H','J','K','L',';'
					,96,39,LSHIFT,92,'Z','X','C','V','B','N'
					,'M',',','.','/',RSHIFT,PRNTSCREEN,LALT,' ',CAPLK};


#define linebuf_size 49
static int shift;
char linebuf[linebuf_size];
static int ext;
static int capslock;

static int linecount = 0;
volatile int bufready = 0;

void keyboard_interrupt()
{

	int q = inb(0x60);

	int pressed = ((q & 0x80)? 0:1);

	if(q == 0xe0)
	{
		ext = 1;
		return;
	}

	if(ext == 1)
	{
		ext = 0;
		return;
	}

	q = q & 0x7f;
	int ch;

	if(shift)
	{
		ch = sscantable[q];
	}
	else if(capslock)
	{
		ch = cscantable[q];
	}
	else
	{
		ch = scantable[q];
	}

	if(pressed == 0)
	{
		if( ch == LSHIFT || ch == RSHIFT)
		{
			shift = 0;
			return;
		}
		else if(ch == CAPLK)
		{
			if(capslock == 1)
			{
				capslock = 0;
				return;
			}
			else if(capslock == 0)
			{
				capslock = 1;
				return;
			}
		}

		if(!bufready)
		{
			if(ch == 127)
			{
				if(linecount > 0)
				{
					linecount--;
					console_putc(127);
				}
			}
			else if (ch == '\n')
			{
				console_putc('\n');
				bufready = 1;
			}
			else if((ch == LCTRL) || (ch == LALT))
			{

			}
			else if(ch != (0 || 27 || PRNTSCREEN))
			{
				if(linecount < linebuf_size)
				{
					linebuf[linecount] = ch;
					linecount++;
					console_putc(ch);
				}
			}
		}
	}
	else if( pressed == 1)
	{
		if( ch == LSHIFT || ch == RSHIFT)
		{
			shift = 1;
			return;
		}
	}
}

int keyboard_getline(char * q, unsigned n)
{
	while(!bufready)
	{
		halt();
	}


	if(n > linecount + 1)
	{
		n = linecount + 1;
	}
	q[n-1] = 0;
	kmemcpy(q,linebuf,n-1);


	int tmp = linecount;
	linecount = 0;
	bufready= 0;

	return tmp;
}
