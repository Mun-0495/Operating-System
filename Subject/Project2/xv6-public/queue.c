#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"
#include "queue.h"

extern uint ticks;

queue* queue_init(struct queue* q, int level) {
    queue* init_queue = (struct queue*)kalloc();
    init_queue->time_quantuam = 2 * (level+1);
    if(level == 99) init_queue->time_quantuam = 99;
    init_queue->front = init_queue->rear = (void*)0;
    init_queue->size = 0;

    q = init_queue;
    return q;
}

int isexist(queue* q, struct proc* p) {
    struct proc* find;
    for(find = q->front; find != q->rear; find = find->next) {
        if(find == p) return 1;
    }
    if(find == p) return 1;
    return 0;
}

int isempty(queue* q) {
    if(q->size == 0)
        return 1;
    return 0;
}

struct proc* front(queue* q) {
    return q->front;
}

struct proc* find_previous_process(queue* q, struct proc* p) {
    struct proc* prev = q->front;
    struct proc* curr;
    for(curr = q->front->next; curr != q->front; curr = curr->next) {
        if(curr == p) break;
        prev = prev->next;
    }
    if(curr != p) return (void*)0;

    return prev;
}

struct proc* find_process(queue* q, struct proc* p) {
    struct proc* curr;
    for(curr = q->front; curr != q->rear; curr = curr->next) {
        if(curr == p) break;
    }
    if(curr != p) return (void*)0;
    
    return curr;
}

struct proc* find_process_pid(queue* q, int pid) {
    struct proc* p;
    for(p = q->front; p != q->rear; p = p->next) {
        if(p->pid == pid) return p;
    }
    if(p->pid == pid) return p;
    
    return (void*)0;
}

struct proc* pop(queue* q) {
    struct proc* proc = q->front;
    q->front = q->front->next;
    q->size--;

    return proc;
}

void push(queue* q, struct proc* p) {

    p->level = (q->time_quantuam) / 2 - 1;
    if(q->time_quantuam == 99) p->level = 99;
    if (isempty(q)) {
        q->front = q->rear = p;
        p->next = p;
        return;
    }
    q->rear->next = p;
    q->rear = p;
    q->size++;
    
    //process now in queue->rear. process->next is q->front
    //link them.
    p->next = q->front; 

    return;
}

//set process to queue->front.
void set_front(queue* q, struct proc* p) {
    struct proc* prev = q->front;
    struct proc* curr = prev->next;

    while (curr != p) {
        prev = curr;
        curr = curr->next;
    }
    q->front = p;
    q->rear = prev;
    return;
}

int process_exists(queue* q, struct proc* p) {
    if (isempty(q)) return 0;
    struct proc* curr = q->front;
    do {
        if (curr == p) return 1;
        curr = curr->next;
    } while (curr != q->front);
    
    return 0;
}

struct proc* pop_targetproc(queue* q, struct proc* p) {
    struct proc* find = find_previous_process(q, p);
    
    if(find == (void*)0) return find;
    
    find->next = find->next->next;
    q->size--;

    return find;
}

/**
 * @brief Return a first element of current priority queue.
 * @param pq Process priority queue.
 * @return [struct proc*] First element of current priority queue.
 */
struct proc* top_pri_proc(queue* pq) {
    struct proc* ret = (void*)0;
    for (int pri = 0; pri <= 10; ++pri) {
        struct proc* p = front(pq);
        while (1) {
            if (p->priority == pri) {
                ret = p;
                return ret;
            }
            p = p->next;
            if (p == front(pq)) break;
        }
    }
    return (void*)0;
}