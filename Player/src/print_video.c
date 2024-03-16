#include "../include/video_decoder.h"

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
    while(1){
        for(int i=0;i<5;i++)    // 设置打印视频帧步长
            cur_frame = decoder_get_frame();
        if(cur_frame.height==0 && cur_frame.linesize==0 && cur_frame.width==0 && cur_frame.data==NULL){
            printf("视频结束！\n");
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

        /*int c = getchar();
        switch (c) {
            case 32:    // space
                pause = pause ^ 1;
                break;
            case 100:   // d

                break;
        }*/

        usleep(1000000/5);     // 设置帧率
        system("clear");
        destroy_frame(&new_frame);
    }
    decoder_close();
    return;
}
