#include "io.h"
#include "riscv.h"
#include "types.h"

extern void trap_vector(void);

uint32_t trap_handler(uint32_t mepc, uint32_t mcause) {
  uint32_t mask = 1 << 31;
  uint32_t return_address = mepc;
  uint32_t cause_code = mcause & (~mask);
  if (mcause & mask) {
    // 最高位为1表示中断，为0表示异常
    printf("中断发生🌶️🌶️🌶️🌶️🌶️🌶️🌶️🌶️🌶️🌶️🌶️%x %p，原因是：", mcause, cause_code);
    return return_address;
  } else {
    printf("异常发生🍋🍋🍋🍋🍋🍋🍋🍋🍋🍋🍋%x %p，原因是：", mcause, cause_code);
    
    switch(cause_code){
        case 1: uart_puts("Instruction access fault\n");break;
        case 2: uart_puts("Illegal instruction\n");break;
        case 3: uart_puts("Breakpoint\n");break;
        case 4: uart_puts("Load address misaligned\n");break;
        case 5: uart_puts("Load access fault\n");break;
        case 6: uart_puts("Store/AMO address misaligned\n");break;
        case 7: uart_puts("Store/AMO access fault\n");break;
        case 11: uart_puts("Environment call from M-mode\n");break;
        default: uart_puts("othsers");

    }
    return return_address + 4;
  }
  //   return mepc;
}

void trap_init(void) { r_mtvec((uint32_t)trap_vector); }

void make_exception() { *(uint32_t *)(0) = 0; }