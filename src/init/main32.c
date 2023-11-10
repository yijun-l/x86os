#include "../include/linux/tty.h"
#include "../include/linux/kernel.h"
#include "../include/linux/traps.h"
#include "../include/asm/system.h"

void kernel32_entry(){
    console_init();
    idt_init();
    STI

    while(1);
}