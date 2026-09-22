#include "io.h"
extern void page_init();
extern int HEAP_START;
extern int _heap_start;

void start_kernel(void)
{
	//留一个特殊数值，debug时候回来验证
	int a = 0x709394;
	//初始化uart，qemu中如果不初始化其实也能正常运行，但是在真机环境中必须初始化
	uart_init();
	uart_puts("hello riscv!!!!!!!!!!!!!!!\n");
	page_init();
	while (1) {}; 
}