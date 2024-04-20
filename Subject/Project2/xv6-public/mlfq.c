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

typedef struct mlfq {
    struct queue* queue[3];
    struct queue* priority_queue;
} mlfq;


struct mlfq* mlfq_init() {
    mlfq* mlfq = (struct mlfq*)kalloc();

    mlfq->queue[L0] = queue_init(mlfq->queue[L0], L0);
    mlfq->queue[L1] = queue_init(mlfq->queue[L1], L1);
    mlfq->queue[L2] = queue_init(mlfq->queue[L2], L2);
    mlfq->priority_queue = queue_init(mlfq->priority_queue, L3);

    return mlfq;
}

void mlfq_push(mlfq* mlfq, struct proc* proc, int n) {
    //TODO : mlfq_push
    push(mlfq->queue[n], proc);
    return;
}

// void L0_push(mlfq* mlfq, struct proc* proc) {
//     push(mlfq->queue[L0], proc);
//     return;
// }

// void mlfq_pqpush(mlfq* mlfq, struct proc* proc) {
//     //TODO : L3 queue push

// }

struct proc* mlfq_pop_targetproc(mlfq* mlfq, struct proc* proc) {
    struct proc* find = (void*)0;
    
    for(int i=0; i<3; i++) {
        find = find_process(mlfq->queue[i], proc);
        if(find == proc) {
            return find;
        }
    }

    find = find_previous_process(mlfq->priority_queue, proc);
    if(find == proc) 
        return find;

    return (void*)0;
}

struct proc* mlfq_pop(mlfq* mlfq) {
    //time quantuam?
    struct proc* proc = (void*)0;
    if(!isempty(mlfq->queue[L0]) && mlfq->queue[L0]->front->state == RUNNABLE) {    
        proc = pop(mlfq->queue[L0]);
    }
    else if(!isempty(mlfq->queue[L1]) && mlfq->queue[L1]->front->state == RUNNABLE) {
        proc = pop(mlfq->queue[L1]);
    }
    else if(!isempty(mlfq->queue[L2]) && mlfq->queue[L2]->front->state == RUNNABLE) {
        proc = pop(mlfq->queue[L2]);
    }
    else if(!isempty(mlfq->priority_queue) && mlfq->priority_queue->front->state == RUNNABLE) {
        proc = top_pri_proc(mlfq->priority_queue);
    }
    
    return proc;
}

void mlfq_boost(mlfq* mlfq) {
    for(int i=1; i<3; i++) {
        while(!isempty(mlfq->queue[i])) {
            struct proc* proc = pop(mlfq->queue[i]);
            proc->level = L0;
            proc->tick = 0;
            proc->priority = 0;
            push(mlfq->queue[L0], proc);
        }
    }
    
    while(!isempty(mlfq->priority_queue)) {
        struct proc* proc = pop(mlfq->priority_queue);
        proc->level = L0;
        proc->tick = 0;
        proc->priority = 0;
        push(mlfq->queue[L0], proc);
    }
}

void down_queue(mlfq* mlfq, int level) {
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
