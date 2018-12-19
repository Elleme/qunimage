#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    setWindowTitle(tr("QunPaint"));

    ui->mdiArea->setViewMode(QMdiArea::TabbedView);
    ui->mdiArea->setTabShape(QTabWidget::Triangular);   //设置窗口的形状
    ui->mdiArea->setTabPosition(QTabWidget::North); //设置窗口的位置
    ui->mdiArea->setTabsClosable(true);         //可关闭的
    ui->mdiArea->setTabsMovable(true);        //可移动的
    this->cur_widget = nullptr;
    //初始化样式
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_choose_clicked() //选择
{
    qDebug()<<"pick mouse"<<endl;
    cur_widget->set_type_to_none();
}

void MainWindow::on_pen_clicked()
{
    qDebug()<<"pick pen"<<endl;
    cur_widget->set_type_to_pen();
}

void MainWindow::on_line_clicked() //画直线
{
    qDebug()<<"pick line"<<endl;
    cur_widget->set_type_to_line();
}

void MainWindow::on_circle_clicked() //画圆
{
    qDebug()<<"pick circle"<<endl;
    cur_widget->set_type_to_circle();
}

void MainWindow::on_ellipse_clicked()//画椭圆
{
    qDebug()<<"pick ellipse"<<endl;
    cur_widget->set_type_to_ellipse();
}

void MainWindow::on_polygon_clicked() //多边形
{
    qDebug()<<"pick polygon"<<endl;
    cur_widget->set_type_to_polygon();
}


void MainWindow::on_Bezier_clicked()
{
    qDebug()<<"pick Bezier"<<endl;
    cur_widget->set_type_to_Bezier();
}

void MainWindow::on_actions_T_triggered()
{
    if(ui->mdiArea->activeSubWindow() == nullptr)
    {
        delete this->cur_widget;
        this->cur_widget = nullptr;
    }
    if(this->cur_widget == nullptr)
    {
        myWidget *draw_ = new myWidget;
        QMdiSubWindow * t = ui->mdiArea->addSubWindow(draw_); //加入视图
        draw_->showMaximized(); //使得最大化
        cur_widget = draw_;
        ui->mdiArea->setActiveSubWindow(t);
        t->setWindowTitle(tr("draw_widget").arg(1));
        t->show();
    }
    else
    {
        QMessageBox::StandardButton rb = QMessageBox::warning(nullptr, "warning", "新建了图案，是否对原有进行保存？",
                                                             QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                if(rb == QMessageBox::Yes)
                {
                   cur_widget->save_pixmap(); //保存
                }
                ui->mdiArea->closeAllSubWindows();
                myWidget *draw_ = new myWidget;
                QMdiSubWindow * t = ui->mdiArea->addSubWindow(draw_); //加入视图
                draw_->showMaximized(); //使得最大化
                cur_widget = draw_;
                ui->mdiArea->setActiveSubWindow(t);
                t->setWindowTitle(tr("draw_widget").arg(1));
                t->show();
    }
}


void MainWindow::on_fillcolor_clicked()
{
    cur_widget->set_type_to_fillcolor();
}

void MainWindow::on_to_back_clicked() //撤销
{
    cur_widget->set_draw_area_back();
}

void MainWindow::on_clear_clicked() //清屏
{
    cur_widget->set_draw_area_clear();
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    cur_widget->set_pen_width(index);
}

void MainWindow::on_comboBox_2_currentTextChanged(const QString &arg1)
{
    cur_widget->set_pen_color(arg1);
}


void MainWindow::on_action_S_triggered()
{
    cur_widget->save_pixmap();
}


void MainWindow::on_caijian_clicked()
{
    cur_widget->set_cuting(); //设置裁剪
}
