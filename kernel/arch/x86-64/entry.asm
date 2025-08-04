BITS 64
	
global kernel_entry
global serial_puts

%define kernel_remap _ZN4init12kernel_remapERKNS_13kernel_args_tE
	
extern _main_thread_stack_top
extern kernel_remap


section .init_entry

kernel_entry:
	cli
	xor rax, rax
	
	lea rsp, [rel _main_thread_stack_top]
	mov rbp, eax

	times 2 push rax
	jmp kernel_remap

.data:
	db "OpenCPProuter kernel", 0





section .data:
gdt_data:
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
.cpu_descriptor:
	dd gdt_data - $$
	dw .cpu_descriptor - gdt_data


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
.hlt:
	hlt
	jmp .hlt
