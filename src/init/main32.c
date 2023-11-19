#include "../include/linux/tty.h"
#include "../include/linux/kernel.h"
#include "../include/linux/traps.h"
#include "../include/asm/system.h"
#include "../include/linux/mm.h"
#include "../include/linux/string.h"

void kernel32_entry(){
    console_init();
    idt_init();
    STI
    print_memory_info();

    /* initialize physical page and print status */
    init_physical_memory();
    print_physical_page_usage();

    /* paging */
    virtual_memory_init();
    print_physical_page_usage();

    /* kmalloc */
    kmalloc(1024);
    print_physical_page_usage();


    while(1);
}