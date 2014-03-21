#ifndef INTERRUPT_H_
#define INTERRUPT_H_


#pragma pack(push,1)
struct IDTLoc
{
	unsigned short size;
	unsigned int addr;
};
#pragma pack(pop)

#pragma pack(push,1)
struct IDTEntry
{
	unsigned short addr_low;
	unsigned short sel_0x10;
	unsigned char zero;
	unsigned char flags_0x8e;
	unsigned short addr_high;
};
#pragma pack(pop)

#pragma pack(push,1)
struct StateBlock
{
	unsigned edi, esi, ebp, esp, edx, ecx, ebx, eax;
	unsigned ss, gs, fs, es, ds;
	unsigned int_num, errcode, eip, cs, eflag;
};
#pragma pack(pop)

void interrupt_init();
void highlevel_handler(struct StateBlock * sb);


#endif

