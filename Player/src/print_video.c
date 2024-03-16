#include "../include/video_decoder.h"
#include <stdlib.h>

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
    init_frame(&cur_frame);
    init_frame(&new_frame);
    
    bool pause = false;
    double fps = 34.0;

    // 将标准输入设置为非规范模式，以禁用行缓冲
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);  // 禁用规范模式和回显
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    
    // 将标准输入设置为非阻塞模式
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
   
    while(1){  
        if(!pause){
            for(int i=0;i<1;i++)    // 设置打印视频帧步长
                cur_frame = decoder_get_frame();
            if(cur_frame.height==0 && cur_frame.linesize==0 && cur_frame.width==0 && cur_frame.data==NULL){
                printf("视频结束！\n");
                // 恢复标准输入的设置
                term.c_lflag |= (ICANON | ECHO);
                tcsetattr(STDIN_FILENO, TCSANOW, &term);
                decoder_close();
                return;
            }
            init_frame(&new_frame);

            //rgb_resize_space(cur_frame, &new_frame, pool_size, strides);
            //rgb_resize_strides(cur_frame, &new_frame, pool_size, strides);
            //grey_resize_space(cur_frame, &new_frame, pool_size, strides);
            grey_resize_strides(cur_frame, &new_frame, pool_size, strides);
            //write_rgbframe(new_frame);
            write_greyframe(new_frame); 

            usleep(1000000/fps);     // 设置帧率 
            destroy_frame(&new_frame);
        }
        
        int key = getchar();
        switch (key) {
            case 32:    // space：暂停/播放
                pause = pause ^ 1;
                break;
            case 100:   // d：播放速度翻倍
                fps *= 2;
                break;
            case 113:   // q：退出播放
                decoder_close();
                return;
                break;
            default:
                break;
        }
        
        if(!pause)
            system("clear");
    }
    decoder_close();
    // 恢复标准输入的设置
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    return;
}

