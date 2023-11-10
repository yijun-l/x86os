[BITS 32]
[SECTION .text]

extern printk
extern keymap_handler

;=========================================================================
; default ISR
;-------------------------------------------------------------------------
global interrupt_handler_entry
interrupt_handler_entry:
    push msg
    call printk
    add esp, 4

    iret

;=========================================================================
; IRQ 1 (Keyboard) ISR
;-------------------------------------------------------------------------
global keymap_handler_entry
keymap_handler_entry:
    call keymap_handler
    iret

msg:
    db "interrupt handled", 10, 0