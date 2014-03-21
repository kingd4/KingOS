#include "disk.h"
#include "util.h"
#include "errno.h"
#include "console.h"
#include "kprintf.h"

static int busy();
static int ready();
static int error();
static void reset();

struct VBR* vbr = (struct VBR*) 0x7c00;

struct File file_table[MAX_FILES];
char aRead[512];
struct DirEntry * Dir_table = (struct DirEntry*) &aRead;


void disk_init()
{
unsigned start_sec = (vbr->first_sector) + 1 + (2 * (vbr->sectors_per_fat));

	disk_read_sectors(aRead,start_sec,1);

}

void list_files()
{
int i;
int b;
	for(i = 0; i<16;i++)
	{
		if((Dir_table[i].fname[0] == 0) || (Dir_table[i].fname[0] == -27))
		{
			continue;
		}
		for(b = 0; b < 8;b++)
		{
			console_putc(Dir_table[i].fname[b]);
		}

		kprintf(".");

		for(b = 0; b < 3;b++)
		{
			console_putc(Dir_table[i].ext[b]);
		}

		kprintf(" (%d)\n",Dir_table[i].size);
	}
}

void remove_file(char * x)
{
	int i;
	i = file_test(x);
	if(i >= 0)
	{
		Dir_table[i].fname[0] = -27;
		kprintf("file removed");
	}
	else
	{
		kprintf("file not found!");
	}
}

int file_test(char * x)
{
char bufa[10];
char bufb[3];

int b;
int a;
int c;
for(a = 0; a < 10; a++)
{

	if(x[a] == '.')
	{
		c = 1;
		b = a+1;
	}
	if(c == 1)
	{
		bufa[a] = ' ';
	}
	else
	{
		bufa[a] = x[a];
	}
}



for(a = 0; a < 3; a++)
{
	if(((x[b + a] > 65) && (x[b + a] < 90)))
	{
		bufb[a] = x[b + a];
	}
	else if(((x[b + a] > 97) && (x[b + a] < 122)))
	{
		bufb[a] = x[b + a];
	}
	else
	{
		bufb[a] = ' ';
	}

}

int i;

	for(i = 0; i < 16; i++)
	{
		if(stringcmp(bufa,2,Dir_table[i].fname,0,8) == 0)
		{
			if(stringcmp(bufb,0,Dir_table[i].ext,0,3) == 0)
			{
				return i;
			}
		}
	}
	return -1;
}




static int busy()
{
	int x = inb(0x1f7);
	return (x&0x80) ? 1:0;
}

static int ready()
{
	int x = inb(0x1f7);
	return (x&0x08) ? 1:0;
}

static int error()
{
	int x = inb(0x1f7);
	return (x&0x21) ? 1:0;
}

static void reset()
{
	outb(0x6,0x3f6);
	outb(0x2,0x3f6);
}

int disk_read_sectors(void* buf, unsigned sec, unsigned num)
{
unsigned short* p = (unsigned short*) buf;
int i; short s;
	while(num>0)
	{
		while(busy()) {}

		outb(0xe0,0x1f6);
		outb(0,0x1f1);
		outb(0x2,0x3f6);
		outb(1,0x1f2);
		outb(sec,0x1f3);
		outb((sec>>8),0x1f4);
		outb((sec>>16),0x1f5);
		outb(0xe0 | (sec>>24),0x1f6);
		outb(0x20,0x1f7);

		while(busy()) {}

		while( !ready() && !error() ) {}

		if( error() )
		{
			reset();
			return -EIO;
		}

		for(i=0;i<256;++i)
		{
			s=inw(0x1f0);
			*p=s; ++p;
		}
		--num;
	}
	return SUCCESS;
}

int disk_write_sectors(void* buf,int first_sector, int num)
{
	unsigned short* p = (unsigned short*) buf;
	int i;
	while(num>0)
	{
		while(busy()) {}

		outb(0xe0,0x1f6);
		outb(0,0x1f1);
		outb(0x2,0x3f6);
		outb(1,0x1f2);
		outb(first_sector,0x1f3);
		outb((first_sector>>8),0x1f4);
		outb((first_sector>>16),0x1f5);
		outb(0xe0 | (first_sector>>24),0x1f6);
		outb(0x30,0x1f7);

		while(busy()) {}

		while( !ready() && !error() ) {}

		if( error() )
		{
			reset();
			return -EIO;
		}

		for(i=0;i<256;++i)
		{
			outw(*p,0x1f0);
			p++;
		}
//flush cache
		outb(0xe7,0x1f7);
		while(busy()){ }
		if(error())
		{
			reset();
			return -EIO;
		}
		--num;
	}
	return SUCCESS;
}


