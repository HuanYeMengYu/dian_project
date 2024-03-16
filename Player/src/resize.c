#include "../include/video_decoder.h"

// 调整视频帧的分辨率(RGB，打印双空格)
void rgb_resize_space(Frame cur_frame, Frame* new_frame, int pool_size, int strides){
    // 创建调整分辨率后的新视频帧
    new_frame->width = ((int)((cur_frame.width - pool_size)/strides) + 1)*2;
    new_frame->height = (int)((cur_frame.height - pool_size)/strides) + 1;
    new_frame->linesize = new_frame->width * 3;
    new_frame->data = (unsigned char*)malloc(new_frame->linesize*new_frame->height*sizeof(char));
    // average pooling
    for(int y=0;y<new_frame->height;y++){
        for(int x=0;x<new_frame->width/2;x++){
            int old_index = (y*cur_frame.linesize + x*3)*strides;
            int new_index = y*new_frame->linesize+x*3*2;
            unsigned int sum_r = 0, sum_g = 0, sum_b = 0;
            for(int j=0;j<pool_size;j++){
                for(int i=0;i<pool_size;i++){
                    sum_r += cur_frame.data[old_index+j*cur_frame.linesize+i*3];
                    sum_g += cur_frame.data[old_index+1+j*cur_frame.linesize+i*3];
                    sum_b += cur_frame.data[old_index+2+j*cur_frame.linesize+i*3];
                }
            } 
            new_frame->data[new_index] = (unsigned char)(sum_r/(pool_size*pool_size));
            new_frame->data[new_index+1] = (unsigned char)(sum_g/(pool_size*pool_size));
            new_frame->data[new_index+2] = (unsigned char)(sum_b/(pool_size*pool_size));
            new_frame->data[new_index+3] = (unsigned char)(sum_r/(pool_size*pool_size));
            new_frame->data[new_index+4] = (unsigned char)(sum_g/(pool_size*pool_size));
            new_frame->data[new_index+5] = (unsigned char)(sum_b/(pool_size*pool_size));
        }
    }
    return;
}

// 调整视频帧的分辨率(RGB，跳行)
void rgb_resize_strides(Frame cur_frame, Frame* new_frame, int pool_size, int strides){
    // 创建调整分辨率后的新视频帧
    Frame temp_frame;
    temp_frame.width = (int)((cur_frame.width - pool_size)/strides) + 1;
    temp_frame.height = (int)((cur_frame.height - pool_size)/strides) + 1;
    temp_frame.linesize = temp_frame.width * 3;
    temp_frame.data = (unsigned char*)malloc(temp_frame.linesize*temp_frame.height*sizeof(char));
    // average pooling
    for(int y=0;y<temp_frame.height;++y){
        for(int x=0;x<temp_frame.width;++x){
            int old_index = (y*cur_frame.linesize + x*3)*strides;
            int new_index = y*temp_frame.linesize+x*3;
            unsigned int sum_r = 0, sum_g = 0, sum_b = 0;
            for(int j=0;j<pool_size;j++){
                for(int i=0;i<pool_size;i++){
                    sum_r += cur_frame.data[old_index+j*cur_frame.linesize+i*3];
                    sum_g += cur_frame.data[old_index+1+j*cur_frame.linesize+i*3];
                    sum_b += cur_frame.data[old_index+2+j*cur_frame.linesize+i*3];
                }
            } 
            temp_frame.data[new_index] = (unsigned char)(sum_r/(pool_size*pool_size));
            temp_frame.data[new_index+1] = (unsigned char)(sum_g/(pool_size*pool_size));
            temp_frame.data[new_index+2] = (unsigned char)(sum_b/(pool_size*pool_size));
        }
    }
    new_frame->height = (int)(temp_frame.height/2);
    new_frame->linesize = temp_frame.linesize;
    new_frame->width = temp_frame.width;
    new_frame->data = (unsigned char*)malloc((new_frame->linesize)*(new_frame->height)*sizeof(char));
    for(int j=0;j<new_frame->height;++j){
        for(int i=0;i<new_frame->width;++i){
            new_frame->data[j*new_frame->linesize+i*3] = temp_frame.data[j*2*temp_frame.linesize+i*3];
            new_frame->data[j*new_frame->linesize+i*3+1] = temp_frame.data[j*2*temp_frame.linesize+i*3+1];
            new_frame->data[j*new_frame->linesize+i*3+2] = temp_frame.data[j*2*temp_frame.linesize+i*3+2];
        }
    }
    destroy_frame(&temp_frame);
    return;
}

// 调整视频帧的分辨率(灰度图，打印双字符)
void grey_resize_space(Frame cur_frame, Frame* new_frame, int pool_size, int strides){
    // 创建调整分辨率后的新视频帧
    new_frame->width = (int)((cur_frame.width - pool_size)/strides) + 1;
    new_frame->height = (int)((cur_frame.height - pool_size)/strides) + 1;
    new_frame->linesize = new_frame->width * 2 + 1;
    new_frame->data = (unsigned char*)malloc(new_frame->linesize*new_frame->height*sizeof(char));
    // average pooling
    for(int y=0;y<new_frame->height;y++){
        for(int x=0;x<new_frame->width;x++){
            int old_index = (y*cur_frame.linesize + x*3)*strides;
            int new_index = y*new_frame->linesize+x*2;
            unsigned int sum_r = 0, sum_g = 0, sum_b = 0;
            for(int j=0;j<pool_size;j++){
                for(int i=0;i<pool_size;i++){
                    sum_r += cur_frame.data[old_index+j*cur_frame.linesize+i*3];
                    sum_g += cur_frame.data[old_index+1+j*cur_frame.linesize+i*3];
                    sum_b += cur_frame.data[old_index+2+j*cur_frame.linesize+i*3];
                }
            } 
            unsigned int aver_r = (int)(sum_r/(pool_size*pool_size));
            unsigned int aver_g = (int)(sum_g/(pool_size*pool_size));
            unsigned int aver_b = (int)(sum_b/(pool_size*pool_size));
            unsigned int grey_scale = (int)(aver_r * 0.299 + aver_g * 0.587 + aver_b * 0.114); 
            new_frame->data[new_index] = (unsigned char)grey_scale;
            new_frame->data[new_index+1] = (unsigned char)grey_scale; 
        }
        new_frame->data[(y+1)*new_frame->linesize-1] = '\n';
    }
    return;
}

// 调整视频帧的分辨率(灰度图，跳行)
void grey_resize_strides(Frame cur_frame, Frame* new_frame, int pool_size, int strides){
    // 创建调整分辨率后的新视频帧
    Frame temp_frame;
    temp_frame.width = (int)((cur_frame.width - pool_size)/strides) + 1;
    temp_frame.height = (int)((cur_frame.height - pool_size)/strides) + 1;
    temp_frame.linesize = temp_frame.width + 1;
    temp_frame.data = (unsigned char*)malloc(temp_frame.linesize*temp_frame.height*sizeof(char));
    // average pooling
    for(int y=0;y<temp_frame.height;y++){
        for(int x=0;x<temp_frame.width;x++){
            int old_index = (y*cur_frame.linesize + x*3)*strides;
            int new_index = y*temp_frame.linesize+x;
            unsigned int sum_r = 0, sum_g = 0, sum_b = 0;
            for(int j=0;j<pool_size;j++){
                for(int i=0;i<pool_size;i++){
                    sum_r += cur_frame.data[old_index+j*cur_frame.linesize+i*3];
                    sum_g += cur_frame.data[old_index+1+j*cur_frame.linesize+i*3];
                    sum_b += cur_frame.data[old_index+2+j*cur_frame.linesize+i*3];
                }
            } 
            unsigned int aver_r = (int)(sum_r/(pool_size*pool_size));
            unsigned int aver_g = (int)(sum_g/(pool_size*pool_size));
            unsigned int aver_b = (int)(sum_b/(pool_size*pool_size));
            unsigned int grey_scale = (int)(aver_r * 0.299 + aver_g * 0.587 + aver_b * 0.114); 
            temp_frame.data[new_index] = (unsigned char)grey_scale;
        }
        temp_frame.data[(y+1)*temp_frame.linesize-1] = '\n';
    }
    // 跳行
    new_frame->height = temp_frame.height / 2;
    new_frame->linesize = temp_frame.linesize;
    new_frame->width = temp_frame.width;
    new_frame->data = (unsigned char*)malloc(new_frame->linesize*new_frame->height*sizeof(char));
    for(int j=0;j<new_frame->height;j++)
        for(int i=0;i<new_frame->linesize;i++)
            new_frame->data[j*new_frame->linesize+i] = temp_frame.data[j*2*temp_frame.linesize+i];
    destroy_frame(&temp_frame);
    return;
}

