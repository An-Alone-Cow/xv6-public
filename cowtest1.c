#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  printf(1, "initial free pages: %d\n", numfreepages());

  void* mem;
  if(fork() > 0) {
    printf(1, "free pages in fork parent: %d\n", numfreepages());
    mem = malloc(5);

    wait();

    free(mem);
  } else {
    sleep(10);
    printf(1, "free pages in fork child: %d\n", numfreepages());

    fork();
    wait();
    printf(1, "free pages in fork child's children: %d\n", numfreepages());
  }

  exit();
}
