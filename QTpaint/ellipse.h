#ifndef ELLIPSE_H
#define ELLIPSE_H
#include<QPainter>
#include"Figure.h"

class myEllipse : public Figure //从Figure进行继承
{
public:
    myEllipse();
    void show_edit_func(QPainter * painter);     //显示编辑点
    void draw_(QPainter * painter,QPoint centre,QPoint end); //每个图形进行绘制
    bool move_(QPainter *painter,int change_rx,int change_ry);
    bool rotate_(QPainter *painter,QPoint end_pos);
    bool resize_(QPainter *painter,QPoint end_pos,int num);
};



void draw_ellipse_one(QPainter * painter,QPoint &centre,QPoint &end);
void draw_ellipse_two(QPainter * painter,QPoint &centre,QPoint &end);

#endif // ELLIPSE_H
