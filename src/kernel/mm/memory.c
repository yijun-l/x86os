#include "../../include/linux/mm.h"
#include "../../include/linux/kernel.h"

#define ARDS_ADDR 0x800 /* the ARDS address we stored during BootLoader*/

void print_memory_info(){
    adrs_info_t* adrs_table = (adrs_info_t*)ARDS_ADDR;
    adrs_t* adrs_item = &(adrs_table->data);    /* pointer to the first ARDS item */
    printk("\nMemory-map Info:\n");
    for(int i = 0; i < adrs_table->length; i++){
        char valid = 'N';
        if((adrs_item + i)->type == 1){
            valid = 'Y';
        }
        printk("[%d] %m - %m, %c\n", i+1, (adrs_item + i)->base_low, (adrs_item + i)->base_low + (adrs_item + i)->length_low - 1, valid);
    }
}
