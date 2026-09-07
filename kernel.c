// 声明uart.c中定义的各种函数，他们后续会被链接进来
void uart_init(void);
void uart_puts(char *c);

void start_kernel(void)
{
	//留一个特殊数值，debug时候回来验证
	int a = 0x709394;
	//初始化uart，qemu中如果不初始化其实也能正常运行，但是在真机环境中必须初始化
	uart_init();
	//输出内容
	uart_puts("hello riscv!!!!!!!!!!!!!!!\n");
	while (1) {}; 
}