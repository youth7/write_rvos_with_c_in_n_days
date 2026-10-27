#include "types.h"
#include "io.h"
#include "schedule.h"
#define SLEEP_INTERVAL 4
#define WAIT_FOR_SECONDS 10

extern void make_exception();
extern void sleep(int s);

void user_task0(void) {
  int i = 0;
  printf("task%d start 🐨 🐨 🐨 🐨 🐨 addr = %p\n", i, user_task0);
  sleep(WAIT_FOR_SECONDS);
  while (1) {
    sleep(SLEEP_INTERVAL);
    printf("task%d in while loop 🐨\n", i);
    yield();
  }
}

void user_task1(void) {
  int i = 1;
  printf("task%d start 😮 😮 😮 😮 😮 addr = %p\n", i, user_task0);
  sleep(WAIT_FOR_SECONDS);
  while (1) {
    sleep(SLEEP_INTERVAL);
    printf("task%d in while loop 😮\n", i);
    printf("即将进入异常\n");
    *(uint32_t *)0 = 0;
    // make_exception();
    printf("已经从异常中返回了\n");    
    yield();
  }
}


void user_task2(void) {
  printf("task2 start ❤️‍🔥 ❤️‍🔥 ❤️‍🔥 ❤️‍🔥 ❤️‍🔥 addr = %p\n", user_task2);
  printf("即将进入异常\n");
  *(uint32_t *)0 = 0;
  // make_exception();
  printf("已经从异常中返回了\n");


  
}


void load_tasks(void) {
  printf("加载的任务地址是user_task0=%p user_task1=%p\n", user_task0, user_task1);
  load_one_task(&user_task0);
  load_one_task(&user_task1);
  // load_one_task(&user_task2);
}