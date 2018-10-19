#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <qgraphicsitem.h>
#include<QtGui>//mousetrack
#include <QGraphicsRectItem>
#include "atar.h"
#include<iostream>
#include<QTextStream>
#include<stdio.h>
#include<QTWidgets/QVBoxLayout>
MainWindow::MainWindow(QMainWindow*parent) :
    QMainWindow(parent),
    //QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    astar=new Astar();
    //绑定信号/槽
   // connect(ui->pushButton,SIGNAL(clicked()),astar,SLOT(setStart(int,int,int,int)));//设置起点终点
    connect(ui->pushButton,SIGNAL(clicked()),astar,SLOT(calculate()));
    connect(ui->pushButton_2,SIGNAL(clicked()),astar,SLOT(resetMap()));
    connect(this,SIGNAL(obstacleset(int,int)),astar,SLOT(obstacleset(int,int)));//绑定当前函数与astar的函数
    connect(astar,SIGNAL(onDrawPose(int,int,int)),this,SLOT(onDrawPose(int,int,int)));

    //绘制网格地图
    m_sceneSize =QSize(560,560);
    m_cellSize=m_sceneSize/80;
    QGraphicsScene *scene=new QGraphicsScene();
    scene->setSceneRect(0,0,m_sceneSize.width(),m_sceneSize.width());
    ui->map->setScene(scene);
    drawGridMap();
    //鼠标坐标显示标签
    MousePosLabel=new QLabel;
    MousePosLabel->setText(tr("obstacle point"));
    QVBoxLayout *layoutV1=new QVBoxLayout();
    layoutV1->addWidget(MousePosLabel);//子窗口显示
    layoutV1->addStretch();
    QHBoxLayout *mainlayout=new QHBoxLayout();
    mainlayout->addStretch();
    mainlayout->addLayout(layoutV1);
    setLayout(mainlayout);
    //map追踪鼠标
    ui->map->setMouseTracking(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()//直接让clicked函数中调用astar
{
     auto start_x_string=ui->start_x->toPlainText();
     auto start_y_string=ui->start_y->toPlainText();
     auto end_x_string=ui->end_x->toPlainText();
     auto end_y_string=ui->end_y->toPlainText();
     auto string2int =[](char ch){
         if(ch>'0' && ch<'9')
             return (int)(ch-'0');
     };
    int len1=start_x_string.length();
    int start_x=0;
    for(int i=len1-1;i>=0;i--){
        char ch=start_x_string[i].toLatin1();
        start_x+=string2int(ch)*((len1-i-1)*10);
    }
    int len2=start_y_string.length();
    int start_y=0;
    for(int i=len2-1;i>=0;i--){
        char ch=start_y_string[i].toLatin1();
        start_y+=string2int(ch)*((len2-i-1)*10);
    }
    int len3=end_x_string.length();
    int end_x=0;
    for(int i=len3-1;i>=0;i--){
        char ch=end_x_string[i].toLatin1();
        end_x+=string2int(ch)*((len3-i-1)*10);
    }
    int len4=end_y_string.length();
    int end_y=0;
    for(int i=len4-1;i>=0;i--){
        char ch=end_y_string[i].toLatin1();
        end_y+=string2int(ch)*((len4-i-1)*10);
    }
    astar->setStart(start_x,start_y,end_x,end_y);//直接调用对象


}

void MainWindow::onDrawPose(int x,int y,int type)
{
    //std::cout<<"draw"<<x<<' '<<y<<' '<<type<<std::endl;
    QGraphicsRectItem *itemRect=nullptr;
    auto items=ui->map->scene()->items(
                QRectF(x*m_cellSize.width()-2,y*m_cellSize.height()-2,m_cellSize.width()-2,m_cellSize.height()-2)
                );
    for (auto item : items) {
        if (typeid(QGraphicsRectItem) == typeid(*item)) {
            itemRect = dynamic_cast<QGraphicsRectItem *> (item);
            break;
        }
    }
    if (nullptr != itemRect) {
        QColor color(255, 255, 255);
        switch (type) {
            case 0:
                color.setRgb(0xFFFFFF);
                break;
            case 8:
                color.setRgb(0, 0, 0);
                break;
            case 1:
                color.setRgb(0, 128, 255);
                break;
            case 2:
                color.setRgb(0xADFF2F);
                break;
            default:
                color.setRgb(255, 255, 255);
                break;
        }
        itemRect->setBrush(QBrush(color));
        itemRect->setPen(QPen(color));
    }
}
void MainWindow::drawGridMap()
{
    QGraphicsScene *scene = ui->map->scene();
    for (int i = 0; i < 80; i++) {
        for (int j = 0; j < 80; j++) {
            scene->addRect(
                QRectF(i * m_cellSize.width(), j * m_cellSize.height(), m_cellSize.width(), m_cellSize.height()),
                QPen(QColor(255, 255, 255)),
                QBrush(QColor(255, 255, 255)));
        }

    }
}
void MainWindow::mousePressEvent(QMouseEvent *e)
{
    QString str="("+QString::number(e->x())+","+QString::number(e->y())+")";
    if(e->x()>300 && e->x()<871 && e->y()>10 && e->y()<581)
    {
        std::cout<<"mouse"<<e->x()<<' '<<e->y()<<std::endl;
        int x=(e->x()-300)/m_cellSize.width();
        int y=(e->y()-10)/m_cellSize.height();
        emit obstacleset(x,y);//发送障碍物设置信号
        onDrawPose(x,y,8);
    }

    if(e->button()==Qt::LeftButton)
    {
        MousePosLabel->setText(tr("障碍选择位置：")+str);
    }
}
void MainWindow::on_pushButton_2_clicked()
{
}
