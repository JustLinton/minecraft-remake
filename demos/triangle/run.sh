#!/bin/bash
progName=l6
echo 正在编译..
g++ ${progName}.cpp -o ${progName} -lGL -lglfw -lGLEW
./${progName}
