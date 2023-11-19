#ifndef _LINUX_MM_H
#define _LINUX_MM_H

#include "./types.h"

#define PAGE_SIZE 4096

typedef struct{
    u32 base_low;
    u32 base_high;
    u32 length_low;
    u32 length_high;
    u32 type;
}adrs_t;

typedef struct{
    u32 length;
    adrs_t data;
} adrs_info_t;

typedef struct{
    u32 addr_start;
    u32 addr_end;
    u32 length;
    u32 page_total;
    u32 page_free;
    u32 page_used;
} pmem_info_t;

typedef struct{
    pmem_info_t* pmem;
    u32 size;
    u8* map;
} pmem_map_t;

void print_memory_info();
void init_physical_memory();
void print_physical_page_usage();
void* get_page();
void free_page(void*);
void virtual_memory_init();

void* kmalloc(size_t);
void kfree_s(void*, int);


#endif
