#!/bin/bash
echo 正在编译..
g++ l6.cpp -o l6 -lGL -lGLU -lglut -lGLESv2
echo 运行..
./l6
