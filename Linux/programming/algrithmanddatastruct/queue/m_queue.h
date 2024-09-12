#define QUEUE_SIZE 1024

template<typename T>
class Queue {
    private:
        int queue_size;
        T items[QUEUE_SIZE];
        int front;
        int rear;
        int count;
        pthread_mutex_t mutex;
        pthread_cond_t not_full;
        pthread_cond_t not_empty;
    public:
        Queue(size_t queue_size) {
            this->queue_size = queue_size;
            front = 0;
            rear = 0;
            count = 0;
            if (pthread_mutex_init(&mutex, NULL)) {}
            if (pthread_cond_init(&not_full, NULL)) {}
            if (pthread_cond_init(&not_empty, NULL)) {}
        }
        void enqueue(T item) {
            pthread_mutex_lock(&mutex);
            while (count == queue_size) {
                pthread_cond_wait(&not_full, &mutex);
            }
            items[rear] = item;
            rear = (rear + 1) % queue_size;
            count++;
            pthread_cond_signal(&not_empty);
            pthread_mutex_unlock(&mutex);
        }
        T dequeue() {
            pthread_mutex_lock(&mutex);
            while (count == 0) {
                pthread_cond_wait(&not_empty, &mutex);
            }
            T item = items[front];
            front = (front + 1) % queue_size;
            count--;
            pthread_cond_signal(&not_full);
            pthread_mutex_unlock(&mutex);
            return item;
        }
};
