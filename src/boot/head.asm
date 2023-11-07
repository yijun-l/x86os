[SECTION .text]
[BITS 32]
extern kernel32_entry

global _start
_start:
;=========================================================================
; 8259 PIC initialization
;-------------------------------------------------------------------------
.config_8259a:
    mov al, 0x11        ; ICW1 - IC4 bit set
    out 0x20, al        ; Master PIC - command port

    out 0xa0, al        ; Slave PIC - command port

    mov al, 0x20        ; ICW2 - IRQ0 (to IRQ7) starts at IDT 0x20
    out 0x21, al        ; Master PIC - data port

    mov al, 0x28        ; ICW2 - IRQ8 (to IRQ15) starts at IDT 0x28
    out 0xa1, al        ; Slave PIC - data port

    mov al, 0x04        ; ICW3 - IRQ2 connected to Slave PIC
    out 0x21, al

    mov al, 0x02        ; ICW3 - ID1 set
    out 0xa1, al

    mov al, 0x03        ; ICW4 - Auto EOI
    out 0x21, al

    out 0xa1, al

.set_mask:
    mov al, 0b11111111
    out 0x21, al

    mov al, 0b11111111
    out 0xa1, al


    call kernel32_entry
    jmp stuck_loop

;=========================================================================
; Halt the CPU
;-------------------------------------------------------------------------

stuck_loop:
    hlt
    jmp stuck_loop