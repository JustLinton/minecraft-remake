#include <bits/stdc++.h>
#include <GL/glut.h>
#include <GLES3/gl3.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace std;

// 顶点坐标 / 颜色
std::vector<glm::vec3> points, colors;

GLuint program; // 着色器程序对象
glm::vec3 scaleControl(1,1,1);  // 缩放控制
glm::vec3 rotateControl(0, 0, 0);   // 旋转控制
glm::vec3 translateControl(0, 0, 0);    // 平移控制

int windowWidth = 512;  // 窗口宽
int windowHeight = 512; // 窗口高

bool keyboardState[1024];   // 键盘状态数组 keyboardState[x]==true 表示按下x键

// --------------- end of global variable definition --------------- //

// 读取文件并且返回一个长字符串表示文件内容
std::string readShaderFile(std::string filepath)
{
    std::string res, line;
    std::ifstream fin(filepath);
    if (!fin.is_open())
    {
        std::cout << "文件 " << filepath << " 打开失败" << std::endl;
        exit(-1);
    }
    while (std::getline(fin, line))
    {
        res += line + '\n';
    }
    fin.close();
    return res;
}

// 获取着色器对象，是常用的一个过程，可以封装起来每次都调用
GLuint getShaderProgram(std::string fshader, std::string vshader)
{
    // 读取shader源文件
    std::string vSource = readShaderFile(vshader);
    std::string fSource = readShaderFile(fshader);
    const char* vpointer = vSource.c_str();
    const char* fpointer = fSource.c_str();

    // 容错
    GLint success;
    GLchar infoLog[512];

    // 创建并编译顶点着色器
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const GLchar**)(&vpointer), NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);   // 错误检测
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "顶点着色器编译错误\n" << infoLog << std::endl;
        exit(-1);
    }

    // 创建并且编译片段着色器
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const GLchar**)(&fpointer), NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);   // 错误检测
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "片段着色器编译错误\n" << infoLog << std::endl;
        exit(-1);
    }

    // 链接两个着色器到program对象
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // 删除着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

// 读取off文件并且生成最终传递给顶点着色器的 顶点位置/顶点颜色
void readOff(std::string filepath, std::vector<glm::vec3>& points, std::vector<glm::vec3>& colors)
{
    std::string line;
    std::ifstream fin(filepath);
    if (!fin.is_open())
    {
        std::cout << "文件 " << filepath << " 打开失败" << std::endl;
        exit(-1);
    }

    std::vector<glm::vec3> vertexPosition, vertexColor; // off模型顶点位置和颜色
    fin >> line;    // OFF字符串
    // 读取各数量
    int vertexNum, faceNum, edgeNum;
    fin >> vertexNum >> faceNum >> edgeNum;

    //输入各顶点的坐标
    for (int i = 0; i < vertexNum; i++)
    {
        float p1, p2, p3;
        fin >> p1 >> p2 >> p3;
        vertexPosition.push_back(glm::vec3(p1, p2, p3));
        vertexColor.push_back(glm::vec3(p1 + 0.5, p2 + 0.5, p3 + 0.5)); // 用位置做颜色
    }
    // 根据面信息生成实际顶点
    points.clear();
    colors.clear();
    // 输入各面的分别的三个顶点
    for (int i = 0; i < faceNum; i++)
    {
        int n, index1, index2, index3;
        fin >> n >> index1 >> index2 >> index3;
        points.push_back(vertexPosition[index1]);
        points.push_back(vertexPosition[index2]);
        points.push_back(vertexPosition[index3]);
        colors.push_back(vertexColor[index1]);
        colors.push_back(vertexColor[index2]);
        colors.push_back(vertexColor[index3]);
    }
}

// 初始化
void init()
{
    // 读取off模型并且生成顶点和顶点颜色
    readOff("./models/bunny.off", points, colors);
    

    // 生成vbo对象
    GLuint vbo;
    glGenBuffers(1, &vbo);

    //指定使用vbo作为buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    //开辟vbo
    // 是一个GL_ARRAY_BUFFER，其大小是sizeof(glm::vec3) * (points.size() + colors.size())，待放入的数据的指针暂时是NULL，因为我们这里只是先开起来。
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * (points.size() + colors.size()), NULL, GL_STATIC_DRAW);

    // 传送数据到vbo 分别传递 顶点位置 和 顶点颜色
    GLuint pointDataOffset = 0;
    GLuint colorDataOffset = sizeof(glm::vec3) * points.size();
    //glbuffersubdata用于部分更新vbo中的数据。需要传入base和limit，并且给出需要写入的数据源的指针。
    //是一个GL_ARRAY_BUFFER，其base是pointDataOffset，其limit是sizeof(glm::vec3) * points.size(),其源数据是points、colors。
    glBufferSubData(GL_ARRAY_BUFFER, pointDataOffset, sizeof(glm::vec3) * points.size(), &points[0]);
    glBufferSubData(GL_ARRAY_BUFFER, colorDataOffset, sizeof(glm::vec3) * colors.size(), &colors[0]);

    // 生成vao对象
    GLuint vao;
    glGenVertexArrays(1, &vao);

    //指定使用vao为vao
    glBindVertexArray(vao);

    // 生成着色器程序对象
    std::string fshaderPath = "./shaders/fshader.fsh";
    std::string vshaderPath = "./shaders/vshader.vsh";
    program = getShaderProgram(fshaderPath, vshaderPath);

    //使用program作为着色器程序
    glUseProgram(program); 

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);  

    glEnableVertexAttribArray(1);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(glm::vec3) * points.size()));  // 注意指定offset参数

    glEnable(GL_DEPTH_TEST);  // 开启深度测试

    glClearColor(0.0, 0.0, 0.0, 1.0);   // 背景颜色 -- 黑
}

void zoom(int direction)
{
    scaleControl += 1 * direction * 0.01;
    glutPostRedisplay();    // 重绘
}

// 鼠标运动函数
void mouse(int x, int y)
{
    // 调整旋转
    rotateControl.y = -100 * (x - float(windowWidth) / 2.0) / windowWidth;
    rotateControl.x = -100 * (y - float(windowHeight) / 2.0) / windowHeight;

    glutPostRedisplay();    // 重绘
}

// 键盘回调函数
void keyboardDown(unsigned char key, int x, int y)
{
    keyboardState[key] = true;
}
void keyboardUp(unsigned char key, int x, int y)
{
    keyboardState[key] = false;
}
// 根据键盘状态判断移动
void move()
{
    if (keyboardState['w']) translateControl.y += 0.0005;
    if (keyboardState['s']) translateControl.y -= 0.0005;
    if (keyboardState['a']) translateControl.x -= 0.0005;
    if (keyboardState['d']) translateControl.x += 0.0005;
    if (keyboardState['-']) zoom(-1);
    if (keyboardState['=']) zoom(1);
    glutPostRedisplay();    // 重绘
}

// 显示回调函数
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);       // 清空窗口颜色缓存

    move(); // 移动控制

    // 构造模型变换矩阵
    glm::mat4 unit(    // 单位矩阵
        glm::vec4(1, 0, 0, 0),
        glm::vec4(0, 1, 0, 0),
        glm::vec4(0, 0, 1, 0),
        glm::vec4(0, 0, 0, 1)
    );
    glm::mat4 scale = glm::scale(unit, scaleControl);    // xyz缩放
    glm::mat4 translate = glm::translate(unit, translateControl);   // 平移
    glm::mat4 rotate = unit;    // 旋转
    rotate = glm::rotate(rotate, glm::radians(rotateControl.x), glm::vec3(1, 0, 0));   // 绕x轴转
    rotate = glm::rotate(rotate, glm::radians(rotateControl.y), glm::vec3(0, 1, 0));   // 绕y轴转
    rotate = glm::rotate(rotate, glm::radians(rotateControl.z), glm::vec3(0, 0, 1));   // 绕z轴转
    glm::mat4 model = translate * rotate * scale;   // 变换级联 -- 生成模型变换矩阵

    // 传递uniform变量
    GLuint mlocation = glGetUniformLocation(program, "model");    // 名为model的uniform变量的位置索引
    //注意，uniform不能使用layout location，所以上面只能采用glGetUniformLocation来获得索引。
    glUniformMatrix4fv(mlocation, 1, GL_FALSE, glm::value_ptr(model));   // 列优先矩阵

    glDrawArrays(GL_TRIANGLES, 0, points.size());   // 绘制n个点

    //如果不使用双缓冲，就会出现这一帧绘制在上一帧之上的情况，造成画面撕裂
    glutSwapBuffers();                  // 交换缓冲区
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);              // glut初始化
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);// 窗口大小
    glutCreateWindow("201900302022 - control"); // 创建OpenGL上下文

    init();

    // 绑定鼠标移动函数 -- 
    glutMotionFunc(mouse);  // 左键按下并且移动
//     glutMouseWheelFunc(mouseWheel); // 滚轮缩放

    // 绑定键盘函数
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);

    glutDisplayFunc(display);           // 设置显示回调函数 -- 每帧执行
    glutMainLoop();                     // 进入主循环

    return 0;
}

