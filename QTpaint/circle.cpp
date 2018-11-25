#include"circle.h"
#include<QDebug>
#include<cmath>

void draw_circle_Bresenham(QPainter * painter,QPoint &centre,QPoint &end) //进行圆的绘制
{
    int x_centre = centre.rx();
    int y_centre = centre.ry();

    int x_end = end.rx();
    int y_end = end.ry();

    double R =  sqrt((x_end-x_centre)*(x_end-x_centre) +(y_end-y_centre)*(y_end-y_centre));//计算两个点之间的距离
    int x,y,p;
    x=0;
    y=(int)R;
    p=(int)(3-2*R);
    for(;x<=y;x++)
    {
        //设置八个点
        QPoint temp1(int(x_centre+x),int(y_centre+y));//开始的点
        QPoint temp2(int(x_centre+y),int(y_centre+x));//第一象限对称点
        QPoint temp3(int(x_centre+x),int(y_centre-y));//第四象限
        QPoint temp4(int(x_centre-y),int(y_centre+x));//第四象限
        QPoint temp5(int(x_centre-x),int(y_centre+y));//第二象限
        QPoint temp6(int(x_centre+y),int(y_centre-x));
        QPoint temp7(int(x_centre-x),int(y_centre-y));
        QPoint temp8(int(x_centre-y),int(y_centre-x));
        //进行点的绘制
        painter->drawPoint(temp1);
        painter->drawPoint(temp2);
        painter->drawPoint(temp3);
        painter->drawPoint(temp4);
        painter->drawPoint(temp5);
        painter->drawPoint(temp6);
        painter->drawPoint(temp7);
        painter->drawPoint(temp8);

        if(p>=0)
        {
                p+=4*(x-y)+10;
                y--;
         }
         else
         {
                p+=4*x+6;
         }
     }
}
