	jmp x
.rep 64
	nop
.endr
x:

	//edi = location in memory to write too.
	//esi = loop count for number of sectors to read into memory.
	//ecx = sector number to read from.

	call controlcheck
	call rootloc
	call controlsetup
	call controlcheck
	call diskcheck

	mov $0x7e00,%edi

	call readdisk
	call controlcheck
	call kernelloc

sectloop:

	call controlsetup
	call controlcheck
	call diskcheck
	call readdisk

	add $1, %ecx
	dec %esi
	cmp $0, %esi
	jne sectloop

	mov $0x7e00, %eax
	jmp *%eax

	hlt

kernelloc:

	mov (0x7c00+28), %ecx
	and $0x00ffffff, %ecx
	add $1, %ecx
	mov (0x7c00+22), %ebx
	and $0x0000ffff, %ebx
	add %ebx, %ecx
	add %ebx, %ecx

	and $0x0fffffff, %ecx
	or $0xe0000000, %ecx

	mov (0x7e00+28), %esi
	and $0x00ffffff, %esi
	inc %esi

	movw (0x7c11), %ax
    shr $4, %eax

    add %eax, %ecx

    mov $0x7e00, %edi

	ret

rootloc:

	mov (0x7c00+28), %ecx
	and $0x00ffffff, %ecx
	add $1, %ecx
	mov (0x7c00+22), %ebx
	and $0x0000ffff, %ebx
	add %ebx, %ecx
	add %ebx, %ecx

	and $0x0fffffff, %ecx
	or $0xe0000000, %ecx

	ret

diskcheck:

	mov $0x00000000, %eax
	mov $0x1f7,%dx
	mov $0x20,%al
	out %al,%dx

disknotready:

	in %dx,%al
	test $0x08,%al
	jz disknotready

	ret

controlsetup:

	mov $0x3f6,%dx
	mov $2,%al
	out %al,%dx

	mov $0x1f2,%dx
	mov	$1,%al
	out %al,%dx

	mov %ecx, %eax

	mov $0x1f3,%dx
	out %al,%dx

	shr $8,%eax

	mov $0x1f4,%dx
	out %al,%dx

	shr $8,%eax

	mov $0x1f5,%dx
	out %al,%dx

	shr $8,%eax

	mov $0x1f6,%dx
	out %al,%dx

	ret

readdisk:

	push %ecx

	mov $256,%ecx
	mov $0x1f0,%dx

readloop:

	in %dx,%ax
	mov %ax,(%edi)
	add $2,%edi
	sub $1,%ecx
	cmp $0,%ecx
	jnz readloop

	pop %ecx

	ret

clearscreen:

	mov $0xb8000,%eax

screenloop:

	movb $' ',(%eax)
	add $2, %eax
	cmp $0xb8f9f,%eax
	jl screenloop

	ret

controlcheck:

	mov $0x1f7,%dx

notready:

	in %dx,%al
	test $0x80,%al
	jnz notready

	ret
