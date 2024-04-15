//header file of queue

typedef struct queue {
    struct proc* rear;
    struct proc* front;
    int time_quantuam;
    int size;
} queue;

struct queue* queue_init(queue* q, int level);

int isempty(queue* q);

struct proc* front(queue* q);

struct proc* find_previous_process(queue* q, struct proc* p);

struct proc* find_process(queue* q, struct proc* p);

struct proc* pop(queue* q);

void unlink_proc(queue* q, struct proc* p);

void push(queue* q, struct proc* p);

//set process to queue->front.
void set_front(queue* q, struct proc* p);

int process_exists(queue* q, struct proc* p);

void clear(struct queue* q, struct proc* p);

struct proc* top_pri_proc(queue* pq);