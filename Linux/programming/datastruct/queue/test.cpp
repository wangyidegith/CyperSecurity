#include <iostream>
#include <pthread.h>
#include "m_queue.h"

Queue<int> myQueue(QUEUE_SIZE);
pthread_t writerThread, readerThread;

void* writerFunction(void* arg) {
    for (int i = 1; i <= 10; ++i) {
        myQueue.enqueue(i);
        std::cout << "Enqueued: " << i << std::endl;
    }
    pthread_exit(NULL);
}

void* readerFunction(void* arg) {
    for (int i = 0; i < 10; ++i) {
        int item = myQueue.dequeue();
        std::cout << "Dequeued: " << item << std::endl;
    }
    pthread_exit(NULL);
}

int main() {
    if (pthread_create(&writerThread, NULL, writerFunction, NULL) != 0) {
        std::cerr << "Failed to create writer thread." << std::endl;
        return 1;
    }

    if (pthread_create(&readerThread, NULL, readerFunction, NULL) != 0) {
        std::cerr << "Failed to create reader thread." << std::endl;
        return 1;
    }

    pthread_join(writerThread, NULL);
    pthread_join(readerThread, NULL);

    return 0;
}
