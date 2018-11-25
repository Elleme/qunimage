#include "mywidget.h"
#include<QDebug>
#include"thepen.h"
#include"line.h"
#include"circle.h"
#include"ellipse.h"

myWidget::myWidget(QWidget *parent) : QWidget(parent)
{
     s =  new myopenGL;
     setAutoFillBackground(true);    //自动设定背景颜色
     setPalette(QPalette(Qt::gray));      //设置调色板的颜色为白色

     cur_draw_area = new QPixmap(801,541);
     cur_draw_area->fill(Qt::white);

     QPixmap *temp = new QPixmap;
     *temp = *cur_draw_area;
     draw_area.push_back(temp);

     type_of_draw = none;       //不在画图形
     is_drawing = false;        //不能画，所以不能够行动

      int style = static_cast<int>(Qt::SolidLine);//设置QPainter的属性
      int weight = 3;
      pen.setStyle((Qt::PenStyle)style);		//设置画笔的格式
      pen.setWidth(weight);					//设置画笔的线宽值
      pen.setColor(Qt::black);

      is_drawing = false;
      draw_or_not = false;
}

void myWidget::set_type_to_pen()
{
     draw_or_not = true;
    type_of_draw = thepen;
}

void myWidget::set_type_to_none()
{
    draw_or_not = false;
    type_of_draw = none;
}

void myWidget::set_type_to_line()
{
     draw_or_not = true;
    type_of_draw = line;
}

void myWidget::set_type_to_circle()
{
     draw_or_not = true;
    type_of_draw = circle;
}

void myWidget::set_type_to_ellipse()
{
     draw_or_not = true;
    type_of_draw = ellipse;
}

void myWidget:: my_paint (QPixmap *the_iamge){
    QPainter *painter = new QPainter;		//新建一个QPainter对象
    painter->begin(the_iamge);			//在当前PIXMAP进行绘制
    painter->setPen(pen);					//将QPen对象应用到绘制对象中
    switch (type_of_draw)
    {
            case none:
            {
                qDebug()<<"pick"<<endl;
                break;
            }
            case thepen:
            {
                qDebug()<<"drwa point"<<endl;
                draw_pen(painter,end_Pos);
                break;
            }
            case line:
            {
                qDebug()<<"draw line"<<endl;
                draw_line_DDA(painter,start_Pos,end_Pos);
                break;
            }
            case circle:
            {
                qDebug()<<"draw circle"<<endl;
                draw_circle_Bresenham(painter,start_Pos,end_Pos);
                break;
            }
            case ellipse:
            {
                qDebug()<<"drwa ellipse"<<endl;
                draw_ellipse_one(painter,start_Pos,end_Pos);
                break;
            }
            default:
            {
                qDebug()<<"none"<<endl;
                break;
            }
    }
    painter->end();
    update();						//重绘绘制区窗体
}

void myWidget::mousePressEvent(QMouseEvent *e) //鼠标按压
{
    temp_draw_area = new QPixmap(801,541);
    temp_draw_area->fill(Qt::white);
    *temp_draw_area = *cur_draw_area;
    qDebug()<<"press"<<endl;
    if(e->button() & Qt::LeftButton) //点击鼠标的左键
    {
        start_Pos = e->pos(); //记录下开始的点的位置
        end_Pos = e->pos();
        qDebug()<<"start"<<start_Pos.rx()<<" "<<start_Pos.ry()<<endl;
        is_drawing = true;
    }
}

void myWidget::mouseMoveEvent(QMouseEvent *e)  //鼠标移动
{
    //目的使用双缓冲进行绘制图像，暂不实现
    if(e->buttons() & Qt::LeftButton){//-----鼠标为左键且正在移动
         qDebug()<<"move"<<endl;
         //实现
         //画笔需要记录
         QPoint *temp_pen_e = new QPoint;
         *temp_pen_e = start_Pos;
         QPoint *temp_pen_b = new QPoint;
         *temp_pen_b = end_Pos;

         end_Pos = e->pos(); //更新
         if(type_of_draw != thepen)
         {
            *temp_draw_area = *cur_draw_area;
            my_paint(temp_draw_area);
         }
         else
         {
             type_of_draw = line;
             start_Pos = *temp_pen_e;
             my_paint(cur_draw_area);

             type_of_draw = thepen;
             start_Pos = *temp_pen_b;
             delete temp_pen_b;
             delete temp_pen_e;
         }
     }
}

void myWidget::mouseReleaseEvent(QMouseEvent *e)  //鼠标释放
{
    is_drawing = false;
    //鼠标释放
    end_Pos = e->pos(); //记录下结束的位置
    qDebug()<<"end"<<end_Pos.rx()<<" "<<end_Pos.ry()<<endl;

    my_paint(cur_draw_area);
    delete temp_draw_area;
    if(type_of_draw != none)
    {
        QPixmap *temp = getPixCopy();
        draw_area.push_back(temp);//保存下来
    }
}

void myWidget::paintEvent(QPaintEvent *)  //画画
{
    //QPainter painter_t(this);
    //painter_t.drawPixmap(QPoint(0,0),*cur_draw_area);
    QPainter painter(this);
    if(is_drawing == true && type_of_draw != thepen){
        painter.drawPixmap(QPoint(0,0),*temp_draw_area);//-------如果正在绘图，既鼠标点击或者正在移动，画在tempImage上
     }
    else
    {
        painter.drawPixmap(QPoint(0,0),*cur_draw_area);//-----------如果鼠标释放，将图保存在image上
    }
}

void myWidget::resizeEvent(QResizeEvent *)//
{

}

QPixmap *myWidget::getPixCopy()
{
    QPixmap *newPix = new QPixmap(801,541);	//创建一个新的QPixmap对象
    newPix->fill(Qt::white);                //填充新QPixmap对象newPix的颜色为白色背景色
    QPainter p(newPix);
    p.drawPixmap(QPoint(0,0),*cur_draw_area);         //在newPix中绘制原pix中的内容
    qDebug() <<"Pix Copy at"<<&newPix<<endl;
    return newPix;
}

void myWidget::set_draw_area_back() //撤销
{
    qDebug() <<"return"<<draw_area.size();
    if((int)draw_area.size() >=2)
    {
       draw_area.pop_back();
    }
    if((int)draw_area.size() >= 1) //当前超过
    {
        QPixmap *temp = draw_area.back();
        *cur_draw_area = *temp;
        update();
    }
}

void myWidget::set_draw_area_clear()//清屏
{
    QPixmap *newPix = new QPixmap(801,541);	//创建一个新的QPixmap对象
    newPix->fill(Qt::white);                //填充新QPixmap对象newPix的颜色为白色背景色
    *cur_draw_area = *newPix;
    draw_area.push_back(newPix);
    update();
}

void myWidget::set_pen_width(int i)
{
         pen.setWidth(i);	//改变宽度
}

void myWidget::set_pen_color(QString i)
{
      pen.setColor(i);
}

 void myWidget::save_pixmap()
 {
     qDebug()<<"save";
     //QString file_path = "C:\\Users\\dell\\Desktop\\1.png";
     //如果没有写后缀就自动加上
     cur_draw_area->save("C:/Users/dell/Desktop/1.png","PNG");
 }
