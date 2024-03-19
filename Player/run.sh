#!/bin/bash

# 编译
gcc -g ./include/* ./src/* -o ./bin/player -L. -lvideodecoder -lavformat -lavcodec -lavutil -lswscale -lpthread

# 进入 bin 目录
cd ./bin

