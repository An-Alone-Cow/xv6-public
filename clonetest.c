#include "types.h"
#include "user.h"

int input1[4] = {1, 2, 3, 0};
int input2[4] = {11, 2, 3, 0};
int input3[4] = {1, 2, 33, 0};

void
fn(void *data)
{
  int *arr = data;
  int a = arr[0], b = arr[1], c = arr[2];

  printf(1, "%d + %d * %d = %d\n", a, b, c, a + b*c);

  arr[3] = a + b*c;

  sleep(100);
  threadexit();
}

int
main(int argc, char *argv[])
{
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

  exit();
}
