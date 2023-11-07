#ifndef _LINUX_HEAD_H
#define _LINUX_HEAD_H

#include "./types.h"

typedef struct gate_desc_t {
    u16 offset_l;
    u16 selector;
    u16 reserved : 8;
    u16 type : 4;
    u16 segment : 1;
    u16 dpl : 2;
    u16 present : 1;
    u16 offset_h;
} __attribute((packed)) gate_desc_t;


typedef struct desc_tab_ptr_t {
    u16 size;
    u32 offset;
} __attribute((packed)) desc_tab_ptr_t;

#endif