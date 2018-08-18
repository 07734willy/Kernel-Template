global loader				   ; the entry symbol for ELF

; Multiboot header defines
MAGIC_NUMBER 	equ 0x1BADB002
FLAGS			equ 0x0			; Relevant FLAGS we might need to set:
								; bit	description
								; 0		set if the kernel must be 4KB aligned
								; 1		activate memory info
								; 2		activate video info
CHECKSUM		equ -(MAGIC_NUMBER+FLAGS)

; Other defines
KERNEL_STACK_SIZE equ 4096



section .text					; start of the text (code) section
	dd MAGIC_NUMBER
	dd FLAGS
	dd CHECKSUM

loader:						 
	mov esp, kernel_stack + KERNEL_STACK_SIZE   ; point esp to the start of the
												; stack (end of memory area)
	
	; Call the kernel main C function using cdecl
	extern kmain
	call kmain

	; Halt the system when kmain returns
loop:
	jmp loop



section .bss
align 4
kernel_stack:					
	resb KERNEL_STACK_SIZE		; reserve stack for the kernel

