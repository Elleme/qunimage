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

    int style = static_cast<int>(Qt::DashDotLine);//设置QPainter的属性
    pen.setWidth(1);
    pen.setStyle((Qt::PenStyle)style);		//设置画笔的格式
    painter->setPen(pen);
    QPoint temp;
    for(int i = x_min; i < x_max; i++)
    {
        temp.rx() = i;
        temp.ry() = y_min;
        painter->drawPoint(temp);
        temp.ry() = y_max;
        painter->drawPoint(temp);
    }
    for(int i = y_min; i < y_max; i++)
    {
        temp.ry() = i;
        temp.rx() = x_min;
        painter->drawPoint(temp);
        temp.rx() = x_max;
        painter->drawPoint(temp);
    }
}

void myPloygon:: draw_(QPainter * painter,QPoint begin,QPoint end)//每个图形进行绘制
{
    //绘画需要重新构建一下
    qDebug()<<"size:"<<this->set_of_point.size();
    assert(begin == this->set_of_point[0]);
    if(this->set_of_point.size() < 2)
    {
        return;
    }
    for(int i = 0; i < this->set_of_point.size() - 1; i++)
    {
        draw_line(painter,this->set_of_point[i],this->set_of_point[i+1]); //两两之间画直线
    }
    //确定外接矩形
    this->set_x_y_min_max(); //设置最小值和最大值

    point_of_move.clear(); //平移点
    point_of_rotate.clear(); //旋转
    point_of_resize.clear(); //实现编辑大小

    this->point_of_resize = this->set_of_point; //可编辑的点就是这个点
    QPoint center_temp((this->x_max + this->x_min)/2 + 0.5,(this->y_max + this->y_min)/2 + 0.5);
    this->point_of_move.push_back(center_temp); //可移动的点
}

bool myPloygon:: move_(QPainter *painter,int change_rx,int change_ry)
{
    if(this->set_of_point.size() < 2)
    {
        return false;
    }
    this->point_begin.rx() += change_rx;
    this->point_end.rx() += change_rx;
    this->point_begin.ry() += change_ry;
    this->point_end.ry() += change_ry;
    for(int i = 0; i < this->set_of_point.size();i++)
    {
        set_of_point[i].rx() += change_rx;
        set_of_point[i].ry() += change_ry;
    }
    this->draw_(painter,this->point_begin,this->point_end);
    return true;
}

bool myPloygon:: rotate_(QPainter *painter,QPoint end_pos)
{

}

bool myPloygon:: resize_(QPainter *painter,QPoint end_pos,int num)
{
    if(this->set_of_point.size() < 2)
    {
        return false;
    }
    if(num == 0)
    {
        this->point_begin = end_pos;
        this->point_end = end_pos;
        this->set_of_point[num] = end_pos;
        this->set_of_point[set_of_point.size()-1] = end_pos;
    }
    else this->set_of_point[num] = end_pos;
    this->draw_(painter,this->point_begin,this->point_end);//画画
    return true;
}

void myPloygon:: set_x_y_min_max()
{
    if(this->set_of_point.size() < 2)
    {
        return;
    }
    qDebug()<<"???";
    assert(this->set_of_point.size() >= 2); //点集一定是大于二的
    this->x_min = set_of_point[0].rx();
    this->x_max = set_of_point[0].rx();
    this->y_min = set_of_point[0].ry();
    this->y_max = set_of_point[0].ry();
    for(int i = 0; i < set_of_point.size(); i++)
   {
       int tempx = set_of_point[i].rx();
       int tempy = set_of_point[i].ry();
       if(tempx < x_min) x_min = tempx;
       if(tempx > x_max) x_max = tempx;
       if(tempy < y_min) y_min = tempy;
       if(tempy > y_max) y_max = tempy;
   }
}


