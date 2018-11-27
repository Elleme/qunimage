#ifndef FIGURE_H
#define FIGURE_H
#include<QVector> //实现点的存储
#include<QPainter>

enum type_draw{
    none,thepen,line,circle,ellipse //一共四种图形
};

class Figure
{
public:
    Figure();
    void set_begin_point(QPoint begin); //设置开始点坐标
    void set_end_point(QPoint end); //设置结束点的坐标
    int is_to_move(QPoint temp);   //移动
    int is_to_rotate(QPoint temp); //旋转
    int is_to_resize(QPoint temp); //重新编辑大小
    QPoint get_start_pos();
    QPoint get_end_pos();
    //虚拟函数，展示继承类相应的属性
    virtual void show_edit_func(QPainter * painter) = 0;     //显示编辑点
    virtual void draw_(QPainter * painter,QPoint centre,QPoint end) = 0; //每个图形进行绘制
    virtual bool move_(QPainter *painter,int change_rx,int change_ry) = 0;
    virtual bool rotate_(QPainter *painter,QPoint end_pos) = 0; //对于图形进行旋转，参数为结束点
    virtual bool resize_(QPainter *painter,QPoint end_pos,int num) = 0; //对于图形进行重新编辑，根据不同参数点执行不同的操作
protected:
    QPoint point_begin; //开始点
    QPoint point_end; //结束点
    QPoint point_center; //中心点
    QPoint *point_fixed; //平移的时候确定的不改变的点
    QVector<QPoint> point_of_move; //平移点
    QVector<QPoint> point_of_rotate; //旋转
    QVector<QPoint> point_of_resize; //实现编辑大小
    type_draw type_of_figure;
    int num_of_resizing; //编辑功能，保证不改变点
};

#endif // FIGURE_H
