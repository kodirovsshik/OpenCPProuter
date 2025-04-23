section .text
BITS 64

global kernel_entry

kernel_entry:
	mov al, 'x'
	mov dx, 0x3F8
	out dx, al
	jmp hlt
	

hlt:
	hlt
	jmp hlt
