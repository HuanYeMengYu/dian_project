#include "../include/video_decoder.h"
#include <bits/getopt_core.h>

const char* version = "dian-player v1.0";
const char* default_filename = "../ref_video/bad_apple.mp4";

int main(int argc, char* argv[])
{
    const char* filename = default_filename;
    int pool_size=1, strides=1;
    bool version_flag=false, help_flag=false, color_flag=false, resize_flag=false;
    int opt = -1;
    while ((opt = getopt(argc, argv, "vhcr::f::")) != -1)
    {
        switch (opt) 
        {   
                case 'v':
                        version_flag = true;
                        break;
                case 'h':
                        help_flag = true;
                        break;
                case 'c':
                        color_flag = true; 
                        break;
                case 'r':
                        resize_flag = true;
                        pool_size = atoi(optarg);
                        break;
                case 'f':
                        filename = optarg;
                        break;
        }
    } 
    
    if(resize_flag)
        strides = atoi(argv[optind]);
    if(!version_flag && !help_flag) 
        print_video(filename, pool_size, strides, color_flag);
    if(help_flag)
        print_help();
    if(version_flag)
        printf("%s\n", version);

    return 0;
}

