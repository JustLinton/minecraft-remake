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
const int ssaa=2;//SSAA超采样的倍数
int Ymax;

struct Point{
    double x,y;
    Point(double x_,double y_):x(x_),y(y_){}
};
vector<Point> pv;
vector<double> T;

double tao(const int& r,const double & t,const int& i,const int& k,const vector<double>& T){
//    cout<<"     "<<(t-T[i])/(T[i+k-r]-T[i])<<endl;
	return (t-T[i])/(T[i+k-r]-T[i]);
}

double deBoor(const int& r,const double & t,const int& i,const int& k,bool isX,const vector<Point>& pv,const vector<double>& T){
	if(r==0)return isX ? pv.at(i).x : pv.at(i).y;
	//    cout<<tao(r,t,i,k,T)<<endl;
	return (1-tao(r,t,i,k,T))* deBoor(r-1,t,i-1,k,isX,pv,T)+tao(r,t,i,k,T)*deBoor(r-1,t,i,k,isX,pv,T);
}

vector<Point> callDeBoor(){
	for(int i=0;i<=10;i++) T.push_back(i/2.0);
	pv.emplace_back(1,1);
	pv.emplace_back(2,2);
	pv.emplace_back(3,1);
	pv.emplace_back(4,2);
	pv.emplace_back(5,1);
	pv.emplace_back(6,2);
	pv.emplace_back(7,1);
	int k=3;
	for(int j=k-1;j<7;j++)
		for(double t=T[j];t<T[j+1];t+=0.1){
	//            deBoor(k-1,t,j,k,false,pv,T);
		cout<< "t: "<<t<<' '<<deBoor(k-1,t,j,k,true,pv,T)<<endl;
	//            cout<<deBoor(k-1,t,j,k,true,pv,T)<<endl;
		}
}


void myDisp(){
    glClear(GL_COLOR_BUFFER_BIT);
    //开始绘制
    glPointSize(6);
    glBegin(GL_POINTS);
    glColor3f(1,0,0);
    //屏幕中心画点
    glVertex2i(centX,centY);
    glEnd();
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
    glutDisplayFunc(myDisp);
    glutMainLoop();
    return 0;
}
