#include<Line.h>
#include<math.h>
#include<QtDebug>


myLine::myLine()
{
    this->type_of_figure = line;
}

void myLine::show_edit_func(QPainter * painter)  //显示编辑点
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

void myLine::draw_(QPainter * painter,QPoint begin,QPoint end)//每个图形进行绘制
{
    //开始的坐标
    if(this->point_begin != begin || this->point_end != end)
    {

        qDebug()<<"not equal" <<end.rx()<<" "<<end.ry()<<" "<<begin.rx()<<" "<<begin.ry();
        qDebug()<<"n"<<this->point_begin.rx()<<" "<<this->point_end.rx();
        assert(0);
    }
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

    qDebug()<<x_begin<<" "<<x_end<<endl;
    qDebug()<<y_begin<<" "<<y_end<<endl;

    for(int i = 0; i <= e;i++)
    {
        QPoint tempPoint((int)(temp_x + 0.5),(int)(temp_y)+0.5); //取最近的整数，而不是向下取整
        painter->drawPoint(tempPoint);
        temp_x += de_x;
        temp_y += de_y;
    }
    //画图绘制完毕,开始push点进去
    point_of_move.clear(); //平移点
    point_of_rotate.clear(); //旋转
    point_of_resize.clear(); //实现编辑大小
    //放入平移点，为直线的中点
    QPoint moving((begin.rx() +end.rx()) /2,(begin.ry() +end.ry()) /2);
    this->point_of_move.push_back(moving); //放入
    //放入旋转点,先定为1/4点
    QPoint rotating(begin.rx()+(end.rx()-begin.rx()) /4,begin.ry()+(end.ry()-begin.ry()) /4);
    this->point_of_rotate.push_back(rotating);//放入
    //放入拖动点
    QPoint temp_resizing_1(begin.rx(),begin.ry());
    QPoint temp_resizing_2(end.rx(),end.ry());
    if(this->num_of_resizing == 0) // 以第一个点为移动的，则
    {
        this->point_of_resize.push_back(temp_resizing_2);
        this->point_of_resize.push_back(temp_resizing_1);
    }
    else
    {
        this->point_of_resize.push_back(temp_resizing_1);
        this->point_of_resize.push_back(temp_resizing_2);
    }
    //结束计算
    QPoint temp((this->point_begin.rx()+this->point_end.rx()+0.5) /2,(this->point_begin.ry()+this->point_end.ry()+0.5) /2 );
    this->point_center = temp; //更新中点
    qDebug()<<"center"<<this->point_center.rx()<<" "<<this->point_center.ry();
}

bool myLine::move_(QPainter *painter,int change_rx,int change_ry)
{
    this->point_begin.rx() += change_rx;
    this->point_begin.ry() += change_ry;
    this->point_end.rx() += change_rx;
    this->point_end.ry() += change_ry;
    this->draw_(painter,this->point_begin,this->point_end);
    return true;
}

double get_2_distance(QPoint a,QPoint b) //获取两个点的坐标的距离平方
{
    int dx = a.rx()-b.rx();
    int dy = a.ry()- b.ry();
    double dis2 =  dx * dx + dy * dy;
    return dis2;
}

bool myLine::rotate_(QPainter *painter,QPoint end_pos)
{

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
    //根据旋转角得出关键坐标
    QPoint t_start = this->point_begin;
    this->point_begin.rx() = (t_start.rx()-temp_center.rx() ) * cosa - (t_start.ry() -temp_center.ry()) * sina + temp_center.rx() + 0.5;
    this->point_begin.ry() = (t_start.rx()-temp_center.rx() ) * sina + (t_start.ry() -temp_center.ry()) * cosa + temp_center.ry() + 0.5;

    QPoint t_end = this->point_end;
    this->point_end.rx() = (t_end.rx()-temp_center.rx() ) * cosa - (t_end.ry() -temp_center.ry()) *sina + temp_center.rx()+ 0.5;
    this->point_end.ry() =  (t_end.rx()-temp_center.rx() ) * sina + (t_end.ry() -temp_center.ry()) *cosa + temp_center.ry()+ 0.5;

    this->draw_(painter,this->point_begin,this->point_end);
    return true;
}

bool myLine::resize_(QPainter *painter,QPoint end_pos,int num)
{
    this->num_of_resizing = num; //为编辑的编号
    QPoint change_start_pos;
    //因为是直线，所以只需要固定一个定点，已改定点为起始点，另一个点为终点，进行改变
    int t = num;
    assert(t >= 0);//必须找到
    if(t == 0)
    {
        change_start_pos = this->point_of_resize[1];
    }
    else
    {
        change_start_pos = this->point_of_resize[0];
    }
    this->point_begin = change_start_pos;
    this->point_end = end_pos; //改变起始坐标和终点坐标
    this->draw_(painter,this->point_begin,this->point_end);
     this->num_of_resizing =  -1;
    return true;
}


