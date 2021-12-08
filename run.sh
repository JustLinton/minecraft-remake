#!/bin/bash
progName=texttest
echo 正在编译..
g++ ${progName}.cpp -o ${progName} -lGL -lglfw -lGLEW -lSOIL -lfreetype
echo 运行..
./${progName}
