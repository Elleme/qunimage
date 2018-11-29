#include"Polygon.h"
#include"QtDebug"

myPloygon::myPloygon()
{
    this->type_of_figure = polygon;
}

void myPloygon::draw_line(QPainter * painter,QPoint begin,QPoint end)//每个图形进行绘制
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

void myPloygon:: show_edit_func(QPainter * painter)     //显示编辑点
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

void myPloygon:: draw_(QPainter * painter,QPoint begin,QPoint end)//每个图形进行绘制
{
    //绘画需要重新构建一下
    qDebug()<<"size:"<<this->set_of_point.size();
    assert(this->set_of_point.size() >= 2); //一定有两个以上的顶点
    assert(begin == this->set_of_point[0]);
    qDebug()<<"???";
    for(int i = 0; i < this->set_of_point.size() - 1; i++)
    {
        draw_line(painter,this->set_of_point[i],this->set_of_point[i+1]); //两两之间画直线
    }
    point_of_move.clear(); //平移点
    point_of_rotate.clear(); //旋转
    point_of_resize.clear(); //实现编辑大小
    this->point_of_resize = this->set_of_point; //点
}

bool myPloygon:: move_(QPainter *painter,int change_rx,int change_ry)
{

}

bool myPloygon:: rotate_(QPainter *painter,QPoint end_pos)
{

}

bool myPloygon:: resize_(QPainter *painter,QPoint end_pos,int num)
{

}

void myPloygon:: push_into_Ploygon(QPoint t)
{

}
