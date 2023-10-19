[ORG  0x500]

SEG_BASE		equ		0
SEG_LIMIT		equ 	0xfffff
CODE_SELECTOR	equ		(1 << 3)
DATA_SELECTOR	equ		(2 << 3)

[SECTION .text]
[BITS 16]
global _start

_start:
;=========================================================================
; Real Mode
;-------------------------------------------------------------------------
    mov ax, 0
    mov ss, ax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov si, ax

    mov si, msg_step_in_protected_mode
    call print
	
	cli						; disable interrupts - set IF flag to 0
    lgdt  [gdtr_val]		; load GDTR register value

    in    al,  0x92
    or    al,  2
    out   0x92, al			; open A20 by Fast A20 Gate

    mov   eax, cr0
    or    eax , 1			; set PE (Protection Enable) bit in CR0 (Control Register 0)
    mov   cr0, eax

    jmp CODE_SELECTOR:protected_mode

;=========================================================================
; print - print string to display
; Input:
;       SI - string address
; Output:
;       none
;-------------------------------------------------------------------------

print:
    mov ah, 0x0e            ; INT 0x10 mode - Teletype output
    mov bh, 0               ; BH = Page Number
    mov bl, 1               ; BL = Color

.loop:
    mov al, [si]            ; AL = Character
    cmp al, 0
    jz .done
    int 0x10                ; IVT 0x10 - Video Services

    inc si
    jmp .loop

.done:
    ret

;=========================================================================
; Protected Mode
;-------------------------------------------------------------------------

[BITS 32]
protected_mode:
    xchg bx, bx             ; Magic Breakpoint

    mov ax, DATA_SELECTOR
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov esp, 0x9fbff		; set kernel stack, 0x7e00 - 0x9fbff is free to use

	jmp stuck_loop

;=========================================================================
; Halt the CPU
;-------------------------------------------------------------------------

stuck_loop:
    hlt
    jmp stuck_loop

;=========================================================================
; GDT table
;-------------------------------------------------------------------------

gdt_base:					; Entry 0 - NULL Segmenet Descriptor
	dd 0, 0

gdt_code:					; Entry 1 - Code Segement Descriptor
	dw SEG_LIMIT & 0xffff
	dw SEG_BASE & 0xffff
	db (SEG_BASE >> 16) & 0xff
	db 0b1_00_1_1_0_0_0
	db ((SEG_LIMIT >> 16) & 0xf) | 0b0_1_0_0_0000
	db SEG_BASE >> 24 & 0xff

gdt_data:					; Entry 2 - Data Segement Descriptor
	dw SEG_LIMIT & 0xffff
	dw SEG_BASE & 0xffff
	db (SEG_BASE >> 16) & 0xff
	db 0b1_00_1_0_0_1_0
	db ((SEG_LIMIT >> 16) & 0xf) | 0b1_1_0_0_0000
	db SEG_BASE >> 24 & 0xff

;=========================================================================
; GDTR Register content
;-------------------------------------------------------------------------

gdtr_val:
	dw $ - gdt_base - 1
	dd gdt_base
;=========================================================================
; String definition
;-------------------------------------------------------------------------

msg_step_in_protected_mode:
    db "Switching to Protected Mode", 0xa, 0xd, 0	

