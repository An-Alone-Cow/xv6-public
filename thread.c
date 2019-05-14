#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"

struct {
    struct spinlock lock;
    struct thread thread[NTHREAD];
} ttable;

int nexttid = 1;


void
tinit(void)
{
  struct thread* t;

  initlock(&ttable.lock, "ttable");

  // initialize thread pool
  for(t = ttable.thread; t < &ttable.thread[NTHREAD]; t++)
    if((t->kstack = kalloc()) == 0)
      panic("tinit.kalloc");
}

struct thread*
allocthread(void)
{
  struct thread *t;

  acquire(&ttable.lock);

  for(t = ttable.thread; t < &ttable.thread[NTHREAD]; t++)
    if(t->state == FREE)
      goto found;

  release(&ttable.lock);
  return 0;

found:
  t->state = ASSIGNED;
  t->tid = nexttid++;

  release(&ttable.lock);

  return t;
}

void
freethread(struct thread* t)
{
  if(t == 0)
    return;

  acquire(&ttable.lock);

  t->tid = 0;
  t->state = FREE;

  t->proc = 0;
  t->parentproc = 0;

  release(&ttable.lock);
}

void
exitthread(struct thread* t)
{
  acquire(&ttable.lock);

  t->state = EXITED;
  t->proc = 0;

  release(&ttable.lock);
}


struct thread*
getthread(int tid)
{
  struct thread *t;

  acquire(&ttable.lock);

  for(t = ttable.thread; t < &ttable.thread[NTHREAD]; t++)
    if(t->tid == tid)
      goto found;

  release(&ttable.lock);
  return 0;

found:

  release(&ttable.lock);
  return t;
}
