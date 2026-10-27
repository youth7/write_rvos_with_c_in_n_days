#include "io.h"
#include "riscv.h"
#include "types.h"

extern void trap_vector(void);

void trap_handler(uint32_t mcause, uint32_t mepc) {
  printf("🪤🪤🪤🪤🪤🪤🪤🪤🪤🪤🪤%x %p\n", mcause, mepc);
}

void trap_init(void) { r_mtvec((uint32_t)trap_vector); }

void make_exception() { *(uint32_t *)(0) = 0; }