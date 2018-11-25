#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include<myopengl.h>
#include<QPainter>
#include<qpainter.h>
#include<QMouseEvent>
#include<QVector>
#include<QPen>
#include<QPixmap>
#include<QtDebug>

enum type_draw{
    none,thepen,line,circle,ellipse //一共四种图形
};

class myWidget : public QWidget
{
    Q_OBJECT
public:
    explicit myWidget(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE; //鼠标按压
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE; //鼠标移动
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE; //鼠标释放
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE; //画画
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE; //
    void set_type_to_none();
    void set_type_to_pen();
    void set_type_to_line();
    void set_type_to_rectangle();
    void set_type_to_circle();
    void set_type_to_ellipse();
    void my_paint (QPixmap *the_iamge); //画图
    void set_draw_area_back(); //撤销
    void set_draw_area_clear();//清屏
    void set_pen_width(int i);//设置笔长
    void set_pen_color(QString i);//设置笔的颜色
    void save_pixmap();
    QPixmap *getPixCopy();

private:
    myopenGL*s;
    QVector<QPixmap*> draw_area; //用于撤销和清空
    QPixmap *cur_draw_area; //用于显示当前的Pixmap
    QPixmap *temp_draw_area;//双缓冲绘图的
    type_draw type_of_draw; //种类
    bool is_drawing; //是否在画画,用来双缓冲
    bool draw_or_not;
    QPen pen; //画笔
    QPoint start_Pos; //开始点
    QPoint end_Pos; //结束点

signals:

public slots:

};

#endif // MYWIDGET_H
