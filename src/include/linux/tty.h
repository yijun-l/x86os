#ifndef _LINUX_TTY_H
#define _LINUX_TTY_H

#include "./types.h"

void console_init();
void console_write(const u8 *buf);

#endif