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

#define L0 0
#define L1 1
#define L2 2
#define L3 3

struct mlfq* mlfq_init() {
    struct mlfq* mlfq = (struct mlfq*)kalloc();

    mlfq->queue[L0] = queue_init(mlfq->queue[L0], L0);
    mlfq->queue[L1] = queue_init(mlfq->queue[L1], L1);
    mlfq->queue[L2] = queue_init(mlfq->queue[L2], L2);
    mlfq->priority_queue = queue_init(mlfq->priority_queue, L3);

    return mlfq;
}

void mlfq_push(struct mlfq* mlfq, struct proc* proc, int n) {
    //TODO : mlfq_push
    if(n == 0) push(mlfq->queue[L0], proc);
    if(n == 0) push(mlfq->queue[L1], proc);
    if(n == 0) push(mlfq->queue[L2], proc);
    if(n == 0) push(mlfq->priority_queue, proc);
    return;
}

struct proc* mlfq_pop(struct mlfq* mlfq) {
    int cnt = 0;
    struct proc* p = (void*)0;

    //L0 RR
    while(cnt < mlfq->queue[L0]->size) {
      p = pop(mlfq->queue[L0]);
      if(p->state == RUNNABLE) break;
      push(mlfq->queue[L0], p);
      cnt++;
    }
    
    //L1 RR
    cnt = 0;
    while(cnt < mlfq->queue[L1]->size) {
      p = pop(mlfq->queue[L1]);
      if(p->state == RUNNABLE) break;
      push(mlfq->queue[L1], p);
      cnt++;
    }

    //L2 RR
    cnt = 0;
    while(cnt < mlfq->queue[L2]->size) {
      p = pop(mlfq->queue[L0]);
      if(p->state == RUNNABLE) break;
      push(mlfq->queue[L0], p);
      cnt++;
    }

    //L3 RR
    cnt = 0;
    while(cnt < mlfq->priority_queue->size) {
      p = top_pri_proc(mlfq->priority_queue);
      if(p -> state == RUNNABLE) break;
      push(mlfq->priority_queue, p);
      cnt++;
    }

    return p;
}

void mlfq_boost(struct mlfq* mlfq) {
    for(int i=1; i<3; i++) {
        while(!isempty(mlfq->queue[i])) {
            struct proc* proc = pop(mlfq->queue[i]);
            proc->level = L0;
            proc->tick = 0;
            proc->priority = 10;
            push(mlfq->queue[L0], proc);
        }
    }
    
    while(!isempty(mlfq->priority_queue)) {
        struct proc* proc = pop(mlfq->priority_queue);
        proc->level = L0;
        proc->tick = 0;
        proc->priority = 10;
        push(mlfq->queue[L0], proc);
    }
}

void down_queue(struct mlfq* mlfq, int level) {
  if(level == L0) {
    while(!isempty(mlfq->queue[L0])) {
      struct proc* proc = pop(mlfq->queue[L0]);
      proc->tick = 0;
      if(proc->pid % 2 == 1) {
        proc->level = L1;
        mlfq_push(mlfq, proc, L1);
      }
      if(proc->pid % 2 == 0) {
        proc->level = L2;
        mlfq_push(mlfq, proc, L2);
      }
    }
  }
  else if(level == L1 || level == L2) {
    while(!isempty(mlfq->queue[level])) {
      struct proc* proc = pop(mlfq->queue[level]);
      proc->tick = 0;
      proc->level = L3;
      mlfq_push(mlfq, proc, L3);
    }
  }
}