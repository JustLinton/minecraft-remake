#include <GL/glut.h>
#include <iostream>
#include <math.h>
using namespace std;
#define mPoint 1
// double x1,y1,x2,y2;
int xx1,yy1,xx2,yy2;
int cntr=0;
 
void plotPoint(double x_,double y_){
	glPointSize(3);
	glBegin(GL_POINTS);
	glColor3f(1.0,1.0,1.0);
	glVertex2f(x_,y_);
		// cout<<(x-250)/250.0<<' '<<-(y-250)/250.0<<endl;
	glEnd();
	glFlush();
}

void plotIntPoint(int x_,int y_){
		cout<<"plotInt:"<<x_<<','<<y_<<endl;
	plotPoint((x_-250)/250.0,-(y_-250)/250.0);
}

void plotDoublePoint(double x_,double y_){
	plotPoint((x_-250)/250.0,-(y_-250)/250.0);
}

void DDALine(int x0,int y0,int x1,int y1){
	if(x0>y0){
		swap(x0,x1);
		swap(y0,y1);
	}
	float dx,dy,y,k,x=0;
	dx=x1-x0;dy=y1-y0;
	k=dy/dx,y=y0;
	// cout<<dy/dx<<endl;
	
	if(abs(dx)>abs(dy)) for(x=x0;x<=x1;x++){
			plotIntPoint(x,int(y+0.5));
			// cout<<x<<' '<<int(y+0.5)<<endl;
			y+=k;
		}
	else {
		if(y0>y1){
			swap(x0,x1);
			swap(y0,y1);
		}
		x=x0;
	for(y=y0;y<=y1;y++){
			// cout<<abs(dx)<<' '<<abs(dy)<<endl;
			// cout<<"k"<<endl;
			// cout<<y0<<' '<<y1<<endl;
				plotIntPoint(int(x+0.5),y);
				// cout<<x<<' '<<int(y+0.5)<<endl;
				x+=1/k;
				cout<<"x: "<<x<<"k: "<<k<<endl;
		}
	}
}

void Bresenham(const int x1, const int y1, const int x2, const int y2)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    int ux = ((dx > 0) << 1) - 1;
    int uy = ((dy > 0) << 1) - 1;
    int x = x1, y = y1, eps = 0; 
    dx = abs(dx);
    dy = abs(dy);
    if (dx > dy)
    {
        for (x = x1; x != x2; x += ux)
        {
            plotIntPoint(x,y);
            eps += dy;
            if ((2*eps) >= dx)
            {
                y += uy;
                eps -= dx;
            }
        }
    }
    else
    {
        for (y = y1; y != y2; y += uy)
        {
            plotIntPoint(x,y);
            eps += dx;
            if ((2*eps) >= dy)
            {
                x += ux;
                eps -= dy;
            }
        }
    }
}

float getd(int x_,int y_,int r_){
	return (x_+1)*(x_+1)+(y_-0.5)*(y_-0.5)-r_*r_;
}

void plotCirclePoint(int X,int Y,int P,int Q){
	//每一个都是1/8圆弧上的一点。
	//MidPointCircle负责画出1/8圆弧，然后操控此函数同时画8个此圆弧，构成完整的圆形。
	plotIntPoint(X + P, Y + Q);
	plotIntPoint(X - P, Y + Q);
	plotIntPoint(X + P, Y - Q);
	plotIntPoint(X - P, Y - Q);
	plotIntPoint(X + Q, Y + P);
	plotIntPoint(X - Q, Y + P);
	plotIntPoint(X + Q, Y - P);
	plotIntPoint(X - Q, Y - P);
}

 void MidPointCircle(int x1 , int y1,int x2,int y2)
{
	int r=sqrt((x2-x1)*(x2-x1)-(y2-y1)*(y2-y1));
	int x, y;
	double d;
	x = 0;
	y = r;
	d = 1.25 - r;
	plotCirclePoint(x1 , y1 , x , y);

	while(x < y)
	{
		if(d < 0)
			d = d + 2 * x + 3;
		else{
				d = d + 2 * ( x - y ) + 5;
				y--;
			}
		x++;
		plotCirclePoint(x1 , y1 , x , y);
	}
}

void myMouse(int button,int state,int x,int y)
{
	 if(state==GLUT_DOWN)
 		{
			++cntr;
			int r=100;
			if(cntr>=3)return;
			if(cntr==1){xx1=x;yy1=y;}
			if(cntr==2){
				xx2=x;yy2=y;
				DDALine(xx1,yy1,xx2,yy2);
				// Bresenham(xx1,yy1,xx2,yy2);
				// MidPointCircle(xx1,yy1,xx2,yy2);
			}
			// plotPoint((x-250)/250.0,-(y-250)/250.0);
			// cout<<(x-250)/250.0<<' '<<-(y-250)/250.0<<endl;
			cout<<x<<y<<endl;
 		}
 	else if(button==GLUT_RIGHT_BUTTON)
 	{
		cntr=0;
  		glClearColor(0.8,0.6,0.7,0.0);
  		glClear(GL_COLOR_BUFFER_BIT);
  		glFlush();
 	}
}

void d1(){
	//背景颜色，初始化窗口
  	glClearColor(0.8,0.6,0.7,0.0);
  	glClear(GL_COLOR_BUFFER_BIT);
  	glFlush();
}
 
int main(int argc,char **argv)
{
	glutInit(&argc,argv);
	glutInitWindowSize(500,500);
	glutCreateWindow("实验1_201900302022_姜九鸣");
	glutDisplayFunc(d1);
	glutMouseFunc(myMouse);
	glutMainLoop();
    return 0;
}