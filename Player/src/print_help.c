#include "../include/video_decoder.h"
#include <string.h>
#include <unistd.h>

const char* help = "Usage: ./player [options]\nOptions:\n  --version                Display program version.\n  --help                   Display the help manual.\n  --color                  Choose to play rgb-video or not.\n  --resize                 Resize the video's resolution.\n  --file                   Choose which video to play.\n  -v                       Display program version.\n  -h                       Display the help manual.\n  -c                       Choose to play rgb-video or not.\n  -r                       Resize the video's resolution.\n  -                        Choose which video to play.\n";

void print_help(void){
    write(STDOUT_FILENO, help, strlen(help));
    return;
}
