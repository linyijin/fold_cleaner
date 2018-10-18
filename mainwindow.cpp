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
    connect(ui->pushButton,SIGNAL(clicked()),astar,SLOT(calculate()));
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

void MainWindow::on_pushButton_clicked()
{
     ui->pushButton->setText(tr("close"));

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

