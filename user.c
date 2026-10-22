#include "io.h"
#include "schedule.h"
#define SLEEP_INTERVAL 4
#define WAIT_FOR_SECONDS 10

extern void sleep(int s);

void user_task0(void) {
  printf("task0 start 🐨 🐨 🐨 🐨 🐨 addr = %p\n", user_task0);
  sleep(WAIT_FOR_SECONDS);
  while (1) {
    sleep(SLEEP_INTERVAL);
    printf("task0 in while loop 🐨\n");
    yield();
  }
}

void user_task1(void) {
  printf("task0 start 😮 😮 😮 😮 😮 addr = %p\n", user_task1);
  sleep(WAIT_FOR_SECONDS);
  while (1) {
    sleep(SLEEP_INTERVAL);
    printf("task0 in while loop 😮\n");
    yield();
  }
}

void load_tasks(void) {
  printf("加载的任务地址是user_task0=%p user_task1=%p\n", user_task0, user_task1);
  load_one_task(&user_task0);
  load_one_task(&user_task1);
}