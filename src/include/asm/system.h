#ifndef _ASM_SYSTEM_H
#define _ASM_SYSTEM_H

#define MAGIC_BREAKPOINT __asm__("xchg bx, bx");

#define STI __asm__("sti");
#define CLI __asm__("cli");

#endif