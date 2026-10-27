#include "io.h"
#include "riscv.h"
#include "types.h"

extern void trap_vector(void);

uint32_t trap_handler(uint32_t mepc, uint32_t mcause) {
  printf("🪤🪤🪤🪤🪤🪤🪤🪤🪤🪤🪤%x %p\n", mcause, mepc);
  return mepc + 4;
//   return mepc;
}

void trap_init(void) { r_mtvec((uint32_t)trap_vector); }

void make_exception() { *(uint32_t *)(0) = 0; }