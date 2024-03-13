#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/video_decoder.h"

// 打印视频帧
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
}

// 打印视频
void print_video(){
    // 打开视频文件
    int status = decoder_init("../ref_video/BadApple样例参考.mp4");
    if(status == -1){
        printf("视频文件打开失败！\n");
        exit(-1);
    } 
    Frame cur_frame = decoder_get_frame();
    if(cur_frame.height==0 && cur_frame.linesize==0 && cur_frame.width==0 && cur_frame.data==NULL){
        printf("视频帧已全部获取！\n");
        decoder_close();
        return;
    }
    print_frame(cur_frame);
    decoder_close();
    return;
}

int main(int argc, char* argv[])
{
    print_video();

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
    return 0;
}

