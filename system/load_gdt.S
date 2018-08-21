; Function to load the gdt then flush all segment registers

global load_gdt        ; make the label visible outside this file

; refresh_segs - refresh all segment registers
; stack: [esp + 4] the address of gdt_ptr
;        [esp    ] return address
;
; gdt:   0x08  the null descriptor
;        0x10  the first descriptor (kernel privl, code segment (exec), read allowed)
;        ...  update as we add GDT entries

load_gdt:
	mov eax, [esp + 4]
	lgdt [eax]
	jmp 0x8:.segFlush

.segFlush:
	mov ax, 0x10
	mov dx, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	ret
