
// 定义UART基地址
#define UART0 0x10000000l

// 定义各个寄存器的偏移量
#define RHR 0
#define THR 0
#define DLL 0
#define IER 1
#define DLM 0
#define FCR 2
#define ISR 2
#define LCR 3
#define MCR 4
#define LSR 5
#define MSR 6
#define SRP 7

// 定义指针的类型
typedef unsigned char uint8;

// 定义指针的类型
#define UART_REG(reg) ((volatile uint8 *)(UART0 + reg))

// 定义读寄存器的宏
#define READ_REG(reg) (*(UART_REG(reg)))

// 定义写寄存器的宏
#define WRITE_REG(reg, value) (*(UART_REG(reg)) = (value))

void uart_init(void);
void uart_puts(char *c);

void uart_init(void)
{
    // 禁用中断控制
    WRITE_REG(IER, 0x00);

    uint8 lcr = READ_REG(LCR);
    // 设置LCR的DLAB位(第7位)为1，改为配置波特率模式
    WRITE_REG(LCR, lcr | 0b10000000);

    // 设置波特率，根据uart的技术手册此时除数为3，即需要往DLM写入0，往DLL写入3
    WRITE_REG(DLL, 3);

    WRITE_REG(DLM, 0);

    /**
     * 写入LCR的0~2位为1，其它位为0，这意味着：
     * 字长为1
     * 停止位为1.5
     * 奇偶校验和断路控制不启用
     * 同时让LCR恢复为正常模式
     * */

    WRITE_REG(LCR, 0b00000011);
}

void uart_put_char(char c)
{
    while ((READ_REG(LSR) & 0b00100000) == 0)
    {
        // LSR的第五位THRE为0的时候，需要一直等待，直到变为1才可以写入数据
    }
    WRITE_REG(THR, c);
}

void uart_puts(char *str)
{
    int i = 0x123456;

    while (*str != '\0')
    {
        uart_put_char(*str);
        str++;
    }
}