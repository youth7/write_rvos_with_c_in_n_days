// 声明uart.c中定义的各种函数，他们后续会被链接进来
extern void uart_init(void);
extern void uart_puts(char *c);
extern int printf(const char* s, ...);
extern int HEAP_START;
extern int _heap_start;

void start_kernel(void)
{
	//留一个特殊数值，debug时候回来验证
	int a = 0x709394;
	//初始化uart，qemu中如果不初始化其实也能正常运行，但是在真机环境中必须初始化
	uart_init();
	//输出内容
	printf("heap addr is %x %x %x\n",  &HEAP_START, HEAP_START, &_heap_start);
	uart_puts("hello riscv!!!!!!!!!!!!!!!\n");
	while (1) {}; 
}