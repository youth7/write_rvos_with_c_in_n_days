#include "io.h"

extern void sleep(int s);


void user_task0(void) {
  while(1) {
    sleep(1);
    printf("this is task0 🐨 🐨 🐨 🐨 🐨 \n");
  }
}

void user_task1(void) {
  while(1) {
    sleep(1);
    printf("this is task1 😮 😮 😮 😮 😮\n");
  }
}
