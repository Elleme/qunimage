#ifndef POLYGON_H
#define POLYGON_H
#include<QPainter>
#include"Figure.h"

class myPloygon : public Figure //从Figure进行继承
{
public:
    myPloygon();
    void show_edit_func(QPainter * painter);     //显示编辑点
    void draw_(QPainter * painter,QPoint begin,QPoint end); //每个图形进行绘制
    bool move_(QPainter *painter,int change_rx,int change_ry);
    bool rotate_(QPainter *painter,QPoint end_pos);
    bool resize_(QPainter *painter,QPoint end_pos,int num);
    void push_into_Ploygon(QPoint t);
    void set_x_y_min_max();
private:
    void draw_line(QPainter * painter,QPoint begin,QPoint end);
    int x_min;
    int y_min;
    int x_max;
    int y_max; //根据这四个点来确定一下外接矩形的大小
};


#endif // POLYGON_H
