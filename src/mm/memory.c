#include "../include/linux/mm.h"
#include "../include/linux/kernel.h"

#define ARDS_ADDR 0x800 /* the ARDS address we stored during BootLoader*/
#define VALID_MEM_ADDR 0x100000


static pmem_info_t pmem_info;
static pmem_map_t pmem_map;

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

/*
 * Byte Map initialization
 */

static void init_memory_map(){
    pmem_map.pmem = &pmem_info;
    pmem_map.map = (u8*)VALID_MEM_ADDR;
    pmem_map.size = pmem_map.pmem->page_total / PAGE_SIZE;
    if(pmem_map.pmem->page_total % PAGE_SIZE){
        pmem_map.size++;
    }
    /* initialize all bitmap to 0 */
    for (int i = 0; i < pmem_map.pmem->page_total; i++){
        *(pmem_map.map + i) = 0;
    }
    for (int i = 0; i <pmem_map.size; i++){
        *(pmem_map.map + i) = 1;
        pmem_map.pmem->page_used++;
        pmem_map.pmem->page_free--;

    }
}

/*
 * Physical Memory initialization
 */

void init_physical_memory(){
    adrs_info_t* adrs_table = (adrs_info_t*)ARDS_ADDR;
    adrs_t* adrs_item = &(adrs_table->data);    /* pointer to the first ARDS item */
    for(int i = 0; i < adrs_table->length; i++){
        if( ((adrs_item + i)->type == 1) && ((adrs_item + i)->base_low == VALID_MEM_ADDR) ){
            pmem_info.addr_start = (adrs_item + i)->base_low;
            pmem_info.addr_end = (adrs_item + i)->base_low + (adrs_item + i)->length_low - 1;
            pmem_info.length = (adrs_item + i)->length_low;
            pmem_info.page_total = (adrs_item + i)->length_low / PAGE_SIZE;
            pmem_info.page_used = 0;
            pmem_info.page_free =  pmem_info.page_total;
            break;
        }
    }
    if(pmem_info.addr_start != VALID_MEM_ADDR){
        printk("No valid physical memory found!\n");
        return;
    }
    init_memory_map();
}

void print_physical_page_usage(){
    printk("\n======= Physical Page =======\n", pmem_map.pmem->page_total);
    printk(" Total - %d\n", pmem_map.pmem->page_total);
    printk(" Used  - %d\n", pmem_map.pmem->page_used);
    printk(" Free  - %d\n", pmem_map.pmem->page_free);
    printk(" Mapping for first 40 pages:\n ");
    for (int i = 0; i < 40; i++) {
        printk("%d", *(pmem_map.map + i));
        if (i % 5 == 4) {
            printk(" ");
        }
    }
    printk("\n=============================\n");
}

void* get_page(){
    int i = 0;
    for (; i < pmem_map.pmem->page_total; i++){
        if(*(pmem_map.map + i) == 0){
            *(pmem_map.map + i) = 1;
            pmem_map.pmem->page_used++;
            pmem_map.pmem->page_free--;
            break;
        }
    }
    return (void*)(pmem_map.pmem->addr_start+PAGE_SIZE*i);
}

void free_page(void* page){
    int index = ((int)page - pmem_map.pmem->addr_start) / PAGE_SIZE;
    *(pmem_map.map + index) = 0;
    pmem_map.pmem->page_used--;
    pmem_map.pmem->page_free++;
}