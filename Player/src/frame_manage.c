#include "../include/video_decoder.h"

// 初始化新视频帧
void init_frame(Frame* new_frame){
    new_frame->data = NULL;
    new_frame->height = new_frame->linesize = new_frame->width = 0;
    return;
}

// 销毁调整分辨率后的新视频帧
void destroy_frame(Frame* new_frame){
    if(!new_frame)
        return;
    if(new_frame->data)
        free(new_frame->data);
    new_frame->data = NULL;
    new_frame->height = new_frame->linesize = new_frame->width = 0;
    return;
}

