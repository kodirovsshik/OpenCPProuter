BITS 64
	
global kernel_entry
global serial_puts
global init_get_entropy16
global init_switch_to_virtual_addresses	

%define kernel_remap _ZN4init12kernel_remapERKNS_13kernel_args_tE
	
extern _main_thread_stack_top
extern kernel_remap


section .init_entry

kernel_entry:
	cli
	cld
	xor rax, rax
	
	lea rsp, [rel _main_thread_stack_top]
	mov rbp, rax

	times 2 push rax
	jmp kernel_remap

.data:
	db "OpenCPProuter kernel", 0




	
section .init

	
init_switch_to_virtual_addresses:
	pop rax
	add rax, rdi

	push qword gdt.code64_kernel - gdt
	push rax

	lea rax, [rel gdt]
	lea rdx, [rel gdt.cpu_data]
	mov [rdx + 2], rax
	lgdt [rdx]

	retf


	
init_get_entropy16:
	lea rsi, [rel .arr]
.loop:
	lodsb
	test al, al
	js .ret
	out 0x70, al
	in al, 0x71
	stosb
	jmp .loop
.ret:
	xor al, al
	stosb
	ret
.arr:
	db 0x10, 0x15, 0x16, 0x17, 0x18, 0x30, 0x31, 0x00, 0x02, 0x04, 0x06, 0x07, 0x08, 0x09, 0x32, -1



	

section .rodata

gdt:
.null:
	dq 0
.code64_kernel:
	dw 0xFFFF
	dw 0
	db 0
	db 0b10011000
	db 0b10101111
	db 0
.data64_kernel:
	dw 0xFFFF
	dw 0
	db 0
	db 0b10010010
	db 0b10101111
	db 0
.cpu_data:
	dw gdt.cpu_data - gdt
	dq 0 




	
section .text

; rdi = string
serial_puts:
	mov dx, 0x3F8
	cld

.loop:
	mov al, [rdi]

	test al, al
	jz .ret

	out dx, al

	inc rdi
	jmp .loop
.ret:
	mov al, 13
	out dx, al
	mov al, 10
	out dx, al
	ret



hlt:	
	cli
	mov al, 0x80
	out 0x70, al
.hlt:
	hlt
	jmp .hlt
