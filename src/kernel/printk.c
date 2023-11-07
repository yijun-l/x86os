#include "../include/stdarg.h"
#include "../include/linux/tty.h"

extern int vsprintf(char *buf, const char *fmt, va_list args);

int printk(const char *fmt, ...)
{
    va_list args;
    int i;
    char buf[2000] = " ";
    va_start(args, fmt);
    i = vsprintf(buf, fmt, args);
    console_write(buf);
    va_end(args);

    return i;
}