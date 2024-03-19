#ifndef __VIDEO_DECODER_HEADER__
#define __VIDEO_DECODER_HEADER__

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/select.h>
#include <pthread.h>

#define BUFFER_SIZE 10

typedef struct _Frame{
    int width;
    int height;
    int linesize;
    unsigned char* data;
} Frame;

// 循环队列结构体
typedef struct {
    Frame **buffer;  // 缓冲区数组指针
    int capacity; // 缓冲区容量
    int front;    // 队列头部索引
    int rear;     // 队列尾部索引
    int count;    // 队列中元素数量
} CircularQueue;

extern CircularQueue Buffer;
extern int pool_size;
extern int strides;
extern double fps;
extern bool color;
extern bool version;
extern bool help;
extern bool resize;
extern const char* filename;
extern bool playing;

int decoder_init(const char *filename);
Frame decoder_get_frame();
void decoder_close();
double get_fps();
int get_frame_index();
int get_total_frames();

// 初始化循环队列
void initQueue(CircularQueue *Buffer, int capacity);
// 判断队列是否为空
bool isEmpty(CircularQueue *Buffer);
// 判断队列是否已满
bool isFull(CircularQueue *Buffer);
// 入队操作
void enqueue(CircularQueue *Buffer, Frame* new_frame);
// 出队操作
void dequeue(CircularQueue *Buffer);
// 销毁队列
void destroyQueue(CircularQueue *Buffer);

// 调整视频帧的分辨率(RGB)，打印双像素点
void rgb_resize_space(Frame cur_frame, Frame* new_frame, int pool_size, int strides);
// 调整视频帧的分辨率(灰度图)，打印双字符
void grey_resize_space(Frame cur_frame, Frame* new_frame, int pool_size, int strides);
// 调整视频帧的分辨率(RGB)，跳行
void rgb_resize_strides(Frame cur_frame, Frame* new_frame, int pool_size, int strides);
// 调整视频帧的分辨率(灰度图)，跳行
void grey_resize_strides(Frame cur_frame, Frame* new_frame, int pool_size, int strides);
// 初始化新视频帧
void init_frame(Frame* new_frame);
// 销毁调整分辨率后的新视频帧
void destroy_frame(Frame* new_frame);
// 打印rgb视频帧(控制背景颜色，打印空白字符)
void write_rgbframe(Frame cur_frame);
// 打印灰度视频帧(write)
void write_greyframe(Frame cur_frame);
// 打印视频
void print_video(const char* filename, int pool_size, int strides, bool color);
// 键盘自动加载到标准输入缓冲区
int getch(void);
// 打印帮助文档
void print_help(void);
// 解析视频、处理视频帧的线程
void* process(void* arg);
// 打印视频帧的线程
void* print(void *arg);

#endif // !__VIDEO_DECODER_HEADER__
