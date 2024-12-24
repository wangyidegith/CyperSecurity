typedef int T;   // This is that you need modify only.
typedef struct Queue Queue;
Queue* queueInit(const int QUEUE_SIZE);
void enqueue(Queue* q, T item);
T dequeue(Queue* q);
void queueDestroy(Queue* q);
