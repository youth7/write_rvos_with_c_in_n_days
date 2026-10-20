#include "types.h"
#include "io.h"

#define MAX_TASKS 10
#define STACK_SIZE 1024




struct context {
    uint32_t ra;
    uint32_t sp;
    uint32_t gp;
    uint32_t tp;

    uint32_t t0; //━━━━━━━━┓
    uint32_t t1; //        ┃t0-t2寄存器是临时寄存器，用于保存函数调用过程中不需要保留的值。
    uint32_t t2; //━━━━━━━━┛
                 
    uint32_t s0;//━━━━━━━━┓
    uint32_t s1;//━━━━━━━━┛ s0-s1寄存器是保存(saved)寄存器，用于保存函数调用过程中需要恢复的值。

    uint32_t a0; //━━━━━━━━┓
    uint32_t a1; //        ┃
    uint32_t a2; //        ┃
    uint32_t a3; //        ┃a0-a7寄存器是函数调用时的参数传递寄存器，a0-a1用于返回值，a2-a7用于传递参数。 
    uint32_t a4; //        ┃
    uint32_t a5; //        ┃
    uint32_t a6; //        ┃
    uint32_t a7; //━━━━━━━━┛
    
    uint32_t s2; //━━━━━━━━┓
    uint32_t s3; //        ┃
    uint32_t s4; //        ┃
    uint32_t s5; //        ┃
    uint32_t s6; //        ┃s2-s11寄存器是保存(saved)寄存器，用于保存函数调用过程中需要恢复的值。 
    uint32_t s7; //        ┃
    uint32_t s8; //        ┃
    uint32_t s9; //        ┃
    uint32_t s10; //       ┃
    uint32_t s11;//━━━━━━━━┛

    uint32_t t3; //━━━━━━━━┓
    uint32_t t4; //        ┃
    uint32_t t5; //        ┃t3-t6寄存器是临时寄存器，用于保存函数调用过程中不需要保留的值。
    uint32_t t6; //━━━━━━━━┛
};

uint8_t task_stacks[MAX_TASKS][STACK_SIZE] = {0};
struct context contexts[MAX_TASKS] = {0}; 

static uint32_t current = -1;
static uint32_t top = -1;

extern void switch_to(struct context * next);
//将一个用户任务加载到任务栈中，并初始化其上下文信息
void load_user_task(void (*fn)(void)){
    top++;
    contexts[top].ra = (uint32_t)fn; // 将任务的地址放置到ra寄存器中
    contexts[top].sp = task_stacks[top] + 1023;// 设置任务的stack地址，需要注意栈是从高地址往低地址增长
}

void switch_to(struct context* context_of_next_task){
    
}

//实现真正的调度
void schedule(void){
    printf("%p schedule = \n", schedule);
    return;
    //确定下一个任务并取出它的上下文
    // current = (current + 1) % MAX_TASKS; 
    // //将上下文传给switch()，实现运行该任务
    // struct context* context_of_next_task = &contexts[current];
    // switch_to(&context_of_next_task);
}

void (*yield) (void) = schedule;

//初始化调度器所需的其他参数，例如mscratch
void schedule_init(void){
}


void sleep(int s) {
  for (int i = 0; i < s * 400000; i++) {
    // 在我的机器上，400000次循环大约为1秒，具体数值可能因机器性能不同而有所差异
  }
}


