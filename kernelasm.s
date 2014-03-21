.global entry
entry:

mov $_sbss,%ebx
mov $_ebss,%ecx
wiper:
	cmp %ebx,%ecx
	je endwiper
	movb $0,(%ebx)
	inc %ebx
	jmp wiper
endwiper:
	#ifdef WIN32
	call _kmain
	#else
	call kmain
	#endif
