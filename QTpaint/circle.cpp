#include"Circle.h"
#include<QDebug>
#include<cmath>

myCircle::myCircle()
{
    this->R  = -1;
}

void myCircle::show_edit_func(QPainter * painter)    //显示编辑点
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

void myCircle::draw_(QPainter * painter,QPoint centre,QPoint end) //每个图形进行绘制
{
    int x_centre = centre.rx();
    int y_centre = centre.ry();
    int x_end = end.rx();
    int y_end = end.ry();
    R =  sqrt((x_end-x_centre)*(x_end-x_centre) +(y_end-y_centre)*(y_end-y_centre));//计算两个点之间的距离
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
    //画完之后，开始进行特使编辑点的存放
    point_of_move.clear(); //平移点
    point_of_rotate.clear(); //旋转
    point_of_resize.clear(); //实现编辑大小
    //移动点move
    this->point_of_move.push_back(this->point_begin); //设置圆心为中点
    //编辑点resize，为四个点，以半径可以切线
    QPoint resize_0(x_centre - R + 0.5,y_centre -R + 0.5);
    QPoint resize_1(x_centre - R + 0.5,y_centre + R + 0.5);
    QPoint resize_2(x_centre + R + 0.5,y_centre - R + 0.5);
    QPoint resize_3(x_centre + R + 0.5,y_centre + R + 0.5);
    this->point_of_resize.push_back(resize_0);
    this->point_of_resize.push_back(resize_1);
    this->point_of_resize.push_back(resize_2);
    this->point_of_resize.push_back(resize_3);
    //旋转，暂时不需要显示
    this->point_of_rotate.clear(); //为空
}

bool myCircle:: move_(QPainter *painter,int change_rx,int change_ry)
{
    this->point_begin.rx() += change_rx;
    this->point_begin.ry() += change_ry;
    this->point_end.rx() += change_rx;
    this->point_end.ry() += change_ry;
    this->draw_(painter,this->point_begin,this->point_end);
    return true;
}

bool myCircle::rotate_(QPainter *painter,QPoint end_pos)
{
    //还是不变的
    return true;
}

bool myCircle::resize_(QPainter *painter,QPoint end_pos,int num)
{
    //获取点
    int t = num;
    int nxt = -1;
    assert(t >= 0); //t一定是大于0的
    //根据t的选取来确定编辑的点
    if(t == 0)
    {
        nxt = 3;
    }
    else if(t == 1)
    {
        nxt = 2;
    }
    else if(t == 2)
    {
        nxt = 1;
    }
    else if(t == 3)
    {
        nxt = 0;
    }
    //确定不动的点
    this->num_of_resizing =nxt; //无法移动
    QPoint temp_fixed = this->point_of_resize[nxt];
    qDebug()<<temp_fixed.rx()<<" "<<temp_fixed.ry();
    //确定新的中心和半径
    QPoint temp_center;
    temp_center.rx()=( temp_fixed.rx() + end_pos.rx())/ 2 + 0.5;
    temp_center.ry()=( temp_fixed.ry() + end_pos.ry())/ 2 + 0.5;
    this->point_begin = temp_center;
    this->R = double(abs(temp_center.rx()- temp_fixed.rx()));
    QPoint end(temp_center.rx(), temp_center.ry() - R);
    this->point_end = end;
    this->draw_(painter,this->point_begin,this->point_end);//画画
    return true;
}
