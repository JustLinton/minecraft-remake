#!/bin/bash
progName=glfwtest
echo 正在编译..
g++ ${progName}.cpp -o ${progName} -lglfw
echo 运行..
./${progName}
