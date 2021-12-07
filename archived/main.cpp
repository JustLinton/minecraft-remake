#include <bits/stdc++.h>
#include <GL/glut.h>
#include <GLES3/gl3.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define OFFFILEBUNNY "./models/bunny.off"
#define OFFFILEBUMPY "./models/bumpy.off"

using namespace std;

int numofvertex = 0;
int numofface = 0;
int numofline = 0;

int WIDTH = 600;
int HEIGHT = 600;
float maxx = -1;
float maxy = -1;
float maxz = -1;
float minx = 1;
float miny = 1;
float minz = 1;

GLfloat ShootPosition[] = { 0,0,0 };
GLfloat ShootDirect[] = { 0,0,0 };

float scale = 1.0;
float px;
float py;
float theta1 = 0;
float theta2 = 0;
float radius = 0;

int displaystate = 0;
float PI = 3.1415926;

struct vertex;
struct face;
struct halfedge;
struct he_face;
struct normalVec;
struct iedge;
vertex* vertexs;
face* faces;
he_face** hefaces;
normalVec* normalvectors;
iedge** iedges;

struct halfedge {
	halfedge* next;
	halfedge* opposite;
	int end;
	bool visit;
	he_face* face;
	halfedge() {
		next = NULL;
		opposite = NULL;
		end = -1;
		face = NULL;
		visit = false;
	}
};

struct vertex {

	float x;
	float y;
	float z;

	halfedge* edge;

	bool visit;
	vertex() {
		visit = false;
	}

	vertex(float a, float b, float c) {
		x = a;
		y = b;
		z = c;
		edge = NULL;
		visit = false;
	}
};

struct normalVec {
	float x;
	float y;
	float z;
	normalVec() {

	}
	normalVec(float a, float b, float c) {
		x = a;
		y = b;
		z = c;
	}
};

struct he_face {

	halfedge* edge;

	bool visit;
	he_face() {
		edge = NULL;
		visit = false;
	}
};

struct face {

	int numofv;
	int* vertexs;
	face() {

	}
	face(int nv) {
		numofv = nv;
		vertexs = new int[nv];
	}
};

struct iedge {

	int start;
	int middle;
	halfedge* he;
	iedge* next;
	iedge() {
		start = -1;
		he = NULL;
		next = NULL;
		middle = -1;
	}
};


int getMiddle(int start, int end, iedge** iedges) {
	iedge* temp = iedges[start];
	while (temp != NULL) {
		if (temp->he->end == end) {
			break;
		}
		temp = temp->next;
	}
	if (temp == NULL) {
		return -1;
	}
	else {
		return temp->middle;
	}
}


halfedge* getHalfEdge(int start, int end, iedge** iedges) {
	iedge* temp = iedges[start];
	while (temp != NULL) {
		if (temp->he->end == end) {
			break;
		}
		temp = temp->next;
	}
	if (temp == NULL) {
		return NULL;
	}
	else {
		return temp->he;
	}

}


void initHalfedge() {
	hefaces = new he_face*[numofface];
	int numofhe = 0;
	for (int i = 0; i < numofvertex; i++)
	{
		iedges[i] = NULL;
	}

	for (int i = 0; i < numofface; i++)
	{

		int v1 = faces[i].vertexs[0];
		int v2 = faces[i].vertexs[1];
		int v3 = faces[i].vertexs[2];

		he_face* hf = new he_face();

		halfedge* he1 = getHalfEdge(v1, v2, iedges);
		halfedge* he2 = getHalfEdge(v2, v3, iedges);
		halfedge* he3 = getHalfEdge(v3, v1, iedges);

		if (he1 == NULL) {  
			he1 = new halfedge();
			he1->end = v2;
			halfedge* opposite1 = new halfedge();
			opposite1->end = v1;
			opposite1->opposite = he1;
			he1->opposite = opposite1;

			iedge* temp = iedges[v1];
			iedge* tempi = new iedge();
			tempi->start = v1;
			tempi->he = he1;
			if (temp == NULL) {
				iedges[v1] = tempi;
			}
			else {
				while (temp->next != NULL) {
					temp = temp->next;
				}
				temp->next = tempi;
			}


			temp = iedges[v2];
			tempi = new iedge();
			tempi->start = v2;
			tempi->he = opposite1;
			if (temp == NULL) {
				iedges[v2] = tempi;
			}
			else {
				while (temp->next != NULL) {
					temp = temp->next;
				}
				temp->next = tempi;
			}

		}

		if (he2 == NULL) {
			he2 = new halfedge();
			he2->end = v3;
			halfedge* opposite2 = new halfedge();
			opposite2->end = v2;
			opposite2->opposite = he2;
			he2->opposite = opposite2;

			iedge* temp = iedges[v2];
			iedge* tempi = new iedge();
			tempi->start = v2;
			tempi->he = he2;
			if (temp == NULL) {
				iedges[v2] = tempi;
			}
			else {
				while (temp->next != NULL) {
					temp = temp->next;
				}
				temp->next = tempi;
			}


			temp = iedges[v3];
			tempi = new iedge();
			tempi->start = v3;
			tempi->he = opposite2;
			if (temp == NULL) {
				iedges[v3] = tempi;
			}
			else {
				while (temp->next != NULL) {
					temp = temp->next;
				}
				temp->next = tempi;
			}
		}

		if (he3 == NULL) {
			he3 = new halfedge();
			he3->end = v1;
			halfedge* opposite3 = new halfedge();
			opposite3->end = v3;
			opposite3->opposite = he3;
			he3->opposite = opposite3;


			iedge* temp = iedges[v3];
			iedge* tempi = new iedge();
			tempi->start = v3;
			tempi->he = he3;
			if (temp == NULL) {
				iedges[v3] = tempi;
			}
			else {
				while (temp->next != NULL) {
					temp = temp->next;
				}
				temp->next = tempi;
			}


			temp = iedges[v1];
			tempi = new iedge();
			tempi->start = v1;
			tempi->he = opposite3;
			if (temp == NULL) {
				iedges[v1] = tempi;
			}
			else {
				while (temp->next != NULL) {
					temp = temp->next;
				}
				temp->next = tempi;
			}
		}

		he1->next = he2;
		he2->next = he3;
		he3->next = he1;

		hf->edge = he1;

		he1->face = hf;
		he2->face = hf;
		he3->face = hf;

		if (vertexs[v1].edge == NULL)
			vertexs[v1].edge = he1;
		if (vertexs[v2].edge == NULL)
			vertexs[v2].edge = he2;
		if (vertexs[v3].edge == NULL)
			vertexs[v3].edge = he3;

		hefaces[i] = hf;
	}

}

void insertIedge(iedge** iedges, iedge* ie) {
	iedge* it = iedges[ie->start];
	if (it == NULL) {
		iedges[ie->start] = ie;
	}
	else {
		while (it->next != NULL)
		{
			it = it->next;
		}
		it->next = ie;
	}
}

void loopDivision() {

	vertex* newvertexs = new vertex[numofvertex + (int)(1.5*numofface)+1];
	he_face** nfaces = new he_face*[4 * numofface];
	iedge** niedges = new iedge*[numofvertex +  (int)(1.5*numofface)+1];
	int numoff = 0;
	int numofv = 0;

	for (int i = 0; i < numofvertex + 1.5*numofface; i++)
	{
		niedges[i] = NULL;
	}

	for (int i = 0; i < numofvertex; i++)
	{
		int n = 0;
		halfedge* hedge = vertexs[i].edge;

		do
		{
			n++;
			hedge = hedge->opposite->next;
		} while (hedge != vertexs[i].edge);

		float beta = (5.0 / 8 - pow((3.0 / 8 + cos(2 * PI / n) / 4), 2)) / n;
		float sumpx = 0;
		float sumpy = 0;
		float sumpz = 0;

		do
		{
			sumpx += vertexs[hedge->end].x;
			sumpy += vertexs[hedge->end].y;
			sumpz += vertexs[hedge->end].z;
			hedge = hedge->opposite->next;
		} while (hedge != vertexs[i].edge);

		float npx = (1 - n * beta)*vertexs[i].x + beta * sumpx;
		float npy = (1 - n * beta)*vertexs[i].y + beta * sumpy;
		float npz = (1 - n * beta)*vertexs[i].z + beta * sumpz;
		vertex nv = vertex();
		nv.x = npx;
		nv.y = npy;
		nv.z = npz;
		nv.edge = vertexs[i].edge;
		newvertexs[i] = nv;
		numofv++;
	}

	for (int i = 0; i < numofface; i++)
	{
		halfedge* fedge = hefaces[i]->edge;
		do
		{
			int v0 = fedge->next->next->end;
			int v1 = fedge->end;
			int v2 = fedge->opposite->next->end;
			int v3 = fedge->next->end;
			iedge* it = iedges[v0];
			int middle = -1;
			middle = getMiddle(v0, v1, iedges);
			//���㲢�����¶���
			if (middle == -1) {
				vertex newv = vertex();
				newv.x = 3 * (vertexs[v0].x + vertexs[v1].x) / 8 + (vertexs[v2].x + vertexs[v3].x) / 8;
				newv.y = 3 * (vertexs[v0].y + vertexs[v1].y) / 8 + (vertexs[v2].y + vertexs[v3].y) / 8;
				newv.z = 3 * (vertexs[v0].z + vertexs[v1].z) / 8 + (vertexs[v2].z + vertexs[v3].z) / 8;

				newvertexs[numofv] = newv;

				iedge* itemp = iedges[v0];
				while (itemp != NULL) {
					if (itemp->he == fedge) {
						itemp->middle = numofv;
						break;
					}
					itemp = itemp->next;
				}
				itemp = iedges[v1];
				while (itemp != NULL) {
					if (itemp->he == fedge->opposite) {
						itemp->middle = numofv;
						break;
					}
					itemp = itemp->next;
				}

				middle = numofv;
				numofv++;
			}

			//�����µİ�߽ṹ
			halfedge* newhe1 = new halfedge();
			halfedge* newhe2 = new halfedge();
			newhe1->end = getMiddle(v0, v1, iedges);
			iedge* newie1 = new iedge();
			newie1->start = v0;
			newie1->he = newhe1;
			insertIedge(niedges, newie1);
			halfedge* op1 = getHalfEdge(middle, v0, niedges);
			newhe1->opposite = op1;
			if (op1 != NULL)
			{
				op1->opposite = newhe1;
			}

			newhe2->end = v1;
			iedge* newie2 = new iedge();
			newie2->start = middle;
			newie2->he = newhe2;
			insertIedge(niedges, newie2);
			halfedge* op2 = getHalfEdge(v1, middle, niedges);
			newhe2->opposite = op2;
			if (op2 != NULL)
			{
				op2->opposite = newhe2;
			}


			newvertexs[v0].edge = newhe1;
			newvertexs[middle].edge = newhe2;


			fedge = fedge->next;
		} while (fedge != hefaces[i]->edge);

		do
		{
			int v0 = fedge->next->next->end;
			int v1 = fedge->end;
			int v3 = fedge->next->end;
			int middle1 = getMiddle(v0, v1, iedges);
			int middle2 = getMiddle(v3, v0, iedges);

			halfedge* middlehe = new halfedge();
			middlehe->end = middle2;
			halfedge* fhe1 = getHalfEdge(v0, middle1, niedges);
			fhe1->next = middlehe;
			halfedge* fhe2 = getHalfEdge(middle2, v0, niedges);
			middlehe->next = fhe2;
			fhe2->next = fhe1;
			he_face* nface = new he_face();
			nface->edge = fhe1;
			nfaces[numoff] = nface;
			numoff++;
			fhe1->face = nface;
			fhe2->face = nface;
			middlehe->face = nface;

			iedge* middleiedge = new iedge();
			middleiedge->start = middle1;
			middleiedge->he = middlehe;
			insertIedge(niedges, middleiedge);

			fedge = fedge->next;
		} while (fedge != hefaces[i]->edge);

		int middle1 = -1;
		int middle2 = -1;
		int middle3 = -1;
		middle1 = getMiddle(fedge->next->next->end, fedge->end, iedges);
		middle2 = getMiddle(fedge->end, fedge->next->end, iedges);
		middle3 = getMiddle(fedge->next->end, fedge->next->next->end, iedges);

		halfedge* middlefhe1 = new halfedge();
		halfedge* middlefhe2 = new halfedge();
		halfedge* middlefhe3 = new halfedge();

		middlefhe1->end = middle1;
		middlefhe2->end = middle2;
		middlefhe3->end = middle3;

		middlefhe1->next = middlefhe2;
		middlefhe2->next = middlefhe3;
		middlefhe3->next = middlefhe1;

		halfedge* middleo1 = getHalfEdge(middle1, middle3, niedges);
		middleo1->opposite = middlefhe1;
		middlefhe1->opposite = middleo1;
		halfedge* middleo2 = getHalfEdge(middle2, middle1, niedges);
		middleo2->opposite = middlefhe2;
		middlefhe2->opposite = middleo2;
		halfedge* middleo3 = getHalfEdge(middle3, middle2, niedges);
		middleo3->opposite = middlefhe3;
		middlefhe3->opposite = middleo3;

		he_face* middlehf = new he_face();
		middlehf->edge = middlefhe1;
		middlefhe1->face = middlehf;
		middlefhe2->face = middlehf;
		middlefhe3->face = middlehf;
		nfaces[numoff] = middlehf;
		numoff++;

		iedge* mie1 = new iedge();
		mie1->start = middle3;
		mie1->he = middlefhe1;
		insertIedge(niedges, mie1);

		iedge* mie2 = new iedge();
		mie2->start = middle1;
		mie2->he = middlefhe2;
		insertIedge(niedges, mie2);

		iedge* mie3 = new iedge();
		mie3->start = middle2;
		mie3->he = middlefhe3;
		insertIedge(niedges, mie3);

	}

	free(vertexs);
	vertexs = newvertexs;
	numofvertex = numofv;
	free(hefaces);
	hefaces = nfaces;

	numofface = numoff;
	free(iedges);
	iedges = niedges;

}


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
    std::string gSource = readShaderFile("./shaders/gshader.gsh");
    const char* vpointer = vSource.c_str();
    const char* fpointer = fSource.c_str();
    const char* gpointer = gSource.c_str();

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


    // 创建并编译几何着色器
    GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometryShader, 1, (const GLchar**)(&gpointer), NULL);
    glCompileShader(geometryShader);
    glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);   // 错误检测
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "几何着色器编译错误\n" << infoLog << std::endl;
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
//     glAttachShader(shaderProgram, geometryShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // 删除着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(geometryShader);

    return shaderProgram;
}


void readFile(string inFile) {
	char data[100];
	ifstream infile;
	infile.open(inFile);
	infile >> data;
	infile >> numofvertex;
	infile >> numofface;
	infile >> numofline;
	vertexs = new vertex[numofvertex];
	faces = new face[numofface];
	int vnum = 0;
	int fnum = 0;

    //orig
    //  std::vector<glm::vec3> vertexPosition, vertexColor; // off模型顶点位置和颜色

    points.clear();
    colors.clear();

	while (vnum < numofvertex) {
		float x;
		float y;
		float z;
		infile >> x;
		infile >> y;
		infile >> z;
		vertexs[vnum] = vertex(x, y, z);
		vnum++;

        //orig
        // vertexPosition.push_back(glm::vec3(x, y, z));
        // vertexColor.push_back(glm::vec3(x + 0.5, y + 0.5, z + 0.5)); // 用位置做颜色
	}

	while (fnum<numofface)
	{
		int numofv;
		infile >> numofv;
		face f = face(numofv);
		for (int i = 0; i < numofv; i++)
		{
			int v;
			infile >> v;
			f.vertexs[i] = v;

            //orig
            // points.push_back(vertexPosition[v]);
            // colors.push_back(vertexColor[v]);
		}
		faces[fnum] = f;
		fnum++;
	}
	infile.close();
}


void modelToGPU(){
    //传递半边模型中存储的各顶点和面片给GPU

    points.clear();
    colors.clear();

    for (int i = 0; i < numofface; i++)
    {
        halfedge* e = hefaces[i]->edge;
        vertex v1 = vertexs[e->end];
        vertex v2 = vertexs[e->next->end];
        vertex v3 = vertexs[e->next->next->end];

        points.push_back(glm::vec3(v1.x, v1.y, v1.z));
        colors.push_back(glm::vec3(v1.x+0.5, v1.y+0.5, v1.z+0.5));

        points.push_back(glm::vec3(v2.x, v2.y, v2.z));
        colors.push_back(glm::vec3(v2.x+0.5, v2.y+0.5, v2.z+0.5));

        points.push_back(glm::vec3(v3.x, v3.y, v3.z));
        colors.push_back(glm::vec3(v3.x+0.5, v3.y+0.5, v3.z+0.5));
    }

      //开辟vbo
    // 是一个GL_ARRAY_BUFFER，其大小是sizeof(glm::vec3) * (points.size() + colors.size())，待放入的数据的指针暂时是NULL，因为我们这里只是先开起来。
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * (points.size() + colors.size()), NULL, GL_STATIC_DRAW);

    // 传送数据到vbo 分别传递 顶点位置 和 顶点颜色
    GLuint pointDataOffset = 0;
    GLuint colorDataOffset = sizeof(glm::vec3) * points.size();
    
    glBufferSubData(GL_ARRAY_BUFFER, pointDataOffset, sizeof(glm::vec3) * points.size(), &points[0]);
    glBufferSubData(GL_ARRAY_BUFFER, colorDataOffset, sizeof(glm::vec3) * colors.size(), &colors[0]);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);  
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(glm::vec3) * points.size()));  // 注意指定offset参数
}


// 初始化
void init()
{
    // 生成vbo对象
    GLuint vbo;
    glGenBuffers(1, &vbo);

    //指定使用vbo作为buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

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
    glEnableVertexAttribArray(1);

    glEnable(GL_DEPTH_TEST);  // 开启深度测试

    glClearColor(1.0, 1.0, 1.0, 1.0);   // 背景颜色

    readFile(OFFFILEBUNNY);
    // readFile(OFFFILEBUMPY);
    iedges = new iedge*[numofvertex];
    initHalfedge();

    //把读进来的半边模型的各顶点和面片先给GPU显示出来
    modelToGPU();
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
    if(keyboardState['0']) loopDivision();
    glutPostRedisplay();    // 重绘
}

// 显示回调函数
void display()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);       // 清空窗口颜色缓存

    //把模型的顶点信息和颜色信息先传递给GPU
    modelToGPU();

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

	// glColor3f(1.0, 0.0, 1.0);
	for (int i = 0; i < numofface; i++)
	{
		halfedge* e = hefaces[i]->edge;
		vertex v1 = vertexs[e->end];
		vertex v2 = vertexs[e->next->end];
		vertex v3 = vertexs[e->next->next->end];

		glEnable(GL_LIGHTING); 
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT, GL_AMBIENT);
		glColor3f(100/255.0, 181/255.0, 246/255.0);

		// glm::vec4 ambientColor(1.0f,0.0f,0.0f,1.0f);
		// glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, &ambientColor.x );

		// glColor3f(1,1,1);
		// glLineWidth(10);
		glBegin(GL_LINE_STRIP);
		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v2.x, v2.y, v2.z);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex3f(v2.x, v2.y, v2.z);
		glVertex3f(v3.x, v3.y, v3.z);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v3.x, v3.y, v3.z);
		glEnd();
		
	}
	glFlush();

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

