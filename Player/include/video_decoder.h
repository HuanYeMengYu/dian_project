#ifndef __VIDEO_DECODER_HEADER__
#define __VIDEO_DECODER_HEADER__

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern const char* asciiChar;

typedef struct _Frame{
    int width;
    int height;
    int linesize;
    unsigned char* data;
} Frame;

int decoder_init(const char *filename);
Frame decoder_get_frame();
void decoder_close();
double get_fps();
int get_frame_index();
int get_total_frames();

// my_function
void rgb_resize_space(Frame cur_frame, Frame* new_frame, int pool_size, int strides);// 调整视频帧的分辨率(RGB)，打印双像素点
void grey_resize_space(Frame cur_frame, Frame* new_frame, int pool_size, int strides);// 调整视频帧的分辨率(灰度图)，打印双字符
void rgb_resize_strides(Frame cur_frame, Frame* new_frame, int pool_size, int strides);// 调整视频帧的分辨率(RGB)，跳行
void grey_resize_strides(Frame cur_frame, Frame* new_frame, int pool_size, int strides);// 调整视频帧的分辨率(灰度图)，跳行
void init_frame(Frame* new_frame);// 初始化新视频帧
void destroy_frame(Frame* new_frame);// 销毁调整分辨率后的新视频帧
void write_rgbframe(Frame cur_frame);// 打印rgb视频帧(控制背景颜色，打印空白字符)
void write_greyframe(Frame cur_frame);// 打印灰度视频帧(write)
void print_video(const char* filename, int pool_size, int strides);// 打印视频

#endif // !__VIDEO_DECODER_HEADER__
