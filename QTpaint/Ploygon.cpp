#include"Polygon.h"
#include"QtDebug"

myPloygon::myPloygon()
{
    this->type_of_figure = polygon;
    this->rotate_angle = 0; //一开始的弧度为0
    this->is_rotating = false;
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
    for(int i = 0; i <= e; i++)
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
    pen.setCapStyle(Qt::RoundCap);
    painter->setPen(pen);//改变画笔的颜色
    for(int i = 0; i <this->point_of_move.size();i++)
    {
        painter->drawPoint(point_of_move[i]);
    }
    for(int i = 0; i <this->point_of_resize.size();i++)
    {
        pen.setColor(Qt::red);
        painter->setPen(pen);//改变画笔的颜色
        painter->drawPoint(point_of_resize[i]);
    }
    for(int i = 0; i <this->point_of_rotate.size();i++)
    {
        pen.setColor(Qt::green);
        painter->setPen(pen);//改变画笔的颜色
        painter->drawPoint(point_of_rotate[i]);
    }

    int style = static_cast<int>(Qt::DashDotLine);//设置QPainter的属性
    pen.setWidth(1);
    pen.setStyle((Qt::PenStyle)style);		//设置画笔的格式
    painter->setPen(pen);
    QPoint temp;
    for(int i = x_min; i < x_max; i+= 3)
    {
        temp.rx() = i;
        temp.ry() = y_min;
        painter->drawPoint(temp);
        temp.ry() = y_max;
        painter->drawPoint(temp);
    }
    for(int i = y_min; i < y_max; i+= 3)
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
    this->set_x_y_min_max();//设置最小值和最大值
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
        QPoint rotate_temp(center_temp.rx(),center_temp.ry() - (y_max-y_min)/8);    //旋转点就是center_temp 1/4y上的点
        this->change_point_by_angle(rotate_temp,sin(this->rotate_angle),cos(this->rotate_angle));
        this->point_of_rotate.push_back(rotate_temp);
    }
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

void myPloygon::change_point_by_angle(QPoint &t, double sina,double cosa)
{
    QPoint temp_center = this->point_center;
    QPoint t_end = t;
    t.rx() = (t_end.rx()-temp_center.rx() ) * cosa - (t_end.ry() -temp_center.ry()) *sina + temp_center.rx()+ 0.5;
    t.ry() =  (t_end.rx()-temp_center.rx() ) * sina + (t_end.ry() -temp_center.ry()) *cosa + temp_center.ry()+ 0.5;
}

bool myPloygon:: rotate_(QPainter *painter,QPoint end_pos)
{
    this->is_rotating = true; //是正在旋转
    //获取旋转角
    QPoint temp_center = this->point_center;
    QPoint temp_begin = this->point_of_rotate[0]; //开始旋转的点
    //根据三角型公式来求a的值
    double AB2 = get_2_distance(temp_center,temp_begin);
    double AC2 = get_2_distance(temp_center,end_pos);
    double BC2 = get_2_distance(temp_begin,end_pos); //
    double sina,cosa; //a的值为0到pi
    cosa = (AB2 + AC2 - BC2)/(2 * sqrt(AB2*AC2)); //得到cosa的值
    sina = sqrt(1 - cosa * cosa); //得到sina的值
    qDebug()<<sina<<" "<<cosa;
    //判断是否是正旋转还是负旋转
    if(temp_begin.rx() == temp_center.rx()) //可以知道两个点是一条直线上的，则我们可以知道斜率不存在
    {
        if(temp_begin.ry() > temp_center.ry()) //负旋转
        {
            if(end_pos.rx() < temp_begin.rx())
            {
                sina = -sina; //为负
            }
        }
        else
        {
            if(end_pos.rx() > temp_begin.rx())
            {
                sina = -sina; //为负
            }
        }
    }
    else
    {
        double k = double(temp_begin.ry()-temp_center.ry())/(temp_begin.rx()-temp_center.rx());//算出斜率
        if(end_pos.ry()- k * (end_pos.rx() - temp_center.rx()) - temp_center.ry() > 0) //在直线的下方
        {
            if(temp_begin.rx() < temp_center.rx())
            {
                sina = - sina;
            }
        }
        else if(end_pos.ry()- k * (end_pos.rx() - temp_center.rx()) + temp_center.ry()  > 0) //上方
        {
            if(temp_begin.rx() > temp_center.rx())
            {
                sina = - sina;
            }
        }
    }
    //根据旋转的弧度得出最后偏移的弧度，得出旋转后的旋转点
    double dealt_angle = acos(cosa); //求出cosa
    if(sina >0) //正向旋转
    {
        this->rotate_angle += dealt_angle;
        if(sin(rotate_angle) < 0) //大于180
        {
            this->rotate_angle = this->rotate_angle - 2 * M_PI;
        }
    }
    else //逆向旋转
    {
        this->rotate_angle -= dealt_angle;
        if(sin(rotate_angle) > 0) //负超过180
        {
            this->rotate_angle = this->rotate_angle + 2 * M_PI;
        }
    }
    //根据旋转角得出关键坐标
    this->change_point_by_angle(this->point_begin,sina,cosa);
    this->change_point_by_angle(this->point_end,sina,cosa);
    for(int i = 0; i < this->set_of_point.size();i++)
    {
        this->change_point_by_angle(this->set_of_point[i],sina,cosa);
    }
    this->change_point_by_angle(this->point_of_rotate[0],sina,cosa); //改变旋转点，但是中心点不改变
    this->draw_(painter,this->point_begin,this->point_end);
    is_rotating = false; //结束旋转
    return true;
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


