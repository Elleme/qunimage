#include "ellipse.h"
#include<cmath>
#include<QDebug>

void draw_point_of_ellipse(QPainter * painter, int x_centre,int y_centre,int x,int y)
{
    QPoint temp1(x_centre+x,y_centre+y);
    QPoint temp2(x_centre+x,y_centre-y);
    QPoint temp3(x_centre-x,y_centre+y);
    QPoint temp4(x_centre-x,y_centre-y);
    //绘制这些点
    painter->drawPoint(temp1);
    painter->drawPoint(temp2);
    painter->drawPoint(temp3);
    painter->drawPoint(temp4);
}

void draw_ellipse_one(QPainter * painter,QPoint &centre,QPoint &end)
{
    //初始化坐标
    int x_centre = centre.rx();
    int y_centre = centre.ry();
    int x_end = end.rx();
    int y_end = end.ry();
    //椭圆是以centre为中心开始创建的
    int r_x = abs(x_end-x_centre);
    int r_y = abs(y_end-y_centre);
    //得到两个直径
    double sqx = r_x * r_x;
    double sqy = r_y * r_y;

    qDebug()<<"r_x"<<r_x<<endl;
    qDebug()<<"r_y"<<r_y<<endl;
    int x = 0;
    int y = r_y;
    double p_1 = (sqy - sqx * r_y + 0.25 * sqx) ;
    draw_point_of_ellipse(painter,x_centre,y_centre,x,y);
    //开始进行区域一的绘制
    while(sqy * x <= sqx * y)
    {
            //计算P_0
            if(p_1 < 0)
            {
                p_1 = p_1 + 2 * sqy * x + 3 * sqy;
            }
            else
            {
                p_1 = p_1 +  2 * sqy * x  - 2 * sqx * y + 3 * sqy+ 2 * sqx;
                y--;
            }
            //计算这些点
            x++;
            draw_point_of_ellipse(painter,x_centre,y_centre,x,y);
     }
     //进行区域二的绘制
     double p_2 = (sqy * (x + 0.5) * (x + 0.5) + sqx* (y - 1) * (y - 1) - sqx * sqy);
     while(y>0)
     {
         if(p_2 >= 0)
         {
              p_2 = p_2 - 2 * sqx * y + 3 * sqx;
         }
         else
         {
              p_2 = p_2 - 2 * sqx * y + 2 * sqy * x  +  3 * sqx + 2 * sqy;
              x++;
          }
          y--;
          draw_point_of_ellipse(painter,x_centre,y_centre,x,y);
      }
}



void draw_ellipse_two(QPainter * painter,QPoint &centre,QPoint &end)
{
    //初始化坐标
    int x_centre = centre.rx();
    int y_centre = centre.ry();
    int x_end = end.rx();
    int y_end = end.ry();
    //椭圆是以centre为中心开始创建的
    int r_x = abs(x_end-x_centre);
    int r_y = abs(y_end-y_centre);
    int x = 0;
    int y = r_y;
    int sqa = r_x * r_x;
    int sqb = r_y * r_y;

    qDebug()<<"r_x"<<r_x<<endl;
    qDebug()<<"r_y"<<r_y<<endl;

    int d = 2 * sqb - 2 * r_y * sqa + sqa;
    int P_x = int((double)sqa / sqrt((double)(sqa + sqb)));
    while (x <= P_x)
    {
        if (d < 0)
        {
            d += 2 * sqb * (2 * x + 3);
        }
        else
        {
            d += 2 * sqb * (2 * x + 3) - 4 * sqa * (y - 1);
            if(y>0) y--;
        }
        x++;
        //四个象限
         draw_point_of_ellipse(painter,x_centre,y_centre,x,y);
    }
    d = sqb * (x * x + x) + sqa * (y * y - y) - sqa * sqb;
    while (y >= 0)
    {
    //四个象限
    draw_point_of_ellipse(painter,x_centre,y_centre,x,y);
    y--;
    if (d < 0)
    {
        x++;
        d = d - 2 * sqa * y - sqa + 2 * sqb * x + 2 * sqb;
    }
    else
        d = d - 2 * sqa * y - sqa;
    }
}
