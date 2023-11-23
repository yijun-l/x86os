#ifndef _ASM_SYSTEM_H
#define _ASM_SYSTEM_H

#define MAGIC_BREAKPOINT __asm__("xchg bx, bx");    /* Bochs magic breakpoint*/

#define STI __asm__("sti");     /* Set Interrupt Flag */
#define CLI __asm__("cli");     /* Clear Interrupt Flag */
#define HLT __asm__("hlt");     /* Halt */

#endif
