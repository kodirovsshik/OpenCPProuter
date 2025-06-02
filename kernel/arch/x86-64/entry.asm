BITS 64
	
global kernel_entry

extern main_thread_stack_top
	


section .entry_text

kernel_entry:
	cli
	lea rsp, [rel main_thread_stack_top]
	mov rbx, rdi

	lea rdi, [rel .data]
	call serial_puts

	mov rdi, rbx
	call serial_puts

	jmp hlt
.data:
	db "OpenCPProuter kernel!", 0






;;; rdi = string
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
