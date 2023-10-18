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
; intialize register and read HDD
;-------------------------------------------------------------------------

    mov di, SETUP_ADDR_BASE
    mov ecx, SETUP_SECTOR_START
    mov bx, SETUP_SECTOR_COUNT
	
    call read_hd

;=========================================================================
; load OS and Print msg when successfully read
;-------------------------------------------------------------------------

    mov si, read_hd_success_msg
    call print

    jmp SETUP_ADDR_BASE

;=========================================================================
; read_hd - read HDD to RAM
; Input:
;	DI - memory address to load from hdd
;	CX - the sector number read from
;	BX - number of sectors to be read
; Output:
;	none
;-------------------------------------------------------------------------

read_hd:
    mov dx, 0x1f2		; 0x1f2(W): Number of sectors to read/write
    mov al, bl
    out dx, al

    inc dx				; 0x1f3(W): LBAlo
    mov al, cl
    out dx, al

    inc dx				; 0x1f4(W): LBAmid
    mov al, ch
    out dx, al

    inc dx				; 0x1f5(W): LBAhi
    shr ecx, 16
    mov al, cl
    out dx, al

    inc dx				; 0x1f6(W): Drive / Head
    and ch, 0b0000_1111
    or ch, 0b1110_0000
    mov al, ch
    out dx, al

    inc dx				; 0x1f7(W): send ATA commands to the device
    mov al, 0x20
    out dx, al
    mov cx, bx

.start_read:			; Read CX sectors
    push cx
    call wait_hd_prepare
    call read_hd_data
    pop cx
    loop .start_read

.done:
    ret

;=========================================================================
; Wait HDD is ready to be read
;-------------------------------------------------------------------------

wait_hd_prepare:
    mov dx, 0x1f7

.check:
    in al, dx			; 0x1f7(R) - read the current HDD status
    and al, 0b1000_1000	; Check BSY and DRQ bit
    cmp al, 0b0000_1000 ; Data is read when BSY not set and DRQ set
    jnz .check
	
.done:	
    ret

;=========================================================================
; Read 1 sector (512 bytes)
;-------------------------------------------------------------------------

read_hd_data:
    mov cx, 256			; 512 bytes as total, 2 bytes per time
    mov dx, 0x1f0

.read:
    in ax, dx			; 0x1f0(R) - Read data bytes.
    mov [edi], ax
    add edi, 2
    loop .read

.done:
    ret
	
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

read_hd_success_msg:
    db "successfully read disk", 0xa, 0xd, 0

times 510 - ($ - $$) db 0	; padding
db 0x55, 0xaa		    	; Boot Sector Signature
