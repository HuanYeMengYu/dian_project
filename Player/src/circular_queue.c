#include "../include/video_decoder.h"

// 初始化循环队列
void initQueue(CircularQueue *Buffer, int capacity) {
    Buffer->buffer = (Frame**)malloc((capacity+1)*sizeof(Frame*));
    for(int i=0;i<Buffer->capacity+1;i++)
        Buffer->buffer[i] = NULL;
    Buffer->capacity = capacity;
    Buffer->front = Buffer->rear = Buffer->count = 0;
    return;
}

// 判断队列是否为空
bool isEmpty(CircularQueue *Buffer) {
    return (Buffer->count == 0);
}

// 判断队列是否已满
bool isFull(CircularQueue *Buffer) {
    return (Buffer->count == Buffer->capacity);
}

// 入队操作
void enqueue(CircularQueue *Buffer, Frame* new_frame) {
    if (!isFull(Buffer)) {
        Buffer->buffer[Buffer->rear] = new_frame;
        Buffer->rear = (Buffer->rear + 1) % (Buffer->capacity+1);
        Buffer->count++;
    } else {
        printf("Queue is full. Cannot enqueue.\n");
        return;
    }
}

// 出队操作
void dequeue(CircularQueue *Buffer) {
    if (!isEmpty(Buffer)) {
        destroy_frame(Buffer->buffer[Buffer->front]);
        free(Buffer->buffer[Buffer->front]);
        Buffer->buffer[Buffer->front] = NULL;
        Buffer->front = (Buffer->front + 1) % (Buffer->capacity+1);
        Buffer->count--;
    } else
        printf("Queue is empty. Cannot dequeue.\n");
    return;
}

// 销毁队列
void destroyQueue(CircularQueue *Buffer) {
    for(int i=0;i<Buffer->capacity+1;i++){
        destroy_frame(Buffer->buffer[i]);
        free(Buffer->buffer[i]);
        Buffer->buffer[i] = NULL;
    }
    free(Buffer->buffer);
    Buffer->buffer = NULL;
    Buffer->capacity = Buffer->front = Buffer->rear = Buffer->count = 0;
    return;
}

