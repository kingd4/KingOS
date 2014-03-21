#ifndef UTIL_H_
#define UTIL_H_


#define outb(v,p) asm volatile("outb %%al,%%dx" :: "a"(v),"d"(p))
#define outw(v,p) asm volatile("outw %%ax,%%dx" :: "a"(v),"d"(p))

unsigned char inb(unsigned short p);
unsigned short inw(unsigned short p);

void kmemcpy(char * d, char * s, int c);

void time(unsigned tmp);

int stringcmp(char * a,int b, char *c,int d,int e);

void halt(void);

#endif
