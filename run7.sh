#!/bin/bash
progName=l7
echo 清理..
rm ${progName}.out
echo 正在编译..
g++ ${progName}.cpp -o ${progName}.out -lGL -lglfw -lGLEW -lSOIL -lassimp
echo 运行..
./${progName}.out
