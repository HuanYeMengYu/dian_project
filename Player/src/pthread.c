#include "../include/video_decoder.h"
#include <pthread.h>

// 解析视频、处理视频帧的线程
void* process(void* arg){
    int status = decoder_init(filename);
    if(status == -1){
        printf("视频文件打开失败！\n");
        pthread_exit(NULL);
    }
    Frame cur_frame;
    init_frame(&cur_frame);
    int frame_strides = color? 5 : 1;
    while(1){
        if(!playing){
            printf("视频结束！\n");
            pthread_exit(NULL);
        }
        if(!isFull(&Buffer)){
            for(int i=0;i<frame_strides;i++){    // 设置打印视频帧步长
                cur_frame = decoder_get_frame();
                if(cur_frame.height==0 && cur_frame.linesize==0 && cur_frame.width==0 && cur_frame.data==NULL){
                    printf("视频结束！\n");
                    playing = false;
                    pthread_exit(NULL);
                }
            }
            Frame *new_frame = (Frame*)malloc(sizeof(Frame));
            init_frame(new_frame);
            if(color)
                rgb_resize_strides(cur_frame, new_frame, pool_size, strides);
            else
                grey_resize_strides(cur_frame, new_frame, pool_size, strides);
            enqueue(&Buffer, new_frame);
        }
    }
    pthread_exit(NULL);
}

// 打印视频帧的线程
void* print(void *arg){
    bool pause = false;
    while(1){
        if(!isEmpty(&Buffer)&&!pause){
            if(color){
                write_rgbframe(*(Buffer.buffer[Buffer.front]));
                usleep(1000000/fps*5);     // 设置帧率
                dequeue(&Buffer);
            }else{
                write_greyframe(*(Buffer.buffer[Buffer.front]));
                usleep(1000000/fps);     // 设置帧率
                dequeue(&Buffer);
            }
        }else if(!playing){
            playing = false;
            pthread_exit(NULL);
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
                playing = false;
                fcntl(STDIN_FILENO, F_SETFL, flags);
                pthread_exit(NULL);
                break;
        }
        
        if(!pause)
            system("clear");
        
        // 恢复stdin的阻塞模式
        fcntl(STDIN_FILENO, F_SETFL, flags);
    }
    pthread_exit(NULL);
}

