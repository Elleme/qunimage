#include "mywidget.h"
#include<QDebug>
#include"thepen.h"
#include"Line.h"
#include"Circle.h"
#include"Ellipse.h"
#include <QCursor>

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
      is_editing = false; //全部为false
      is_moving = -1;       //标志位设置成-1表示无操作
      is_rotating = -1;
      is_resizing = -1;
}

void myWidget::set_type_to_pen()//设置为笔
{
    draw_or_not = true;
    type_of_draw = thepen;
}

void myWidget::set_type_to_none()//设置为无图形
{
    draw_or_not = false;
    type_of_draw = none;
}

void myWidget::set_type_to_line()//设置为直线
{
     draw_or_not = true;
    type_of_draw = line;
}

void myWidget::set_type_to_circle()//设置为圆
{
     draw_or_not = true;
    type_of_draw = circle;
}

void myWidget::set_type_to_ellipse()//设置为椭圆
{
     draw_or_not = true;
    type_of_draw = ellipse;
}

bool  myWidget::set_new_figure(Figure *& temp)//构造新的图形
{
    switch (type_of_draw)
    {
            case none:
            {
                temp = nullptr;
                break;
            }
            case thepen:
            {
                temp = nullptr;
                 break;
            }
            case line:
            {
                temp = new myLine;
                break;
            }
            case circle:
            {
                temp = new myCircle;
                break;
            }
            case ellipse:
            {
                temp = new myEllipse;
                break;
            }
            default:
            {
                temp = nullptr;
                qDebug()<<"none"<<endl;
                break;
            }
    }
    if(temp == nullptr)
    {
        return false; //没有创建图形对象
    }
    else return true; //成功创建图形对象之一
}

void myWidget::mousePressEvent(QMouseEvent *e) //鼠标按压
{
    temp_draw_area = new QPixmap(801,541);
    temp_draw_area->fill(Qt::white);
    *temp_draw_area = *cur_draw_area;
    qDebug()<<"press"<<endl;
    if(e->button() & Qt::LeftButton) //点击鼠标的左键
    {
        qDebug()<<"start"<<start_Pos.rx()<<" "<<start_Pos.ry()<<endl;
        if(type_of_draw == none) //什么也不画
        {
            qDebug()<<"donothing";
        }
        else if(type_of_draw == thepen) //进行画笔类的操作
        {
            start_Pos = e->pos(); //记录下开始的点的位置
            end_Pos = e->pos();
            qDebug()<<"begin using pen";
            is_drawing = true;
        }
        else if(is_drawing == false && is_editing == false) //进行绘图
        {
            start_Pos = e->pos(); //记录下开始的点的位置
            qDebug()<<"begin drawing";
            //开始创建一个新的Figure对象进行操作
            bool succ =  this->set_new_figure(cur_figure);
            if(succ == false && (type_of_draw != none && type_of_draw != thepen))
            {
                qDebug()<<"error when creating Figure";
                assert(0); //中断
            }
            is_drawing = true; //开始绘图
            cur_figure->set_begin_point(start_Pos); //设置起始位置
        }
        else if(is_drawing == false && is_editing == true) //编辑模式
        {
            start_Pos = e->pos(); //记录下开始的点的位置
            end_Pos = e->pos();
            qDebug()<<"begin editing";
            //根据鼠标位置来进行判断
            is_moving = cur_figure->is_to_move(start_Pos); //判断是哪种模式
            is_resizing = cur_figure->is_to_resize(start_Pos);
            is_rotating = cur_figure->is_to_rotate(start_Pos);
            //判断结束,鼠标移动根据这些moving来进行迁徙
            qDebug()<<is_moving<<" "<<is_rotating<<" "<<is_resizing;
            if(is_moving == -1 && is_rotating == -1 && is_resizing == -1) //这三个功能都不使用，则进行保存功能
            {
                my_paint(cur_draw_area);
                delete temp_draw_area;
                if(type_of_draw != none)
                {
                    QPixmap *temp = getPixCopy();
                    draw_area.push_back(temp);//保存下来
                }
                delete cur_figure; //删除,改写
                is_editing = false; //退出编辑模式
                qDebug()<<"finish drawing";
            }
        }
        else if(is_drawing == true && is_editing == false)
        {
            qDebug()<<"error when click";
            assert(0);
        }
    }
}

void myWidget::mouseMoveEvent(QMouseEvent *e)  //鼠标移动
{
    e->accept();
    if(e->buttons() & Qt::LeftButton)
    {//-----鼠标为左键且正在移动
         QPoint *temp_pen_e = new QPoint;
         *temp_pen_e = start_Pos;
         QPoint *temp_pen_b = new QPoint;
         *temp_pen_b = end_Pos;
         end_Pos = e->pos(); //更新
         if(type_of_draw == none) //
         {
             qDebug()<<"do nothing"; //什么也不做
         }
         else if(type_of_draw == thepen) //进行画笔操作
         {
             qDebug()<<"use pen";
             type_of_draw = line;
             start_Pos = *temp_pen_e;
             my_paint(cur_draw_area);

             type_of_draw = thepen;
             start_Pos = *temp_pen_b;
             delete temp_pen_b;
             delete temp_pen_e;
         }
         else if(type_of_draw == mulity) //多边形的画图
         {

         }
         else //一次成型的图形
         {
            *temp_draw_area = *cur_draw_area;
            //对于不是thepen的图形来说，移动鼠标要分成三个部分来进行操作
            if(is_drawing == true && is_editing == false) //进入画图模式
            {
                //进行画图操作
                cur_figure->set_end_point(end_Pos); //设置当前位置的结尾
                my_paint(temp_draw_area);
            }
            else if(is_drawing == false && is_editing == true) //进入编辑模式
            {

                if(is_moving != -1) //显示移动功能
                {
                    //todo:实现图片的平行移动
                    QPainter *painter = new QPainter;		//新建一个QPainter对象
                    painter->begin(temp_draw_area);			//在当前PIXMAP进行绘制
                    painter->setPen(pen);					//将QPen对象应用到绘制对象中
                    cur_figure->move_(painter,(end_Pos.rx()-temp_pen_b->rx()),(end_Pos.ry()-temp_pen_b->ry()));
                    painter->end();
                    update();
                }
                else if(is_rotating != -1) //使用旋转功能
                {
                    QPainter *painter = new QPainter;		//新建一个QPainter对象
                    painter->begin(temp_draw_area);			//在当前PIXMAP进行绘制
                    painter->setPen(pen);					//将QPen对象应用到绘制对象中
                    cur_figure->rotate_(painter,end_Pos);
                    painter->end();
                    update();
                }
                else if(is_resizing != -1) //使用重新编辑的功能
                {
                    QPainter *painter = new QPainter;		//新建一个QPainter对象
                    painter->begin(temp_draw_area);			//在当前PIXMAP进行绘制
                    painter->setPen(pen);					//将QPen对象应用到绘制对象中
                    cur_figure->resize_(painter,end_Pos,is_resizing);
                    painter->end();
                    update();
                }
                else
                {
                    qDebug()<<"why do nothing?"<<endl;
                    assert(0);
                }
            }
         }
    }

}

void myWidget::mouseReleaseEvent(QMouseEvent *e)  //鼠标释放
{
    //鼠标释放
    end_Pos = e->pos(); //记录下结束的位置
    //qDebug()<<"end"<<end_Pos.rx()<<" "<<end_Pos.ry()<<endl;
    if(type_of_draw == none)
    {
       qDebug()<<"do nothing after release";
    }
    else if(type_of_draw == thepen)
    {
        qDebug()<<"finish using pen";
        is_drawing = false;
        is_editing = false;
        cur_figure = nullptr;
    }
    else if(is_drawing == true) //绘画结束
    {
        cur_figure->set_end_point(end_Pos);
        qDebug()<<"begin to edit";
        is_drawing = false;
        is_editing = true; //进入编辑模式
        QPainter *painter = new QPainter;		//新建一个QPainter对象
        painter->begin(temp_draw_area);			//在当前PIXMAP进行绘制
        painter->setPen(pen);        			//将QPen对象应用到绘制对象中
        cur_figure->show_edit_func(painter); //利用这个函数来进行对与编辑窗口的展示
        update();
    }
    else if(is_editing == true) //编辑结束，仍然是编辑模式
    {
        qDebug()<<"end of editing";
        QPainter *painter = new QPainter;		//新建一个QPainter对象
        painter->begin(temp_draw_area);			//在当前PIXMAP进行绘制
        painter->setPen(pen);        			//将QPen对象应用到绘制对象中
        cur_figure->show_edit_func(painter); //利用这个函数来进行对与编辑窗口的展示
        update();
    }
    else
    {
        qDebug()<<"do nothing";
    }
}

void myWidget::paintEvent(QPaintEvent *)  //画画
{
    QPainter painter(this);
    if((is_drawing == true  || is_editing == true)&& type_of_draw != thepen){
        painter.drawPixmap(QPoint(0,0),*temp_draw_area);//-------如果正在或正在编辑绘图，既鼠标点击或者正在移动，画在tempImage上
     }
    else
    {
        painter.drawPixmap(QPoint(0,0),*cur_draw_area);//-----------如果鼠标释放，将图保存在image上
    }
}

void myWidget::resizeEvent(QResizeEvent *)//
{

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
                qDebug()<<"draw point"<<endl;
                draw_pen(painter,end_Pos);
                break;
            }
            case line:
            {
                qDebug()<<"draw line"<<endl;
                this->cur_figure->draw_(painter,cur_figure->get_start_pos(),cur_figure->get_end_pos());
                break;
            }
            case circle:
            {
                qDebug()<<"draw circle"<<endl;
                this->cur_figure->draw_(painter,cur_figure->get_start_pos(),cur_figure->get_end_pos());
                break;
            }
            case ellipse:
            {
                qDebug()<<"drwa ellipse"<<endl;
                this->cur_figure->draw_(painter,cur_figure->get_start_pos(),cur_figure->get_end_pos());
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

void myWidget::set_pen_width(int i) //设置笔的宽度
{
         pen.setWidth(i);	//改变宽度
}

void myWidget::set_pen_color(QString i)//设置笔的颜色
{
      pen.setColor(i);
}

 void myWidget::save_pixmap()//保存图片到固定的路径，TOOO:设置到指定的路径
 {
     qDebug()<<"save";
     //QString file_path = "C:\\Users\\dell\\Desktop\\1.png";
     //如果没有写后缀就自动加上
     cur_draw_area->save("C:/Users/dell/Desktop/1.png","PNG");
 }
