#include "../Thready.h"
#include <stdio.h>
#include <string.h>

typedef struct {
  char name[64];
  int age;
  int counter;
} my_data_t;

static void name_thread(void *context, my_data_t *my_data) {
  tStart(context) {
    printf("Started 'name' thread\n");
    printf("Reading user data:\n");
    printf("  Name: %s\n", my_data->name);
    printf("  Age: %d\n", my_data->age);
    static int i = 0;
    for (; i < 8; i++) {
      tSleepMs(100);
      printf("\e[%dmCounting: %d\e[0m\n", my_data->counter + 32, my_data->counter);
      my_data->counter++; // increment counter
    }
    printf("👋 Finishing name thread 👋\n");
  }
}

static void no_user_data_thread(void *context, void *data) {
  tStart(context) {
    printf("stateless thread starting!\n");
    static int i = 0;
    for (; i < 3; i++) {
      tSleepMs(500);
      printf("--- I have no user data but I can count %d --- \n", i);
    }
    printf("👋 Stateless thread finishing! 👋\n");
  }
}


int main(int argc, char **argv) {
  printf("Let's create some \e[33mthreads\e[0m 🚀\n");

  static ThreadyContext name_ctx;

  static my_data_t my_data;
  strncpy(my_data.name, "Alari Õis 🌼", sizeof(my_data.name));
  my_data.age = 35;
  my_data.counter = 0;

  threadyCreate(&name_ctx, (thready_t)name_thread, &my_data);

  static ThreadyContext no_state_ctx;
  threadyCreate(&no_state_ctx, (thready_t)no_user_data_thread, NULL);

  while (true) {
    threadyLoop();
  }
  return 0;
}
