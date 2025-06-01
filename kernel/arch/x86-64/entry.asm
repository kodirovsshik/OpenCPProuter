
global kernel_entry

section .entry_text
BITS 64


kernel_entry:
	mov dx, 0x3F8
	lea rsi, [rel .data]
	cld
.loop:
	lodsb
	test al, al
	jz hlt
	out dx, al
	jmp .loop

.data:
	db "OpenCPProuter kernel!", 0

hlt:	
	cli
.hlt:
	hlt
	jmp .hlt
