#include"thepen.h"
#include<QDebug>

void draw_pen(QPainter *painter,QPoint end_Pos)
{
    painter->drawPoint(end_Pos);
}
