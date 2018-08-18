global loader				   ; the entry symbol for ELF

section .text:				  ; start of the text (code) section

loader:						 
	mov eax, 0xCAFEBABE		 ; place the number 0xCAFEBABE in the register eax

.loop:
	jmp .loop				   ; loop forever
