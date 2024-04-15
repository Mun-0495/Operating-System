#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"
#include "queue.h"

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
    mlfq->queue[L2] = queue_init(mlfq->queue[L3], L2);
    mlfq->priority_queue = queue_init(mlfq->priority_queue, L3);

    return mlfq;
}

void mlfq_push(mlfq* mlfq, struct proc* proc, int n) {

    push(mlfq->queue[n], proc);
    return;
}

void mlfq_pqpush(mlfq* mlfq, struct proc* proc) {
    //TODO : L3 queue push

}

struct proc* mlfq_pop(mlfq* mlfq) {
    struct proc* proc = (void*)0;
    if(!isempty(mlfq->queue[L0])) {
        proc = pop(mlfq->queue[L0]);
    }
    else if(!isempty(mlfq->queue[L1])) {
        proc = pop(mlfq->queue[L1]);
    }
    else if(!isempty(mlfq->queue[L2])) {
        proc = pop(mlfq->queue[L2]);
    }
    else if(!isempty(mlfq->priority_queue)) {
        proc = pop(mlfq->priority_queue);
    }
    
    return proc;
}

void mlfq_boost(mlfq* mlfq) {
    for(struct proc* i = mlfq->queue[L1]->front; i != mlfq->queue[L1]->rear; i = i->next) {
        i->tick = 0;
        push(mlfq->queue[L0], i);
    }

    for(struct proc* i = mlfq->queue[L2]->front; i != mlfq->queue[L2]->rear; i = i->next) {
        i->tick = 0;
        push(mlfq->queue[L0], i);
    }

    for(struct proc* i = mlfq->queue[L3]->front; i != mlfq->queue[L3]->rear; i = i->next) {
        i->tick = 0;
        push(mlfq->queue[L0], i);
    }
}


