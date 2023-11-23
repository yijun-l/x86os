[BITS 32]
[SECTION .text]

extern clock_handler
extern current

;=========================================================================
; IRQ 0 (Clock) ISR
;-------------------------------------------------------------------------
global clock_handler_entry
clock_handler_entry:

    cmp dword [current], 0
    je .call_clock_handler

;=========================================================================
; Store the context of current task
;-------------------------------------------------------------------------
.store_context:
    push eax
    mov eax, [current]
    mov dword [eax + 11 * 4], ecx
    mov dword [eax + 12 * 4], edx
    mov dword [eax + 13 * 4], ebx
    mov dword [eax + 15 * 4], ebp
    mov dword [eax + 16 * 4], esi
    mov dword [eax + 17 * 4], edi

    push ecx
    ; stack layout
    ; ecx | eax | eip | cs | eflags || esp
    ;  0  |  4  |  8  | 12 |   16   || 20

    ; eip
    mov ecx, [esp + 8]
    mov dword [eax + 8 * 4], ecx

    ; eflags
    mov ecx, [esp + 16]
    mov dword [eax + 9 * 4], ecx

    ; esp0
    mov ecx, esp
    add ecx, 20
    mov dword [eax + 1 * 4], ecx
    mov dword [eax + 14 * 4], ecx

    ; eax
    mov ecx, [esp + 4]
    mov dword [eax + 4 * 10], ecx

    pop ecx
    pop eax

.call_clock_handler:
    call clock_handler
    iret