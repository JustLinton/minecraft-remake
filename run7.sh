#!/bin/bash
progName=l7
echo 正在编译..
g++ ${progName}.cpp -o ${progName} -lGL -lglfw -lGLEW -lSOIL -lassimp
echo 运行..
./${progName}
