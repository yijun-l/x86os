[BITS 32]
[SECTION .text]

extern clock_handler

;=========================================================================
; IRQ 0 (Clock) ISR
;-------------------------------------------------------------------------
global clock_handler_entry
clock_handler_entry:
    call clock_handler
    iret