#include "Figure.h"

Figure::Figure()
{
    point_of_move.clear(); //平移点
    point_of_rotate.clear(); //旋转
    point_of_resize.clear(); //实现编辑大小
    type_of_figure = none; //设置成无图形属性
    num_of_resizing = -1;
}

void Figure::set_begin_point(QPoint begin) //设置开始点坐标
{
    this->point_begin = begin;
}

void Figure::set_end_point(QPoint end)//设置结束点的坐标
{
    this->point_end = end;
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
