QEMU=C:/Users/Danny/Desktop/qemu-1.6.0/qemu-system-i386.exe
CC=gcc
CXX=g++
RM=rm

all: tools
	$(CC) -x assembler-with-cpp vbr.s -c
	objcopy -O binary vbr.o vbr.bin
	$(CC) -x assembler-with-cpp kernelasm.s -c
	$(CC) -x assembler-with-cpp lowlevel.s -c
	$(CC) -x assembler-with-cpp midlevel.s -c
	$(CC) -c -Wall kernelc.c
	$(CC) -c -Wall console.c
	$(CC) -c -Wall kprintf.c
	$(CC) -c -Wall util.c
	$(CC) -c -Wall interrupt.c
	$(CC) -c -Wall keyboard.c
	$(CC) -c -Wall disk.c
	ld -T linkerscript.txt kernelasm.o kernelc.o lowlevel.o midlevel.o -o kernel.tmp console.o interrupt.o kprintf.o util.o keyboard.o disk.o  
	objcopy -O binary kernel.tmp kernel.bin
	./mkdisk hd1.img 64 vbr.bin
	./mcp hd1.img kernel.bin KERNEL.BIN
	./mcp hd1.img kernelc.c KERNELC.C
	./mcp hd1.img console.c CONSOLE.C
	$(QEMU) -monitor vc:80Cx27C hd1.img
		
tools: mcp.exe mfat.exe mkdisk.exe mls.exe mrm.exe
mfat.exe: mfat
mcp.exe: mcp
mkdisk.exe: mkdisk
mrm.exe: mrm
mls.exe: mls

clean: 
	$(RM) *.o *.exe *.img
