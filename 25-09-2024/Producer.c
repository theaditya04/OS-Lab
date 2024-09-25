#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


typedef struct Node {
    int data;
    struct Node* next;
} Node;


typedef struct {
    Node* head;
    Node* tail;
    int size;
    int maxSize;
    pthread_mutex_t mutex;
    pthread_cond_t notFull;
    pthread_cond_t notEmpty;
} Buffer;


void initBuffer(Buffer* buf, int maxSize) {
    buf->head = NULL;
    buf->tail = NULL;
    buf->size = 0;
    buf->maxSize = maxSize;
    pthread_mutex_init(&buf->mutex, NULL);
    pthread_cond_init(&buf->notFull, NULL);
    pthread_cond_init(&buf->notEmpty, NULL);
}


void produce(Buffer* buf, int item) {
    pthread_mutex_lock(&buf->mutex);
    while (buf->size == buf->maxSize) {
        printf("Buffer is full. Producer is waiting...\n");
        pthread_cond_wait(&buf->notFull, &buf->mutex);
    }


    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = item;
    newNode->next = NULL;

    if (buf->tail) {
        buf->tail->next = newNode;
    } else {
        buf->head = newNode; // First element
    }
    buf->tail = newNode;
    buf->size++;

    printf("Produced: %d. Buffer size: %d\n", item, buf->size);
    pthread_cond_signal(&buf->notEmpty);
    pthread_mutex_unlock(&buf->mutex);
}


int consume(Buffer* buf) {
    pthread_mutex_lock(&buf->mutex);
    while (buf->size == 0) {
        printf("Buffer is empty. Consumer is waiting...\n");
        pthread_cond_wait(&buf->notEmpty, &buf->mutex);
    }


    Node* temp = buf->head;
    int item = temp->data;
    buf->head = buf->head->next;

    if (buf->head == NULL) {
        buf->tail = NULL; 
    }
    buf->size--;

    printf("Consumed: %d. Buffer size: %d\n", item, buf->size);
    free(temp);

    pthread_cond_signal(&buf->notFull);
    pthread_mutex_unlock(&buf->mutex);
    return item;
}


void* producer(void* arg) {
    Buffer* buf = (Buffer*)arg;
    for (int i = 0; i < 10; i++) {
        produce(buf, i);
        sleep(rand() % 2);
    }
    return NULL;
}

void* consumer(void* arg) {
    Buffer* buf = (Buffer*)arg;
    for (int i = 0; i < 10; i++) {
        consume(buf);
        sleep(rand() % 2); // Simulate time taken to consume
    }
    return NULL;
}

int main() {
    int maxSize;
    printf("Enter buffer size: ");
    scanf("%d", &maxSize);

    Buffer buf;
    initBuffer(&buf, maxSize);

    pthread_t prodThread, consThread;


    pthread_create(&prodThread, NULL, producer, (void*)&buf);
    pthread_create(&consThread, NULL, consumer, (void*)&buf);


    pthread_join(prodThread, NULL);
    pthread_join(consThread, NULL);


    pthread_mutex_destroy(&buf.mutex);
    pthread_cond_destroy(&buf.notFull);
    pthread_cond_destroy(&buf.notEmpty);

    return 0;
}

