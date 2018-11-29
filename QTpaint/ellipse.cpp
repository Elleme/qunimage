#include "Ellipse.h"
#include<cmath>
#include<QDebug>

myEllipse::myEllipse()
{
        this->type_of_figure = ellipse;
}

void myEllipse::show_edit_func(QPainter * painter)  //显示编辑点
{
    qDebug()<<"show how to edit";
    QPen pen;
    pen.setWidth(10);					//设置画笔的线宽值
    pen.setColor(Qt::blue);
    painter->setPen(pen);//改变画笔的颜色
    for(int i = 0; i <this->point_of_move.size();i++)
    {
        qDebug()<<"show how to edit1";
        painter->drawPoint(point_of_move[i]);
    }
    for(int i = 0; i <this->point_of_resize.size();i++)
    {
        qDebug()<<"show how to edit2";
        painter->drawPoint(point_of_resize[i]);
    }
    for(int i = 0; i <this->point_of_rotate.size();i++)
    {
        qDebug()<<"show how to edit3";
        painter->drawPoint(point_of_rotate[i]);
    }
}

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


void myEllipse::draw_(QPainter * painter,QPoint centre,QPoint end)//每个图形进行绘制
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
     //画完之后，开始进行特使编辑点的存放

     point_of_move.clear(); //平移点
     point_of_rotate.clear(); //旋转
     point_of_resize.clear(); //实现编辑大小
     //移动点move
     this->point_of_move.push_back(this->point_begin); //设置圆心为中点
     //编辑点resize，为四个点，以半径可以切线
     QPoint resize_0(x_centre - r_x ,y_centre - r_y);
     this->point_of_resize.push_back(resize_0);
     //旋转，暂时不需要显示
     this->point_of_rotate.clear(); //为空
}

bool myEllipse:: move_(QPainter *painter,int change_rx,int change_ry)
{
    this->point_begin.rx() += change_rx;
    this->point_begin.ry() += change_ry;
    this->point_end.rx() += change_rx;
    this->point_end.ry() += change_ry;
    this->draw_(painter,this->point_begin,this->point_end);
    return true;
}

bool myEllipse::rotate_(QPainter *painter,QPoint end_pos)
{
    return true;
}

bool myEllipse::resize_(QPainter *painter,QPoint end_pos,int num)
{
    int t = num;
    assert(t >= 0); //t一定是大于0的
    this->point_end = end_pos;
    this->draw_(painter,this->point_begin,this->point_end);//画画
    return true;
}
