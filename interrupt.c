#include "interrupt.h"
#include "kprintf.h"
#include "util.h"
#include "console.h"
#include "keyboard.h"


volatile unsigned jiffies;


void interrupt_init(void)
{
static struct IDTEntry itable[256];
extern unsigned lla;
unsigned *a = (unsigned*) &lla;

int i = 0;

	for(i = 0; i < 256; i++)
	{
		itable[i].flags_0x8e = 0x8e;
		itable[i].zero = 0;
		itable[i].sel_0x10 = 0x10;
		itable[i].addr_low = (unsigned) (a[i]);
		itable[i].addr_high = (unsigned) ((a[i]) >> 16);
	}

struct IDTLoc l;

	l.size = 256*8;
	l.addr = (unsigned) itable;

	asm volatile("lidt (%%eax)"::"a"(&l));

	outb(0x11,0x20);
	outb(32,0x21);
	outb(4,0x21);
	outb(1,0x21);
	outb(0,0x21);

	outb(0x11,0xa0);
	outb(40,0xa1);
	outb(2,0xa1);
	outb(1,0xa1);
	outb(0,0xa1);


	outb(0xa,0x70);
	unsigned char tmp = inb(0x71);
	outb(0xa,0x70);
	outb(9|(tmp&0xf0),0x71);

	outb(0xb,0x70);
	tmp = inb(0x71);
	outb(0xb,0x70);
	outb(tmp|0x40,0x71);
	asm volatile("sti");

	return;
}

void highlevel_handler(struct StateBlock * sb)
{
	//kprintf("Interrupt %d called!", sb->int_num);

	if(((sb->int_num) >= 32) && ((sb->int_num) <= 39))
	{
		if((sb->int_num) == 33)
		{
			keyboard_interrupt();
		}
		outb(32,0x20);
	}
	else if(((sb->int_num) >= 40) && ((sb->int_num) <= 47))
	{
		if(sb->int_num == 40)
		{
			jiffies++;
			time(jiffies);
		}
		outb(32,0x20);
		outb(32,0xa0);
		outb(0xc,0x70);
		inb(0x71);
	}
}


