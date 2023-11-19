#include "../include/linux/string.h"
#include "../include/linux/mm.h"
#include "../include/linux/types.h"
#include "../include/linux/kernel.h"
#include "../include/asm/system.h"

void virtual_memory_init(){
    int* pdt = (int*)get_page();

    memset(pdt, 0, PAGE_SIZE);

    for (int i = 0; i < 4; i++){
        int ptt = (int)get_page();
        int pde = ptt | 0x7;

        *(pdt + i) = pde;
        int* ptt_arr = (int*)ptt;

        /* 1st entry in PTT, physical memory (4M): 0x0 - 0x3FFFFF */
        for (int j = 0; j < 0x400; j++) {
            int pte = (j * 0x1000 + i * 0x400 * 0x1000) | 0x7;
            *(ptt_arr + j) = pte;
        }
    }
    MAGIC_BREAKPOINT
    set_cr3((u32)pdt);
    enable_paging();
    MAGIC_BREAKPOINT


}