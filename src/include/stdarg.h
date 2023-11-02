#ifndef _STDARG_H
#define _STDARG_H

typedef char* va_list;

/* move pointer p to the first variable */
#define va_start(p, count) (p = (va_list)&count + sizeof(char*))

/* get the value which pointered by p, then p pointered to the next variable */
#define va_arg(p, t) (*(t*)((p += sizeof(char*)) - sizeof(char*)))

/* reset pointer p */
#define va_end(p) (p = 0)

#endif
