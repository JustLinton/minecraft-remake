#!/bin/bash
echo 正在编译..
g++ l5.cpp -o l5 -lGL -lGLU -lglut -lGLESv2
echo 运行..
./l5
