#include "../include/video_decoder.h"
#include <unistd.h>

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
    double fps = 34.0;  // 灰度图34.0
    while(1){  
        if(!pause){
            for(int i=0;i<1;i++)    // 设置打印视频帧步长
                cur_frame = decoder_get_frame();
            if(cur_frame.height==0 && cur_frame.linesize==0 && cur_frame.width==0 && cur_frame.data==NULL){
                printf("视频结束！\n");
                decoder_close();
                return;
            }
            init_frame(&new_frame);

            //rgb_resize_space(cur_frame, &new_frame, pool_size, strides);
            rgb_resize_strides(cur_frame, &new_frame, pool_size, strides);
            //grey_resize_space(cur_frame, &new_frame, pool_size, strides);
            //grey_resize_strides(cur_frame, &new_frame, pool_size, strides);
            write_rgbframe(new_frame);
            //write_greyframe(new_frame); 

            usleep(1000000/fps);     // 设置帧率
            destroy_frame(&new_frame);
        }
       
        // 将标准输入设置为非阻塞模式(stdin缓冲区没有字符时getchar()函数不会阻塞程序)
        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
        int key = 0; 
        key=getch();

        switch (key) {
            case ' ':    // space：暂停/播放
                pause = pause ^ 1;
                break;
            case 'd':   // d：播放速度翻倍
                fps *= 2;
                break;
            case 'q':   // q：退出播放
                decoder_close();
                return;
                break;
        }
        
        if(!pause)
            system("clear");
        
        // 恢复stdin的阻塞模式
        fcntl(STDIN_FILENO, F_SETFL, flags);
    }
    decoder_close();
    return;
}

