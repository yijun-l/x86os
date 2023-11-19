#ifndef _LINUX_KERNEL_H
#define _LINUX_KERNEL_H

#include "./types.h"
extern int printk(const char *fmt, ...);
extern int sprintf(char *buf, const char *fmt, ...);

extern u32 get_cr3();
extern void set_cr3(u32 v);
extern void enable_paging();

#endif
