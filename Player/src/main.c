#include "../include/video_decoder.h"
#include <bits/getopt_core.h>
#include <bits/getopt_ext.h>
#include <stdbool.h>
#include <sys/types.h>

CircularQueue Buffer;

int pool_size = 1;
int strides = 1;
double fps = 31.5;
bool color = false;
bool version=false;
bool help=false;
bool resize=false;
bool playing = true;
const char* filename = "../ref_video/bad_apple.mp4";
const char* cur_version = "dian-player v1.0";
const char* help_txt = "../resource/help.txt";

static struct option long_options[] =
{  
    {"version", no_argument, NULL, 'v'},
    {"help", no_argument, NULL, 'h'},
    {"color", no_argument, NULL, 'c'},
    {"resize", optional_argument, NULL, 'r'},
    {"file", optional_argument, NULL, 'f'},
    {"fps", optional_argument, NULL, 'F'},
    {NULL, 0, NULL, 0},
}; 

int main(int argc, char* argv[])
{
    initQueue(&Buffer, BUFFER_SIZE);
    int opt = -1;
    int option_index = 0;
    while ((opt = getopt_long(argc, argv, "vhcr::f::", long_options, &option_index)) != -1)
    {
        char *endptr = NULL;
        switch (opt) 
        {   
                case 'v':
                        version = true;
                        break;
                case 'h':
                        help = true;
                        break;
                case 'c':
                        color = true; 
                        break;
                case 'r':
                        resize = true;
                        pool_size = atoi(optarg);
                        break;
                case 'f':
                        filename = optarg;
                        break;
                case 'F':
                        fps = strtod(optarg, &endptr);
                        break;
        }
    } 
    
    if(resize)
        strides = atoi(argv[optind]);
    if(!version && !help){
        pthread_t process_tid, print_tid;
        pthread_create(&process_tid, NULL, process, NULL);
        pthread_create(&print_tid, NULL, print, NULL);
        pthread_join(process_tid, NULL);
        pthread_join(print_tid, NULL);
    }
    if(help){
        char command[100];
        sprintf(command, "cat %s", help_txt);
        // 执行系统命令
        system(command);
    }
    if(version)
        printf("%s\n", cur_version);
    
    decoder_close();
    destroyQueue(&Buffer);
    return 0;
}

