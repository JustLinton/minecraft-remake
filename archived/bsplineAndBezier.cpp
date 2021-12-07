#include<cstdio>
#include<iostream>
#include<algorithm>
#include<vector>
#include<cmath>
#include <GL/glut.h>
using namespace std;

const int polygon_num=2;
const int winwidth=600,winheight=600;
const int centX=winwidth*0.5,centY=winheight*0.5;

const double bSFDF=2.0;//bSplineFactorDistanceFactor,即参数均匀分布的B样条曲线，对于任意i，其i到i+1的距离=1/bSFDF。
const double renderRate=0.005;//渲染间隔。即每个渲染点之间的距离。

struct Point{
    double x,y;
    Point(double x_,double y_):x(x_),y(y_){}
};
vector<Point> pv;
vector<double> T;


double tao(const int& r,const double & t,const int& i,const int& k,const vector<double>& T){
	return (t-T[i])/(T[i+k-r]-T[i]);
}

double deBoor(const int& r,const double & t,const int& i,const int& k,bool isX,const vector<Point>& pv,const vector<double>& T){
	if(r==0)return isX ? pv.at(i).x : pv.at(i).y;
	return (1-tao(r,t,i,k,T))* deBoor(r-1,t,i-1,k,isX,pv,T)+tao(r,t,i,k,T)*deBoor(r-1,t,i,k,isX,pv,T);
}

double bezier(const int& n,const double& t,const int& k,const bool& isX,const vector<Point>& pv){
	if(k==0)return isX ? pv[n].x : pv[n].y;
	return (1-t) * bezier(n,t,k-1,isX,pv) + t* bezier(n+1,t,k-1,isX,pv);
}

vector<Point> calBezier(const vector<Point>& pv,const int& k){
	vector<Point> res;
	if(pv.size()<k+1)return res;
	for(double t=0.0;t<=1.0;t+=renderRate){
		res.emplace_back(bezier(0,t,k,true,pv),bezier(0,t,k,false,pv));
	}
	return res;
}

vector<Point> callDeBoor(const vector<Point>& pv,const vector<double>& T,const int& k){
	vector<Point> res;
	for(int j=k-1;j<pv.size();j++)
		for(double t=T[j];t<T[j+1];t+=renderRate){
			res.emplace_back(deBoor(k-1,t,j,k,true,pv,T),deBoor(k-1,t,j,k,false,pv,T));
		}
	return res;
}

void drawBezier(){
	glClear(GL_COLOR_BUFFER_BIT);
	//开始绘制
	glPointSize(4);
	glBegin(GL_POINTS);
	glColor3f(1,0,0);
	//屏幕中心画点
	// glVertex2i(centX,centY);
	vector<Point> vpp = calBezier(pv,3);
	for(auto ite=pv.begin();ite!=pv.end();ite++)glVertex2i(ite->x,+ite->y);
	glColor3f(0,1,0);
	for(auto ite=vpp.begin();ite!=vpp.end();ite++)glVertex2i(ite->x,+ite->y);
	glEnd();
	glFlush();
}

void drawBSpline(){
	T.clear();
	for(int i=0;i<=pv.size()+1;i++) T.push_back(i/bSFDF);
	glClear(GL_COLOR_BUFFER_BIT);
	//开始绘制
	glPointSize(4);
	glBegin(GL_POINTS);
	glColor3f(1,0,0);
	//屏幕中心画点
	// glVertex2i(centX,centY);
	vector<Point> vpp = callDeBoor(pv,T,3);
	for(auto ite=pv.begin();ite!=pv.end();ite++)glVertex2i(ite->x,+ite->y);
	glColor3f(0,1,0);
	for(auto ite=vpp.begin();ite!=vpp.end();ite++)glVertex2i(ite->x,+ite->y);
	glEnd();
	glFlush();
}

void refreshGraphics(){
	if(!pv.empty()){
		drawBSpline();
		// drawBezier();
	}

}

void myMouse(int button, int state, int mouse_x, int mouse_y)
{
	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
		{	
			int x=mouse_x,y=winheight-mouse_y;
			pv.emplace_back(x,y);
			cout<<pv.size()<<endl;
			refreshGraphics();
		}
	}

}

void mymenu(int value){
	refreshGraphics();
	
	if (value == 1){
		exit(0);
	}

	if (value == 2){
		exit(0);
	}

}

void myDisp(){
	glClear(GL_COLOR_BUFFER_BIT);
	refreshGraphics();
	glFlush();
}

void init(){
	glClearColor(1,1,1,0);//设置背景色为黑色
	glClear(GL_COLOR_BUFFER_BIT);//将屏幕上的所有像素点置为预置色
	gluOrtho2D(0,winwidth,0,winheight);//变换坐标系到屏幕坐标系
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc,char* argv[]){
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(winwidth,winheight);
	glutInitWindowSize(winwidth,winheight);
	glutCreateWindow("201900302022 - lab4");
	init();
	glutMouseFunc(myMouse);

	glutDisplayFunc(myDisp);

	glutCreateMenu(mymenu);//注册菜单回调函数
	glutAddMenuEntry("Exit",1);//添加菜单项
	glutAddMenuEntry("Exit",2);//添加菜单项
	glutAttachMenu(GLUT_RIGHT_BUTTON);//把当前菜单注册到指定的鼠标键

	glutMainLoop();
    return 0;
}
