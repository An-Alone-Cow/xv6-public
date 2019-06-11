#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  printf(1, "initial free pages: %d\n", numfreepages());

  int in_stack_variable = 0;
  if(fork() > 0) {
    printf(1, "%d |free pages in fork parent: %d\n", in_stack_variable, numfreepages());

    in_stack_variable = 1;
    printf(1, "%d |free pages in fork parent after write: %d\n", in_stack_variable, numfreepages());

    wait();
  } else {
    sleep(10);
    printf(1, "%d |free pages in fork child: %d\n", in_stack_variable, numfreepages());

    in_stack_variable = 1;
    printf(1, "%d |free pages in fork child after write: %d\n", in_stack_variable, numfreepages());
  }

  exit();
}
