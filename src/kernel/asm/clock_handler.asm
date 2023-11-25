[BITS 32]
[SECTION .text]

extern clock_handler
extern current
extern task_esp0
extern task_ebp0
extern task_ss0

;=========================================================================
; IRQ 0 (Clock) ISR
;-------------------------------------------------------------------------
global clock_handler_entry
clock_handler_entry:
    push ecx

    mov ecx, [current]
    cmp ecx, 0
    je .pop_ecx  ; check if this is the 1st task

;=========================================================================
; Store the context of current task
;-------------------------------------------------------------------------
.store_context:

    ; stack layout - | ecx | eip | cs | eflags || esp |

    mov [ecx + 10 * 4], eax
    mov [ecx + 12 * 4], edx
    mov [ecx + 13 * 4], ebx
    mov [ecx + 15 * 4], ebp
    mov [ecx + 16 * 4], esi
    mov [ecx + 17 * 4], edi


    mov eax, ecx
    pop ecx
    ; stack layout - | eip | cs | eflags || esp |
    mov [eax + 11 * 4], ecx

    ; eip
    mov  ecx, [esp]
    mov [eax + 8 * 4], ecx

    ; cs
    mov  ecx, [esp + 4]
    mov [eax + 19 * 4], ecx

    ; eflags
    mov  ecx, [esp + 8]
    mov [eax + 9 * 4], ecx

    ; esp
    mov edx, esp
    add edx, 12
    mov [eax + 14 * 4], edx

    ; restore kernel stack
    mov esp, dword [task_esp0]
    mov ebp, dword [task_ebp0]
    mov ss, word [task_ss0]

    jmp .call_clock_handler

.pop_ecx:
    pop ecx

.call_clock_handler:
    call clock_handler
    iret
