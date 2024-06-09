#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"
#include "queue.h"
#include "mlfq.h"

int sys_yield(void) {
    yield();
    return 0;
}

int sys_getlev(void) {
    return getlev();
}

int sys_setpriority(void) {
    int pid, priority;
    if(argint(0, &pid) || argint(1, &priority) < 0) 
        return -1;
    setpriority(pid, priority);
    return 0;
}