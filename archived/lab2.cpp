#include<bits/stdc++.h>
#include<GL/glut.h>
using namespace std;

const int INF=99999999;

struct Line{
	int fx,fy,tx,ty;
	int newfx,newfy,newtx,newty;
	Line(int fx_,int fy_,int tx_,int ty_):fx(fx_),fy(fy_),tx(tx_),ty(ty_){}
};
struct position
{
	int x,y;
	position(int x_,int y_):x(x_),y(y_){}
};

vector<Line> lines;
int cntr=0,mode=0; //mode:0 draw lines; 1 draw rect.
int xx1,xx2,yy1,yy2;
int xmin=INF, xmax=-INF, ymin=INF, ymax=-INF;
bool areaCreated=false;

void myInit()
{
	glClearColor(0.8, 0.6, 0.7, 0.0);
	glColor3f(0.5f, 0.4f, 0.9f);
	glPointSize(5.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluOrtho2D(0.0, 640, 0.0, 480);
}

void drawLineWithoutFlush(int x1,int y1,int x2,int y2){
	// glBegin(GL_LINES);
	glLineWidth(4.0);	
	glBegin(GL_LINE_STRIP);
	glVertex2i(x1, 480-y1); 
	glVertex2i(x2, 480-y2); 
	glEnd();

}

Line myclip(int x1,int x2, int y1,int y2);

void refreshGraphics(){
	glClear(GL_COLOR_BUFFER_BIT);

	for(auto ite=lines.begin();ite!=lines.end();ite++){
		glColor4f(1.0, 0.0, 0.0, 1.0);
		drawLineWithoutFlush(ite->fx,ite->fy,ite->tx,ite->ty);
		// cout<<ite->fx<<' '<<ite->fy<<' '<<ite->tx<<' '<<ite->ty<<endl;
		if(areaCreated){
			Line theClipped=myclip(ite->fx,ite->tx,ite->fy,ite->ty);
			glColor4f(1.0, 1.0, 1.0, 1.0);
			drawLineWithoutFlush(theClipped.fx,theClipped.fy,theClipped.tx,theClipped.ty);
		}
	}
	if(areaCreated){
		glColor4f(1.0, 1.0, 0.0, 1.0);
		drawLineWithoutFlush(xmin,ymin,xmin,ymax);
		drawLineWithoutFlush(xmax,ymin,xmax,ymax);
		drawLineWithoutFlush(xmin,ymin,xmax,ymin);
		drawLineWithoutFlush(xmin,ymax,xmax,ymax);
	}
	glFlush();
	
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	glVertex2i(1, 1);
	glEnd();
	glFlush();
	refreshGraphics();
}


void mymenu(int value){
	refreshGraphics();
	
	if (value == 1){
		lines.clear();
		areaCreated=false; //取消创建了的区域
		xmin=INF; xmax=-INF; ymin=INF; ymax=-INF;
		glClearColor(0.8, 0.6, 0.7, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glFlush();
	}

	if (value == 2){
		cntr=0;
		mode=0;
	}

	if (value == 3){
		cntr=0;
		mode=1;
	}

	if (value == 4){
		exit(0);
	}

}


void myMouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			if(mode==0){
				//draw lines
				cntr++;
				if(cntr%2){
					xx1=x;yy1=y;
			
				}else{
					xx2=x;yy2=y;
					lines.emplace_back(xx1,yy1,xx2,yy2);
				}
				refreshGraphics();//如果不刷新，不会立即显示此线
			}else if(mode==1){
				//draw rect
				cntr++;
				if(cntr%2) {
					areaCreated=false;
					xmin=INF; xmax=-INF; ymin=INF; ymax=-INF;
					xx1=x;yy1=y;
				}else{
					xx2=x;yy2=y;
					xmin=min(xx1,xx2);
					xmax=max(xx1,xx2);
					ymin=min(yy1,yy2);
					ymax=max(yy1,yy2);
					areaCreated=true;
				}
				// cout<<cntr<<' '<<cntr%2<<' '<<areaCreated<<endl;
				// cout<<"xmin"<<xmin<<" xmax"<<xmax<<" ymin"<<ymin<<" ymax"<<ymax<<endl;
				refreshGraphics();//如果不刷新，不会立即显示此线
			}
	
		}

	}

}
int Clip(float p, float q, float* tL, float* tU)
{
	int flag = 1;/*flag为标志变量0表示舍弃1表示可见*/
	float r;

	if (p < 0.0)
	{//对入边进行考虑。因为入边是p1，p3，分别等于(-dx)，(-dy)，所以这里判断<0就是对入边进行考虑。
		r = q / p;//算出和边界延长线交点对应的参数u值，记为r
		if (r > *tU)//u1>u2,排除（因为必然整条直线都在区域外。）
			flag = 0;
		else if (r > *tL) {
			*tL = r;/*m取进入点最大参数值*/
		}
	}
	else if (p > 0.0) {
		//对出边进行考虑。因为入边是p2，p4，分别等于(+dx)，(+dy)，所以这里判断>0就是对入边进行考虑。
		r = q / p;
		if (r < *tL)//u1>u2
			flag = 0;
		else if (r < *tU) {
			*tU = r;/*n取离开点的最小值*/
		}
	}

	else if (q < 0 && p == 0) //根据我的推导，p=0则线与边界平行；q<0则直线完全在线外。不用裁剪，flag=0。
		flag = 0;
	return flag;
}

Line myclip(int x1,int x2, int y1,int y2)
// line clipping algorithm 
{
	float dx, dy, tL, tU;
	tL = 0, tU = 1.0;

	dx = x2 - x1;
	//tL(即“左”)=new (x1,y1)'s u,即得出的(x1,y1)的最终的线段端点的参数， tU（即“上”）=new (x2,y2)'s u，即得出的(x2,y2)的最终的线段端点的参数。
	//对四个qi，pi进行判断
	if (Clip(-dx, x1 - xmin, &tL, &tU))
		if (Clip(dx, xmax - x1, &tL, &tU)) {
			dy = y2 - y1;
			if (Clip(-dy, y1 - ymin, &tL, &tU))
				if (Clip(dy, ymax - y1, &tL, &tU))
				{
					if (tU < 1.0)
					{
						x2 = x1 + tU * dx;//直接根据参数方程计算出边交点
						y2 = y1 + tU * dy;
					}
					if (tL > 0.0)
					{
						x1 = x1 + tL * dx;//直接根据参数方程计算入边交点
						y1 = y1 + tL * dy;
					}
					//得出裁剪后的线段，返回。
					return Line(x1,y1,x2,y2);
				}
		}
	return Line(0,0,0,0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("sfere");

	myInit();
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouse);
	glutCreateMenu(mymenu);//注册菜单回调函数
 
	glutAddMenuEntry("Clear",1);//添加菜单项
	glutAddMenuEntry("Draw Lines",2);//添加菜单项
	glutAddMenuEntry("Set Area",3);//添加菜单项
	glutAddMenuEntry("Exit",4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);//把当前菜单注册到指定的鼠标键
	
	glutMainLoop();
}
