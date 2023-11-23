[BITS 32]
[SECTION .text]

extern current
extern get_task_function

;=========================================================================
; Switch task from Current to Next
;-------------------------------------------------------------------------
global switch_task
switch_task:
    push eax
    mov eax, [current]
    cmp dword [eax + 1 * 4], 0  ; compare esp0 and 0
    je .initialize_task

.switch_to_exist_task:
    mov ecx, [eax + 11 * 4]
    mov edx, [eax + 12 * 4]
    mov ebx, [eax + 13 * 4]
    mov esp, [eax + 14 * 4]
    mov ebp, [eax + 15 * 4]
    mov esi, [eax + 16 * 4]
    mov edi, [eax + 17 * 4]

    push ebx
    mov ebx, eax
    mov eax, [eax + 10 * 4]
    mov ebx, [ebx + 8 * 4]
    mov dword [save_eip], ebx
    pop ebx

    sti
    jmp dword [save_eip]

.initialize_task:
    pop eax
    push dword [current]
    call get_task_function
    add esp, 4
    mov ebp, [eax + 4]
    mov esp, [eax + 4]

    sti
    call [eax]

    ret

[SECTION .data]
save_eip dd 0