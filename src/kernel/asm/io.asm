[BITS 32]
[SECTION .text]

global inb
inb:
    push ebp
    mov ebp, esp

    xor eax, eax
    mov dx, [ebp + 8]

    in al, dx
    
    leave
    ret

global outb
outb:
    push ebp
    mov ebp, esp

    mov dx, [ebp + 8]
    mov al, [ebp + 12]

    out dx, al

    leave
    ret

