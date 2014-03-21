int main(){return 0;}
int _main(){return 0;}
int __main(){return 0;}
void _alloca(){}

#include "kprintf.h"
#include "console.h"
#include "util.h"
#include "interrupt.h"
#include "keyboard.h"
#include "disk.h"




void kmain(void)
{
unsigned char command = 1;
char x[50];


	console_init();
	interrupt_init();
	disk_init();

	while(1)
	{


		if(command == 1)
		{
			kprintf("C:\>");
			command = 0;
		}

		keyboard_getline( x, 50);

		if(x[0] == 'l')
		{
			list_files();
			command = 1;
			kprintf("\n");
		}
		else if((x[0] == 'r') && (x[1] == ' '))
		{
			//kprintf("delete file");
			remove_file(x);

			command = 1;
			kprintf("\n");
		}
		else
		{
			kprintf("Invalid Command!");
			command = 1;
			kprintf("\n");
		}

	}
	halt();
}
