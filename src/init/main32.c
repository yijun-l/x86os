#include "../include/linux/tty.h"
#include "../include/linux/kernel.h"
#include "../include/linux/traps.h"
#include "../include/asm/system.h"
#include "../include/linux/mm.h"

void kernel32_entry(){
    console_init();
    idt_init();
    STI
    print_memory_info();

    while(1);
}