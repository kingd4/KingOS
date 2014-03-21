.global midlevel_handler
.global _midlevel_handler
_midlevel_handler:
midlevel_handler:

	push %ds
	push %es
	push %fs
	push %gs
	push %ss
	push %eax
	push %ebx
	push %ecx
	push %edx
	push %esp
	push %ebp
	push %esi
	push %edi
	push %esp

#ifdef WIN32
	call _highlevel_handler
#else
	call highlevel_handler

#endif

	pop %esp
	pop %edi
	pop %esi
	pop %ebp
	pop %esp
	pop %edx
	pop %ecx
	pop %ebx
	pop %eax
	pop %ss
	pop %gs
	pop %fs
	pop %es
	pop %ds

	add $8, %esp

	iret
