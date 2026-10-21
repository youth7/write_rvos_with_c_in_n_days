#include "io.h"
#include "schedule.h"

extern void sleep(int s);

void user_task0(void) {
  printf("this is task0 🐨 🐨 🐨 🐨 🐨 addr = %p\n", user_task0);
  while (1) {
    sleep(1);
    printf("this is task0 🐨  addr = %p\n", user_task0);
    yield();
  }
}

void user_task1(void) {
  printf("this is task1 😮 😮 😮 😮 😮 addr = %p\n", user_task1);
  while (1) {
    sleep(1);
    printf("this is task1 😮  addr = %p\n", user_task1);
    yield();
  }
}

void load_tasks(void) {
  printf("加载的任务地址是user_task0=%p user_task1=%p\n", user_task0, user_task1);
  load_one_task(&user_task0);
  load_one_task(&user_task1);
}