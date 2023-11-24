[BITS 32]
[SECTION .text]

extern get_task_function
extern current
extern inc_sched_times
extern task_exit
extern task_esp0
extern task_ss0

;=========================================================================
; Switch task from Current to Next
;-------------------------------------------------------------------------
global switch_task  ; void switch_task();
switch_task:
    mov ecx, [current]
    push ecx
    call inc_sched_times
    add esp, 4
    cmp eax, 0
    je .initialize_task

.restore_context:
    mov ecx, [current]

    mov eax, [ecx + 10 * 4]
    mov edx, [ecx + 12 * 4]
    mov ebx, [ecx + 13 * 4]
    mov esp, [ecx + 14 * 4]
    mov ebp, [ecx + 15 * 4]
    mov esi, [ecx + 16 * 4]
    mov edi, [ecx + 17 * 4]

    sti

    mov ecx, [ecx + 8 * 4]

    jmp ecx



.initialize_task:
    push ebp
    ; save esp0 and ss0
    cmp dword [task_esp0], 0
    jne .load_esp0
    mov dword [task_esp0], esp
    mov word [task_ss0], ss

.load_esp0:
    ; save kernel stack to TSS esp0
    mov eax, [current]
    mov [eax + 1 * 4], esp
    mov [eax + 2 * 4], ss
    sub esp, 0x10

    ; load task statck
    mov esp, [eax + 14 * 4]
    mov ebp, [eax + 15 * 4]

    ; load task function
    push dword [current]
    call get_task_function
    add esp, 4

    sti
    call eax

.end:
    ; restore kernel stack
    mov ecx, [current]
    mov esp, dword [ecx + 1 * 4]
    mov ss, word [ecx + 2 * 4]
    pop ebp

    ; release task
    push ecx
    call task_exit
    add esp, 4

    ret
