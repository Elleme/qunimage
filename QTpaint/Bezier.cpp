#include "Bezier.h"
#include"QtDebug"
#include<qmath.h>
int factorial(int num) { //递归阶乘
    if (num <= 1) {
        return 1;
    } else {
        return num * factorial(num - 1);
    }
}

myBezier::myBezier()
{
    this->type_of_figure = Bezier; //为贝塞尔曲线
    this->rotate_angle = 0; //一开始的弧度为0
    this->is_rotating = false;
    this->BezierNodes.clear();
    this->t = 0;
    this->Bezier_finished = false;
}

void myBezier::draw_line(QPainter * painter,QPoint begin,QPoint end)//每个图形进行绘制
{
    //开始的坐标
    int x_begin = begin.rx();
    int y_begin = begin.ry();
    int x_end = end.rx();
    int y_end = end.ry();
    //DDA计算距离，判断直线的斜率
    double d_x = x_end - x_begin;
    double d_y = y_end - y_begin;
    //计算斜率
    double e = qAbs(d_x) > qAbs(d_y) ? qAbs(d_x) : qAbs(d_y); //判断划分的依据
    double de_x = d_x /e; //X增量
    double de_y = d_y /e; //Y增量
    double temp_x = x_begin;
    double temp_y = y_begin;
    for(int i = 0; i <= e;i++)
    {
        QPoint tempPoint((int)(temp_x + 0.5),(int)(temp_y)+0.5); //取最近的整数，而不是向下取整
        painter->drawPoint(tempPoint);
        temp_x += de_x;
        temp_y += de_y;
    }
}

void myBezier:: show_edit_func(QPainter * painter)     //显示编辑点
{
    if(this->set_of_point.size() < 2)
    {
        return;
    }
    qDebug()<<"show how to edit";
    QPen pen;
    pen.setWidth(10);					//设置画笔的线宽值
    pen.setColor(Qt::blue);
    painter->setPen(pen);//改变画笔的颜色
    for(int i = 0; i <this->point_of_move.size();i++)
    {
        painter->drawPoint(point_of_move[i]);
    }
    for(int i = 0; i <this->point_of_resize.size();i++)
    {
        painter->drawPoint(point_of_resize[i]);
    }
}

void myBezier:: draw_(QPainter * painter,QPoint begin,QPoint end)//每个图形进行绘制
{
        qDebug()<<"start drawing Bezier";
        assert(begin == this->set_of_point[0]);
        if(this->set_of_point.size() < 2)
        {
            return;
        }
        for(int i = 0; i < this->set_of_point.size() - 1; i++)
        {
            draw_line(painter,this->set_of_point[i],this->set_of_point[i+1]); //两两之间画直线
        }
        point_of_resize.clear(); //实现编辑大小
        this->point_of_resize = this->set_of_point; //可编辑的点就是这个点
        if(is_rotating == false) //如果旋转则不需要改变
        {
            point_of_move.clear(); //平移点
            point_of_rotate.clear(); //旋转
            QPoint center_temp((this->x_max + this->x_min)/2 + 0.5,(this->y_max + this->y_min)/2 + 0.5);
            this->point_center = center_temp; //确定中心点
            this->point_of_move.push_back(center_temp); //可移动的点
            //重点，设置旋转点，根据角度来进行计算
        }
       if(this->Bezier_finished == true) //可以画曲线
       {
           this->BezierNodes.clear(); //清空
           this->t = 0;
           QVector<QPoint> ordNode = this->set_of_point; //为原始点集
           qDebug()<<"!!!!!!!!!!!!!";
           this->drawBezier(painter,ordNode); //绘制曲线
       }
}

void myBezier::drawBezier(QPainter * painter,QVector<QPoint> nodes)
{
    for(t = 0; t < 1 ;t = t + 0.01){
        drawNodes(painter,nodes,t);
    }
     QVector<QPoint> nextNodes = nodes;
     this->drawNodes(painter,nodes,t); //进行绘制
}

void myBezier::drawNodes(QPainter * painter,QVector<QPoint> nodes,double st)
{
    if(nodes.size() == 0) return; //结束
    QVector<QPoint>  _nodes;
    for(int i = 0; i < nodes.length();i++)
        _nodes.push_back(nodes[i]);

    QVector<QPoint>  next_nodes;
    next_nodes.clear(); //清空
    for(int i = 0; i < _nodes.length() ; i++)
    {
        if(_nodes.length() == 1)
        {
            this->BezierNodes.push_back(_nodes[i]);
            if(this->BezierNodes.length() > 1)
            {
                for(int j = 0; j < this->BezierNodes.length() ; j++)
                {
                    if (j >= 1)
                    {
                        this->draw_line(painter,this->BezierNodes[j - 1],this->BezierNodes[j]);
                    }
                }
            }
        }
    }
    if(_nodes.length())
    {
        for(int i = 0; i < _nodes.length() - 1; i++) {
            next_nodes.push_back(bezier_next_Point(_nodes[i],_nodes[i+1], st));
        }
        drawNodes(painter,next_nodes,st);
    }
}

QPoint myBezier:: bezier_next_Point(QPoint a,QPoint b,double st)
{
    double x = 0;
    double y = 0;
    for(int i = 0; i < 2 ; i++)
    {
        if(i == 0) {
            x += a.rx() * pow(( 1 - st ), 1) * pow(st, 0) ;
            y += a.ry() * pow(( 1 - st ), 1) * pow(st, 0);
        }
        else {
            x += factorial(1) / factorial(i) / factorial(1 - i) * b.rx() * pow(( 1 - st ), 0) * pow(st, 1) ;
            y += factorial(1) / factorial(i) / factorial(1 - i) * b.ry() * pow(( 1 - st ), 0) *pow(st ,1) ;
        }
    }
    QPoint temp;
    temp.setX(x);
    temp.setY(y);
    return temp;
}

bool myBezier:: move_(QPainter *painter,int change_rx,int change_ry)
{

}

void myBezier::change_point_by_angle(QPoint &t, double sina,double cosa)
{

}

bool myBezier:: rotate_(QPainter *painter,QPoint end_pos)
{

}

bool myBezier:: resize_(QPainter *painter,QPoint end_pos,int num)
{

}

void myBezier:: set_x_y_min_max()
{

}

