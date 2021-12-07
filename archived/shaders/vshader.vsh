#version 330 core

layout (location = 0) in vec3 vPosition;  // cpu传入的顶点坐标
layout (location = 1) in vec3 vColor;     // cpu传入的顶点颜色

out vec3 vColorOut; // 向片元着色器传递顶点颜色

uniform mat4 model; // 模型变换矩阵

void main()
{
    gl_Position = model * vec4(vPosition, 1.0); // 指定ndc坐标
    vColorOut = vColor; // 这个颜色经过线性插值生成片元颜色
}
