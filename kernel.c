#include "types.h"
#include "io.h"
#include "schedule.h"
extern void page_init();
extern void load_tasks();
extern void trap_init();
extern void make_exception();


void start_kernel(void)
{
	//留一个特殊数值，debug时候回来验证
	int a = 0x709394;
	//初始化uart，qemu中如果不初始化其实也能正常运行，但是在真机环境中必须初始化
	uart_init();
	uart_puts("hello riscv!!!!!!!!!!!!!!!\n");
	page_init();
	trap_init();
	make_exception();
	// *(uint32_t *)0 = 0;
	schedule_init();
	load_tasks();
	schedule();
	while (1) {}; 
}