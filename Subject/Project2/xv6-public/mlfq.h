#define L0 0
#define L1 1
#define L2 2
#define L3 3

typedef struct mlfq{
    struct queue* queue[3];
    struct queue* priority_queue;
} mlfq;


struct mlfq* mlfq_init();

void mlfq_push(mlfq* mlfq, struct proc* proc, int n);

struct proc* mlfq_pop_targetproc(mlfq* mlfq, struct proc* proc);

struct proc* mlfq_pop(struct mlfq* mlfq);

void mlfq_boost(mlfq* mlfq);

void down_queue(mlfq* mlfq, int level);