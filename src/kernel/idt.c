#include "../include/linux/head.h"
#include "../include/linux/kernel.h"
#define IDT_SIZE 256

gate_desc_t idt[IDT_SIZE] = {0};
desc_tab_ptr_t idt_ptr;

void interrupt_handler_entry(){
    printk("Interrupt handled.\n");
    while(1);
}

void idt_init(){
    printk("IDT initialing...\n");

    for (int i = 0; i < IDT_SIZE; i++){
        gate_desc_t *p = &idt[i];

        int handler = (int)interrupt_handler_entry;

        p->offset_l = handler & 0xffff;
        p->offset_h = (handler >> 16) & 0xffff;
        p->selector = 1 << 3;       /* 1st entry in GDT with R0 */
        p->reserved = 0;
        p->type = 0xe;              /* 32-bit Interrupt Gate    */
        p->segment = 0;
        p->dpl = 0;                 /* R0                       */
        p->present = 1;
    }

    idt_ptr.size = IDT_SIZE * 8;
    idt_ptr.offset = idt;

    asm volatile("lidt idt_ptr;");

}
