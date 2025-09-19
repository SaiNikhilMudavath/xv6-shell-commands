#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

extern int printhistory();
int sys_gethistory(void)
{
  return printhistory();
}


int blocksyscalls[30][30]={0}; //x is shell and y is syscall num
int index=1;
int exec_suc=0;

int sys_inc(void)
{
  if(index>0)
  {
    for(int i=0;i<30;i++)
    {
      blocksyscalls[index][i]=blocksyscalls[index-1][i];
    }
  }
  index++;
}

int sys_block(void)
{
  int syscnum;
  int r=argint(0,&syscnum);
  if(r<0) return -1;
  if(syscnum<=0 || syscnum>26) return -1;
  if(syscnum==1 || syscnum==2) return -1;
  // blocksyscalls[syscnum]=1;
  // cprintf("index is %d\n",index);
  blocksyscalls[index-1][syscnum]=1;
  // cprintf("blocking syscall %d and changing the bit and num is %d\n",syscnum,blocksyscalls[index-1]);
  return 0;

}

int sys_unblock(void)
{
  int syscnum;
  int r=argint(0,&syscnum);
  if(r<0) return -1;
  if(syscnum<=0 || syscnum>26) return -1;
  if(syscnum==1 || syscnum==2) return -1;
  // blocksyscalls[syscnum]=0;
  if (index>1 && blocksyscalls[index-2][syscnum]==0)
  {
    blocksyscalls[index-1][syscnum]=0;
  }
  else if(index==1) blocksyscalls[index-1][syscnum]=0;
  else return -1;
  return 0;
}