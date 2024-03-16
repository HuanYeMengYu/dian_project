#include "../include/video_decoder.h"

//  .^,:;Il!i><~+_-?][}{1)(|/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B$@
const char* asciiChar = " .',:;irsXA253hMHGS#9B&@";

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

    print_video("../ref_video/bad_apple.mp4", 6, 6);
 
    return 0;
}

