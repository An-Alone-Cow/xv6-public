#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  printf(1, "initial free pages: %d\n", numfreepages());

  char* mem = malloc(5);

  printf(1, "free pages after malloc: %d\n", numfreepages());

  if(fork() > 0) {
    printf(1, "free pages in fork parent: %d\n", numfreepages());

    wait();
  } else {
    sleep(10);
    printf(1, "free pages in fork child: %d\n", numfreepages());

    mem[0] = 'a';

    printf(1, "free pages in fork child after write: %d\n", numfreepages());
  }

  exit();
}
