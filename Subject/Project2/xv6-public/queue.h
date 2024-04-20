//header file of queue

typedef struct queue {
    struct proc* rear;
    struct proc* front;
    int time_quantuam;
    int size;
} queue;

struct queue* queue_init(queue* q, int level);

int isempty(queue* q);

struct proc* pop(queue* q);

void push(queue* q, struct proc* p);

//set process to queue->front.
int process_exists(queue* q, struct proc* p);

void clear(struct queue* q, struct proc* p);

struct proc* top_pri_proc(queue* pq);

struct proc* find_process_pid(queue* q, int pid);