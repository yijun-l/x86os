#include "../include/stdarg.h"
#include "../include/linux/tty.h"

/* convey an integer to a string */
static void itos(int num, char* buf, int format)
{
    int i = 0;
    int is_negative = 0;

    /* handle negative numbers */
    if(num < 0){
        is_negative = 1;
        num = -num;
    }

    if(num == 0){
        *(buf + i++) = '0';
    }

    /* convert each digit in reverse order */
    while(num > 0){
        if (num % format < 10){
            *(buf + i++) = num % format + '0';
        } else{
            *(buf + i++) = (num % format - 10) + 'a';
        }
        num /= format;
    }

    /* add the negative sign if necessary */
    if(is_negative){
        *(buf + i++) = '-';
    }

    /* Null-terminate the string */
    *(buf + i) = '\0';

    /* reverse the string to get the correct order */
    int len = i;
    for(int j = 0; j < len/2; j++){
        char tmp = *(buf + j);
        *(buf + j) = *(buf + i - j - 1);
        *(buf + i - j - 1) = tmp;
    }
}

/* convey an memory address to a string */
static void mtos(unsigned int num, char* buf)
{
    int i = 0;

    for(int c = 0; c < 8; c++){
        if (num % 16 < 10){
            *(buf + i++) = num % 16 + '0';
        } else{
            *(buf + i++) = (num % 16 - 10) + 'a';
        }
        num /= 16;
    }
    *(buf + i++) = 'x';
    *(buf + i++) = '0';

    /* Null-terminate the string */
    *(buf + i) = '\0';

    /* reverse the string to get the correct order */
    int len = i;
    for(int j = 0; j < len/2; j++){
        char tmp = *(buf + j);
        *(buf + j) = *(buf + i - j - 1);
        *(buf + i - j - 1) = tmp;
    }
}

int vsprintf(char *buf, const char *fmt, va_list args)
{
    char* str;
    char tmp_buf[200] = {0};
    char* tmp = tmp_buf;

    for(str=buf ; *fmt ; ++fmt){
        if(*fmt != '%'){
            *str++ = *fmt;
            continue;
        }
        fmt++;      /* skip first '%' */
        switch(*fmt){
            case 'c':
                *str++ = va_arg(args, char);
                break;
            case 'd':
                itos(va_arg(args, int), tmp, 10);
                while(*tmp){
                    *str++ = *tmp++;
                }
                break;
            case 'x':
                itos(va_arg(args, int), tmp, 16);
                while(*tmp){
                    *str++ = *tmp++;
                }
                break;
            case 'm':
                mtos(va_arg(args, int), tmp);
                while(*tmp){
                    *str++ = *tmp++;
                }
                break;
            case 's':
                tmp = va_arg(args, char*);
                while(*tmp){
                    *str++ = *tmp++;
                }
                break;
            default:
                break;
        }
    }
    return str - buf;
}

/* write to buffer */
int sprintf(char* buf, const char *fmt, ...)
{
    va_list args;
    int i;

    va_start(args, fmt);
    i = vsprintf(buf, fmt, args);
    va_end(args);

    return i;
}

/* write to console */
int printf(const char *fmt, ...)
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