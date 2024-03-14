#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/video_decoder.h"

//  .^,:;Il!i><~+_-?][}{1)(|/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B$@
const char* asciiChar = " .',:;irsXA253hMHGS#9B&@";

// 打印rgb视频帧的rgb平面到txt文件
void rgbframe_to_txt(Frame cur_frame){
    FILE *fp;
    // 打开输出文件
    fp = fopen("../test/rgbframe.txt", "w");
    if (fp == NULL) {
        printf("Error: Unable to open file for writing\n");
        return;
    }
    // 将灰度值写入文件
    for (int y = 0; y < cur_frame.height; y++) {
        for (int x = 0; x < cur_frame.width; x++) {
            unsigned int index = (y*cur_frame.width+x)*3;
            fprintf(fp, "%d,%d,%d ", cur_frame.data[index], cur_frame.data[index+1], cur_frame.data[index+2]);
        }
        fprintf(fp, "\n"); // 换行
    }
    // 关闭文件
    fclose(fp);
    return;
}

// 打印灰度图视频帧的灰度值平面到txt文件
void greyframe_to_txt(Frame cur_frame){
    FILE *fp;
    // 打开输出文件
    fp = fopen("../test/greyframe.txt", "w");
    if (fp == NULL) {
        printf("Error: Unable to open file for writing\n");
        return;
    }
    // 将灰度值写入文件
    for (int y = 0; y < cur_frame.height; y++) {
        for (int x = 0; x < cur_frame.width; x++) {
            fprintf(fp, "%d ", cur_frame.data[y*cur_frame.width+x]);
        }
        fprintf(fp, "\n"); // 换行
    }
    // 关闭文件
    fclose(fp);
    return;
}

// 调整视频帧的分辨率(RGB)
void rgb_resize(Frame cur_frame, Frame* new_frame, int pool_size, int strides){
    // 创建调整分辨率后的新视频帧
    new_frame->width = (int)((cur_frame.width - pool_size)/strides) + 1;
    new_frame->height = (int)((cur_frame.height - pool_size)/strides) + 1;
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
            new_frame->data[new_index] = (int)(sum_r/(pool_size*pool_size));
            new_frame->data[new_index+1] = (int)(sum_g/(pool_size*pool_size));
            new_frame->data[new_index+2] = (int)(sum_b/(pool_size*pool_size));
        }
    }
    return;
}

// 调整视频帧的分辨率(灰度图)
void greyscale_resize(Frame cur_frame, Frame* new_frame, int pool_size, int strides){
    // 创建调整分辨率后的新视频帧
    new_frame->width = (int)((cur_frame.width - pool_size)/strides) + 1;
    new_frame->height = (int)((cur_frame.height - pool_size)/strides) + 1;
    new_frame->linesize = new_frame->width * 2;
    new_frame->data = (unsigned char*)malloc(new_frame->linesize*new_frame->height*sizeof(char));
    // average pooling
    for(int y=0;y<new_frame->height;y++){
        for(int x=0;x<new_frame->width;x++){
            int old_index = (y*cur_frame.width + x)*strides*3;
            int new_index = (y*new_frame->width+x)*2;
            unsigned int sum_r = 0, sum_g = 0, sum_b = 0;
            for(int j=0;j<pool_size;j++){
                for(int i=0;i<pool_size;i++){
                    sum_r += cur_frame.data[old_index+(j*cur_frame.width+i)*3];
                    sum_g += cur_frame.data[old_index+1+(j*cur_frame.width+i)*3];
                    sum_b += cur_frame.data[old_index+2+(j*cur_frame.width+i)*3];
                }
            } 
            unsigned int aver_r = (int)(sum_r/(pool_size*pool_size));
            unsigned int aver_g = (int)(sum_g/(pool_size*pool_size));
            unsigned int aver_b = (int)(sum_b/(pool_size*pool_size));
            unsigned int grey_scale = (int)(aver_r * 0.299 + aver_g * 0.587 + aver_b * 0.114); 
            new_frame->data[new_index] = (unsigned char)grey_scale;
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

// 打印rgb视频帧(控制背景颜色，打印空字符)
void print_rgb_frame(Frame cur_frame){
    for (int y = 0; y < cur_frame.height; ++y) {    // 设置打印字符步长：y+=2
        for (int x = 0; x < cur_frame.width; ++x) {     // 设置打印字符步长：x+=2
            int index = (y * cur_frame.width + x) * 3;
            unsigned char r = cur_frame.data[index];
            unsigned char g = cur_frame.data[index + 1];
            unsigned char b = cur_frame.data[index + 2];
            printf("\e[48;2;%d;%d;%dm  ", r, g, b); // 输出空格字符
        }
        printf("\e[0m\n"); // 恢复默认背景色
    } 
    return;
}

// 打印灰度视频帧(printf)
void print_greyscale_frame(Frame cur_frame){    // 设置打印字符步长：y+=2
    for (int y = 0; y < cur_frame.height; ++y) {    // 设置打印字符步长：x+=2
        for (int x = 0; x < cur_frame.width; ++x){
            unsigned int ascii_index = (int)(cur_frame.data[(y * cur_frame.width + x)*2] / 256.0 * strlen(asciiChar));
            printf("%c ", asciiChar[ascii_index]);
        }
        printf("\n");
    } 
    return;
}

// 打印灰度视频帧(write)
void write_greyscale_frame(Frame cur_frame){    // 设置打印字符步长：y+=2
    for (int y = 0; y < cur_frame.height; ++y) {    // 设置打印字符步长：x+=2
        for (int x = 0; x < cur_frame.width; ++x){
            int ascii_index = (int)(cur_frame.data[(y * cur_frame.width + x)*2] / 256.0 *strlen(asciiChar));
            cur_frame.data[(y * cur_frame.width + x)*2] = asciiChar[ascii_index];
            if(x!=cur_frame.width-1)
                cur_frame.data[(y * cur_frame.width + x)*2 + 1] = ' ';
            else
                cur_frame.data[(y+1) * cur_frame.width * 2 - 1] = '\n';
        } 
    } 
    write(STDOUT_FILENO, cur_frame.data, cur_frame.width*cur_frame.height*2);
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
    init_frame(&cur_frame);
    init_frame(&new_frame);
    int frame_num = get_total_frames();
    while(1){
        for(int i=0;i<5;i++)    // 设置打印视频帧步长
            cur_frame = decoder_get_frame();
        if(cur_frame.height==0 && cur_frame.linesize==0 && cur_frame.width==0 && cur_frame.data==NULL){
            printf("视频结束！\n");
            decoder_close();
            return;
        }
        init_frame(&new_frame);
        greyscale_resize(cur_frame, &new_frame, pool_size, strides);
        write_greyscale_frame(new_frame);
        usleep(1000000/10);     // 设置帧率
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

    print_video("../ref_video/bad_apple.mp4", 2, 2);
 
    return 0;
}

