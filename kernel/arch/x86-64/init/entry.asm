BITS 64
	
global halt
global kernel_entry
global serial_puts
global init_get_entropy16
global init_switch_to_virtual_addresses	

%define kernel_init _ZN4init11kernel_initERKNS_13kernel_args_tE
	
extern _main_thread_stack_top
extern kernel_init


section .init_entry

kernel_entry:
;;; push rdi
;;; int3
.post_int3:
;;; pop rdi
	cli
	cld

	call print_kernel_base

	xor rax, rax
	
	lea rsp, [rel _main_thread_stack_top]
	mov rbp, rax

	times 2 push rax
	jmp kernel_init

.data:
	db "OpenCPProuter kernel", 0




	
section .init


print_kernel_base:
	lea rax, [rel kernel_entry]
.loop1_start:
	mov ecx, 16
.loop1:
	mov edx, eax
	and dl, 15
	cmp dl, 10
	jae .loop1_add_hex
.loop1_add_decimal:
	add dl, '0'
	jmp .loop1_tail
.loop1_add_hex:
	add dl, 'A' - 10
.loop1_tail:
	push dx
	shr rax, 4
	loop .loop1
.loop2_start:
	mov ecx, 16
	mov dx, 0x3F8
.loop2:
	pop ax
	out dx, al
	loop .loop2
.ret:
	mov al, `\n`
	out dx, al
	mov al, `\r`
	out dx, al
	ret


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



halt:	
	cli
	mov al, 0x80
	out 0x70, al
.hlt:
	hlt
	jmp .hlt
