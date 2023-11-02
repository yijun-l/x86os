#include "../include/linux/tty.h"
#include "../include/linux/kernel.h"

void kernel32_entry(){

    console_init();
    printf("==> %s\n","Yijun");
    printf("==> %d\n",83777629);
    printf("==> %c\n",'C');
    while(1);

}

