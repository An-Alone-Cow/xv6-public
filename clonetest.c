#include "types.h"
#include "user.h"

int input1[4] = {1, 2, 3, 0};
int input2[4] = {11, 2, 3, 0};
int input3[4] = {1, 2, 33, 0};

int mutex;
int use_mutex = 0;

void
fn(void *data)
{
  int *arr = data;
  int a = arr[0], b = arr[1], c = arr[2];

  if(use_mutex)
    acquire_mutex(mutex);

  printf(1, "%d + %d * %d = %d\n", a, b, c, a + b*c);

  if(use_mutex)
    release_mutex(mutex);

  arr[3] = a + b*c;

  sleep(100);
  threadexit();
}

int
main(int argc, char *argv[])
{
  mutex = create_mutex();

  void *stack1 = malloc(5000);
  void *stack2 = malloc(5000);
  void *stack3 = malloc(5000);

  int tid1 = clone(fn, &input1, stack1);
  int tid2 = clone(fn, &input2, stack2);
  int tid3 = clone(fn, &input3, stack3);

  join(tid2);
  join(tid1);
  join(tid3);

  printf(1, "Results:\n%d %d %d\n", input1[3], input2[3], input3[3]);
  printf(1, "\n");

  use_mutex = 1;

  tid1 = clone(fn, &input1, stack1);
  tid2 = clone(fn, &input2, stack2);
  tid3 = clone(fn, &input3, stack3);

  join(tid2);
  join(tid1);
  join(tid3);

  printf(1, "Results:\n%d %d %d\n", input1[3], input2[3], input3[3]);

  delete_mutex(mutex);

  exit();
}
