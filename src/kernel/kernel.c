#include "../include/linux/types.h"
#include "../include/linux/kernel.h"

inline u32 get_cr3(){
    asm volatile("mov eax, cr3;");
}

inline void set_cr3(u32 v){
    asm volatile("mov cr3, eax;": : "a"(v));
}

inline void enable_paging(){
    asm volatile("mov eax, cr0;"
             "or eax, 0x80000000;"
             "mov cr0, eax;");
}

//inline u32 get_cr3(){
//    __asm__ ("mov eax, cr3;");
//}
//
//inline void set_cr3(u32 v){
//    __asm__ ("mov cr3, eax;": : "a"(v));
//}
//
//inline void enable_paging(){
//    __asm__ ("mov eax, cr0;"
//             "or eax, 0x80000000;"
//             "mov cr0, eax;");
//}