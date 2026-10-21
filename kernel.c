#include "io.h"
#include "schedule.h"
extern void page_init();
extern void user_task0();
extern void user_task1();
extern void load_tasks();



void start_kernel(void)
{
	//留一个特殊数值，debug时候回来验证
	int a = 0x709394;
	//初始化uart，qemu中如果不初始化其实也能正常运行，但是在真机环境中必须初始化
	uart_init();
	uart_puts("hello riscv!!!!!!!!!!!!!!!\n");
	page_init();
	// printf("*yield=%p  yield=%p &yield=%p  schedule=%p\n", *yield, yield, &yield, schedule);
	schedule_init();
	load_tasks();
	schedule();
	// user_task0();
	// user_task1();
	while (1) {}; 
}