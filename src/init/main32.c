#include "../include/linux/tty.h"
#include "../include/linux/kernel.h"
#include "../include/linux/traps.h"
#include "../include/asm/system.h"

void kernel32_entry(){
    console_init();
    idt_init();
    MAGIC_BREAKPOINT
    int i = 1/0;

    while(1);

}