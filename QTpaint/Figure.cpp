#include "Figure.h"
#include"QDebug.h"
Figure::Figure()
{
    point_of_move.clear(); //平移点
    point_of_rotate.clear(); //旋转
    point_of_resize.clear(); //实现编辑大小
    this->set_of_point.clear(); //多边形进行清空
    type_of_figure = none; //设置成无图形属性
    num_of_resizing = -1;
    this->finished = false; //为空
}

bool is_near(QPoint t,QPoint dest)
{
    double dx =  t.rx() - dest.rx() ;
    double dy =  t.ry() - dest.ry() ;
    double d = dx*dx + dy * dy;
    if(d <= 25)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Figure::set_begin_point(QPoint begin) //设置开始点坐标
{
    this->point_begin = begin;
}

void Figure::set_end_point(QPoint end)//设置结束点的坐标
{
    this->point_end = end;
    if(this->type_of_figure == polygon) //多边形
    {
        int t = (int)this->set_of_point.size() - 1;
        if(t > 0)
        {
            this->set_of_point[t] = end; //改变最后一个点
            qDebug()<<"change last point";
        }
    }
}

bool catch_the_point(QPoint &a,QPoint &b)
{
    int dis = (a.rx()-b.rx())*(a.rx()-b.rx()) + (a.ry()-b.ry())*(a.ry()-b.ry());
    if(dis <= 5)
    {
        return true;
    }
    else return false;
}


int Figure::is_to_move(QPoint temp)  //移动
{
    for(int i = 0; i < this->point_of_move.size();i++)
    {
        if(catch_the_point(point_of_move[i],temp) == true)
        {
            return i;
        }
    }
    return -1;
}

int Figure::is_to_rotate(QPoint temp) //旋转
{
    for(int i = 0; i < this->point_of_rotate.size();i++)
    {
        if(catch_the_point(point_of_rotate[i],temp) == true)
        {
            return i;
        }
    }
    return -1;
}

int Figure::is_to_resize(QPoint temp) //重新编辑大小
{
    for(int i = 0; i < this->point_of_resize.size();i++)
    {
        if(catch_the_point(point_of_resize[i],temp) == true)
        {
            return i;
        }
    }
    return -1;
}


QPoint  Figure:: get_start_pos()
{
    return this->point_begin;
}

QPoint  Figure::get_end_pos()
{
    return  this->point_end;
}


bool Figure::is_polyon_to(QPoint &t) //判断多边形是否为空
{
    if(this->set_of_point.empty() == true) //没有点
    {
        return true;
    }
    else
    {
         if(is_near(t,set_of_point[0]) == true)
         {
             t = set_of_point[0]; //改变最后一个定点
             return false;
         }
         else
             return  true;
    }
}

void Figure::add_into_set(QPoint t)
{
    qDebug()<<"add into set"<<this->set_of_point.size();
    this->set_of_point.push_back(t);
    this->point_of_resize.push_back(t);
    qDebug()<<"after add into set"<<this->set_of_point.size();
}

int Figure::num_of_set()
{
    return this->set_of_point.size(); //返回为真
}

bool  Figure:: is_polyon_finished()
{
    if(this->set_of_point.size()<2)
    {
        return false;
    }
    QPoint t = this->set_of_point.back(); //最后一个元素
    if(is_near(t,this->set_of_point[0]) == true)        //如果相同
    {
        t  = this->set_of_point[0];
        this->set_of_point.pop_back();
        this->set_of_point.push_back(t);
        return true;
    }
    return false;
}

double  Figure::get_2_distance(QPoint a,QPoint b) //获取两个点的坐标的距离平方
{
    int dx = a.rx()-b.rx();
    int dy = a.ry()- b.ry();
    double dis2 =  dx * dx + dy * dy;
    return dis2;
}
