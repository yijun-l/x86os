#ifndef _LINUX_MM_H
#define _LINUX_MM_H

#include "./types.h"

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

void print_memory_info();

#endif