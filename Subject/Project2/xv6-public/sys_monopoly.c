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

int sys_setmonopoly(void) {
    int pid, password;
    if(argint(0, &pid) < 0 || argint(1, &password) < 0) return -1;
    return setmonopoly(pid, password);
}

int sys_monopolize(void) {
    monopolize();
    return 0;
}

int sys_unmonopolize(void) {
    unmonopolize();
    return 0;
}