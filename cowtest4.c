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
    if(fork() > 0) {
      sleep(30);

      mem[0] = 'a';
      printf(1, "free pages after fourth write: %d\n", numfreepages());

      wait();
    } else {
      sleep(20);
      mem[0] = 'a';
      printf(1, "free pages after third write: %d\n", numfreepages());
    }
    wait();
  } else {
    if(fork() > 0) {
      sleep(10);

      mem[0] = 'a';
      printf(1, "free pages after second write: %d\n", numfreepages());

      wait();
    } else {
      printf(1, "free pages after before writes: %d\n", numfreepages());

      mem[0] = 'a';
      printf(1, "free pages after first write: %d\n", numfreepages());

      sleep(50);
    }
  }

  exit();
}
