[ORG 0x7c00]

SETUP_ADDR_BASE     equ     0x500
SETUP_SECTOR_START  equ     1	
SETUP_SECTOR_COUNT  equ     2

[SECTION .text]
[BITS 16]
global _start

_start:
    xchg bx, bx		    ; Magic Breakpoint

;=========================================================================
; Clear the screen
;-------------------------------------------------------------------------

    mov ah, 0x00	    ; Set Video Mode
    mov al, 0x03	    ; Text Mode, 80*25
    int 0x10		    ; IVT 0x10 - Video Services

;=========================================================================
; Read content from Storage Device to memory 
;-------------------------------------------------------------------------

    mov si, dap		    ; Disk Address Package (DAP) address
    mov ah, 0x42	    ; Extended Read Sectors From Drive
    mov dl, 0x80	    ; drive index (e.g. 1st HDD = 80h)
    int 0x13		    ; IVT 0v13: Floppy/IO routines

    JC .read_hd_failure	; CF Set On Error, Clear if No Error

;=========================================================================
; Print msg when successfully read
;-------------------------------------------------------------------------

    mov si, read_hd_success_msg
    call print

    jmp stuck_loop 

;=========================================================================
; Print msg when read failure
;-------------------------------------------------------------------------

.read_hd_failure
    mov si, read_hd_failure_msg
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
; Disk Address Packet (DAP) format
;
;       offset  size    description
;		-----	-----	-----
;       0       1 byte  size of DAP (set this to 10h)
;       1       1 byte  unused, should be zero
;       2-3     2 byte  number of sectors to be read
;       4-7     4 byte  segment:offset pointer to the memory buffer 
;			to which sectors will be transferred
;       8-F     8 byte  absolute number of the start of the sectors 
;			to be read using logical block addressing
;-------------------------------------------------------------------------

dap:
    db 0x10
    db 0
    dw SETUP_SECTOR_COUNT
    dd SETUP_ADDR_BASE
    dq SETUP_SECTOR_START

;=========================================================================
; String definition
;-------------------------------------------------------------------------

read_hd_failure_msg:
    db "read disk failed", 0xa, 0xd, 0

read_hd_success_msg:
    db "successfully read disk", 0xa, 0xd, 0

;=========================================================================
; padding
;-------------------------------------------------------------------------

times 510 - ($ - $$) db 0
db 0x55, 0xaa		    ; Boot Sector Signature
