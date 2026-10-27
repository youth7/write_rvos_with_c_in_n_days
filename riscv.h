#include "types.h"
#pragma once

/*

GCC 内联汇编的完整语法结构是：

asm volatile ("汇编指令" : 输出列表 : 输入列表 : 破坏列表);

GCC 语法规定，破坏列表为空时，它前面的冒号必须省去，因此完整的四段式语法就呈现为

asm volatile ("汇编指令" : 输出列表 : 输入列表 );

*/

static inline void r_mtvec(uint32_t trap_handler){
    asm volatile(
        "csrw mtvec, %0":
        :
        "r"(trap_handler)
    );
}


static inline void r_mscratch(uint32_t mscratch){
    asm volatile(
        "csrw mscratch, %0" : 
        : 
        "r" (mscratch)
    );
}