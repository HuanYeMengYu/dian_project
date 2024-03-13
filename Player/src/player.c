#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/video_decoder.h"

// 调整视频帧的分辨率
void resize(Frame cur_frame, Frame* new_frame, int pool_size, int strides){
    // 创建调整分辨率后的新视频帧
    new_frame->width = (int)((cur_frame.width - pool_size)/(double)strides) + 1;
    new_frame->height = (int)((cur_frame.height - pool_size)/(double)strides) + 1;
    new_frame->linesize = new_frame->width * 3;
    new_frame->data = (unsigned char*)malloc(new_frame->linesize*new_frame->height*sizeof(char));
    // average pooling
    for(int y=0;y<new_frame->height;y++){
        for(int x=0;x<new_frame->width;x++){
            int old_index = (y*cur_frame.width + x)*strides*3;
            int new_index = (y*new_frame->width+x)*3;
            unsigned int sum_r = 0, sum_g = 0, sum_b = 0;
            for(int j=0;j<pool_size;j++){
                for(int i=0;i<pool_size;i++){
                    sum_r += cur_frame.data[old_index+(j*cur_frame.width+i)*3];
                    sum_g += cur_frame.data[old_index+1+(j*cur_frame.width+i)*3];
                    sum_b += cur_frame.data[old_index+2+(j*cur_frame.width+i)*3];
                }
            } 
            new_frame->data[new_index] = (int)(sum_r/pool_size);
            new_frame->data[new_index+1] = (int)(sum_g/pool_size);
            new_frame->data[new_index+2] = (int)(sum_b/pool_size);
        }
    }
    return;
}

// 初始化新视频帧
void init_frame(Frame* new_frame){
    new_frame->data = NULL;
    new_frame->height = new_frame->linesize = new_frame->width = 0;
    return;
}

// 销毁调整分辨率后的新视频帧
void destroy_frame(Frame* new_frame){
    free(new_frame->data);
    new_frame->data = NULL;
    new_frame->height = new_frame->linesize = new_frame->width = 0;
    return;
}

// 打印视频帧(控制背景颜色，打印空字符)
void print_frame(Frame cur_frame){
    for (int y = 0; y < cur_frame.height; ++y) {
        for (int x = 0; x < cur_frame.width; ++x) {
            int index = (y * cur_frame.width + x) * 3;
            unsigned int r = (int)(cur_frame.data[index]);
            unsigned int g = (int)(cur_frame.data[index + 1]);
            unsigned int b = (int)(cur_frame.data[index + 2]);
            printf("\e[48;2;%u;%u;%um ", r, g, b); // 输出空格字符
        }
        printf("\e[0m\n"); // 恢复默认背景色
    }
    return;
}

// 打印视频
void print_video(const char* filename, int pool_size, int strides){
    // 打开视频文件
    int status = decoder_init(filename);
    if(status == -1){
        printf("视频文件打开失败！\n");
        exit(-1);
    }
    Frame cur_frame;
    Frame new_frame;
    int frame_num = get_total_frames();
    while(1){
        for(int i=0;i<10;i++)
            cur_frame = decoder_get_frame();
        if(cur_frame.height==0 && cur_frame.linesize==0 && cur_frame.width==0 && cur_frame.data==NULL){
            printf("视频结束！\n");
            decoder_close();
            return;
        }
        init_frame(&new_frame);
        resize(cur_frame, &new_frame, pool_size, strides);
        print_frame(new_frame);
        //sleep(1);
        system("clear");
        destroy_frame(&new_frame);
    }
    decoder_close();
    return;
}

int main(int argc, char* argv[])
{
    // 命令行参数解析
    printf("初始值：optind = %d，opterr = %d\n", optind, opterr);
    int opt;
    while ((opt = getopt(argc, argv, "vha:b:c::")) != -1)
    { 
        printf("开始解析参数：optind = %d\n", optind);
        switch (opt) 
        {   
                case 'v':
                        printf("捕获到选项: -v\n");
                        printf("系统版本v1.1.0\n");   
                        break;
                case 'h':
                        printf("捕获到选项: -h\n"); 
                        printf("用法示例：\n");
                        break;
        }
    }

    print_video("../ref_video/bad_apple.mp4", 1, 1);
 
    return 0;
}

