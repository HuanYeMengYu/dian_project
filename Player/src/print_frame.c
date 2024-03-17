#include "../include/video_decoder.h"
#include <stdio.h>

//  .^,:;Il!i><~+_-?][}{1)(|/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B$@
const char* asciiChar = " .',:;irsXA253hMHGS#9B&@";

// 打印rgb视频帧(控制背景颜色，打印空字符)
void write_rgbframe(Frame cur_frame){
    char* ansi_rgb = (char*)malloc((cur_frame.width*20+5)*cur_frame.height*sizeof(char));
    for (int y = 0; y < cur_frame.height; ++y) {    // 设置打印字符步长：y+=2
        for (int x = 0; x < cur_frame.width; ++x) {     // 设置打印字符步长：x+=2
            int index = y*cur_frame.linesize + x*3;
            unsigned char r = cur_frame.data[index];
            unsigned char g = cur_frame.data[index + 1];
            unsigned char b = cur_frame.data[index + 2];
            int ansi_index = y*(cur_frame.width*20+5) + x*20;
            ansi_rgb[ansi_index] = '\e';
            ansi_rgb[ansi_index+1] = '[';
            ansi_rgb[ansi_index+2] = '4';
            ansi_rgb[ansi_index+3] = '8';
            ansi_rgb[ansi_index+4] = ansi_rgb[ansi_index+6] = ansi_rgb[ansi_index+10] = ansi_rgb[ansi_index+14] = ';';
            ansi_rgb[ansi_index+5] = '2';
            char buf[10];
            sprintf(buf, "%03d", r);
            for(int i=0;i<3;i++)
                ansi_rgb[ansi_index+7+i] = buf[i];
            sprintf(buf, "%03d", g);
            for(int i=0;i<3;i++)
                ansi_rgb[ansi_index+11+i] = buf[i];
            sprintf(buf, "%03d", b);
            for(int i=0;i<3;i++)
                ansi_rgb[ansi_index+15+i] = buf[i];
            ansi_rgb[ansi_index+18] = 'm';
            ansi_rgb[ansi_index+19] = ' ';
        }
        ansi_rgb[(y+1)*(cur_frame.width*20+5)-5] = '\e';
        ansi_rgb[(y+1)*(cur_frame.width*20+5)-4] = '[';
        ansi_rgb[(y+1)*(cur_frame.width*20+5)-3] = '0';
        ansi_rgb[(y+1)*(cur_frame.width*20+5)-2] = 'm';
        ansi_rgb[(y+1)*(cur_frame.width*20+5)-1] = '\n';
    } 
    write(STDOUT_FILENO, ansi_rgb, cur_frame.height*(cur_frame.width*20+5));
    //fwrite(ansi_rgb, sizeof(unsigned char), cur_frame.height*(cur_frame.width*20+5), stdout);
    return;
}

// 打印灰度视频帧(write)
void write_greyframe(Frame cur_frame){    // 设置打印字符步长：y+=2
    for (int y = 0; y < cur_frame.height; ++y) {    // 设置打印字符步长：x+=2
        for (int x = 0; x < cur_frame.linesize-1; ++x){
            int ascii_index = (int)(cur_frame.data[y*cur_frame.linesize+x]/256.0*strlen(asciiChar));
            cur_frame.data[y * cur_frame.linesize + x] = asciiChar[ascii_index];
        } 
    } 
    write(STDOUT_FILENO, cur_frame.data, cur_frame.linesize*cur_frame.height);
    //fwrite(cur_frame.data, sizeof(unsigned char), cur_frame.linesize*cur_frame.height, stdout);
    return;
}

