
global kernel_entry

section .entry_text
BITS 64


kernel_entry:
	mov dx, 0x3F8
	mov rsi, .data
.loop:
	lodsb
	jz hlt
	out dx, al
	jmp .loop

.data:
	db "OpenCPProuter!", 0
	db "unreachable"

hlt:	
	cli
	hlt
	jmp hlt
