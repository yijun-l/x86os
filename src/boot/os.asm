[ORG  0x500]

[SECTION .text]
[BITS 16]
global _start

_start:
    xchg bx, bx		    ; Magic Breakpoint
;=========================================================================
; Register initialization
;-------------------------------------------------------------------------
    mov ax, 0
    mov ss, ax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov si, ax

    mov si, msg
    call print

    jmp stuck_loop 

;=========================================================================
; Halt the CPU
;-------------------------------------------------------------------------

stuck_loop:
    hlt
    jmp stuck_loop 
	
;=========================================================================
; print - print string to display
; Input:
;	SI - string address
; Output:
;	none
;-------------------------------------------------------------------------

print:
    mov ah, 0x0e	    ; INT 0x10 mode - Teletype output
    mov bh, 0		    ; BH = Page Number
    mov bl, 1		    ; BL = Color

.loop:
    mov al, [si]	    ; AL = Character
    cmp al, 0
    jz .done
    int 0x10		    ; IVT 0x10 - Video Services

    inc si
    jmp .loop

.done:
    ret
	
;=========================================================================
; String definition
;-------------------------------------------------------------------------

msg:
    db "OS is loaded, and running now", 0xa, 0xd, 0
