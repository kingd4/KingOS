#include "util.h"
#include "console.h"
#include "interrupt.h"
#include "kprintf.h"
#include "disk.h"

extern int row;
extern int col;

void kmemcpy(char * d, char * s, int c)
{
	char *dp = (char*)d;
	char *sp = (char*)s;
	int i;
	for(i=0;i<c;i++,dp++,sp++)
	{
		*dp = *sp;
	}
}

unsigned char inb(unsigned short p)
{
	int v;
	asm volatile("inb %%dx,%%al": "=a"(v):"d"(p));
	return (unsigned char) v;
}

unsigned short inw(unsigned short p)
{
	int v;
	asm volatile("inw %%dx,%%ax": "=a"(v):"d"(p));
	return (unsigned short) v;
}

void time(unsigned tmp)
{
	int rr = row;
	int cc = col;
	set_screen_location(0,68);
	unsigned h,m,s,f;

	h = tmp/128/60/60%60;
	m = tmp/128/60%60;
	s = tmp/128%60;
	f = tmp*1000/128;
	f = f % 1000;

	kprintf("%02d:%02d:%.02d.%03d",h,m,s,f);

	set_screen_location(rr,cc);

	return;
}

int stringcmp(char * a, int b, char * c, int d, int e) // array 1, start index, array 2, start index, places
{
int i;

	for(i = 0; i < e; i++)
	{
		if(a[i+b] != c[i+d])
		{
			return -1;
		}
	}
	return 0;
}


void halt(void)
{
	asm volatile("hlt");
}
