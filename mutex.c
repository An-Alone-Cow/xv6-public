#include "types.h"
#include "defs.h"
#include "param.h"
#include "x86.h"
#include "date.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
#include "mutex.h"


struct {
	struct spinlock lock;
	struct mutex mutex[NMUTEX];
} mtable;

int nextmid = 1;


void
minit(void)
{
  initlock(&mtable.lock, "mtable");
}

static struct mutex*
allocmutex()
{
  struct mutex *m;

  acquire(&mtable.lock);

  for(m = mtable.mutex; m < &mtable.mutex[NMUTEX]; m++)
  	if(m->mid == 0)
	  goto found;

  release(&mtable.lock);
  return 0;

found:
  m->mid = nextmid++;

  release(&mtable.lock);
  return m;
}

static struct mutex*
getmutex(int mid)
{
  struct mutex *m;

  acquire(&mtable.lock);

  for(m = mtable.mutex; m < &mtable.mutex[NMUTEX]; m++)
  	if(m->mid == mid)
  	  goto found;

  release(&mtable.lock);
  return 0;

found:

  release(&mtable.lock);
  return m;
}

int
create_mutex(void)
{
  struct mutex *m = allocmutex();

  initlock(&m->lk, "mutex spin lock");
  m->locked = 0;
  m->pid = 0;
  m->ownerpid = myproc()->pid;

  return m->mid;
}

int
acquire_mutex(int mid)
{
  struct mutex *m;
  int pid = myproc()->pid;

  if((m = getmutex(mid)) == 0)
    return -1;

  acquire(&m->lk);

  while(m->locked){
  	sleep(m, &m->lk);
  }

  //lk->locked = 1;
  fetch_and_add(&m->locked, 1);
  m->pid = pid;

  release(&m->lk);
  return 0;
}

int
release_mutex(int mid)
{
  struct mutex *m;
  int pid = myproc()->pid;

  if((m = getmutex(mid)) == 0)
    return -1;

  acquire(&m->lk);

  if(m->locked && m->pid == pid){
    fetch_and_add(&m->locked, -1);
    m->pid = 0;

    wakeup(m);
  } else {
    release(&m->lk);
    return -1;
  }

  release(&m->lk);
  return 0;
}

static void
deallocmutex(struct mutex *m)
{
    m->mid = 0;
    m->pid = 0;
    m->ownerpid = 0;
    m->locked = 0;
}

int
delete_mutex(int mid)
{
  struct mutex *m;
  int pid = myproc()->pid;

  if((m = getmutex(mid)) == 0)
    return -1;

  if(m->ownerpid != pid)
    return -1;

  acquire(&mtable.lock);

  deallocmutex(m);

  release(&mtable.lock);
  return 0;
}

void
delete_proc_mutexes(int pid)
{
  struct mutex *m;

  acquire(&mtable.lock);

  for(m = mtable.mutex; m < &mtable.mutex[NMUTEX]; m++)
    if(m->ownerpid == pid)
      deallocmutex(m);

  release(&mtable.lock);
}
