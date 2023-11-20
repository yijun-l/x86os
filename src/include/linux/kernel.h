#ifndef _LINUX_KERNEL_H
#define _LINUX_KERNEL_H

#include "./types.h"

int printk(const char *fmt, ...);
int sprintf(char *buf, const char *fmt, ...);

u32 get_cr3();
void set_cr3(u32 v);
void enable_paging();

void clock_init();

#endif
