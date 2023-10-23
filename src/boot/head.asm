[SECTION .text]
[BITS 32]
extern kernel32_entry

global _start
_start:
    call kernel32_entry
    jmp stuck_loop

;=========================================================================
; Halt the CPU
;-------------------------------------------------------------------------

stuck_loop:
    hlt
    jmp stuck_loop