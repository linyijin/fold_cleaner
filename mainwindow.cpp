#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<cstdlib>

//#include <qgraphicsitem.h>
#include<QtGui>//mousetrack
#include <QGraphicsRectItem>
#include "atar.h"
#include<iostream>
#include<QTextStream>
#include<stdio.h>
#include<QTWidgets/QVBoxLayout>
using std::cout;
MainWindow::MainWindow(QMainWindow*parent) :
    QMainWindow(parent),
    //QWidget(parent),
    ui(new Ui::MainWindow)
{
    setWindowTitle(tr("路径规划演示"));
    ui->setupUi(this);
    astar=new Astar();
    //绑定信号/槽
   // connect(ui->pushButton,SIGNAL(clicked()),astar,SLOT(setStart(int,int,int,int)));//设置起点终点
    connect(ui->pushButton,SIGNAL(clicked()),astar,SLOT(calculate()));
    connect(ui->pushButton_2,SIGNAL(clicked()),astar,SLOT(resetMap()));
    connect(this,SIGNAL(obstacleset(int,int)),astar,SLOT(obstacleset(int,int)));//绑定当前函数与astar的函数
    connect(astar,SIGNAL(onDrawPose(int,int,int)),this,SLOT(onDrawPose(int,int,int)));
    connect(astar,SIGNAL(showPath(int)),this,SLOT(showPath(int)));//绑定显示信号
    connect(astar,SIGNAL(resetAxis()),this,SLOT(resetAxis()));
    connect(astar,SIGNAL(showState(int)),this,SLOT(showState(int)));

    //绘制网格地图
    m_sceneSize =QSize(571,571);
    m_cellSize=m_sceneSize/80;
    QGraphicsScene *scene=new QGraphicsScene();
    scene->setSceneRect(0,0,m_sceneSize.width(),m_sceneSize.height());
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
    if(!clickSet){
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
    astar->setStart(start_x,start_y);//直接调用对象
    astar->setEnd(end_x,end_y);
    }
    ui->pushButton->setDisabled(true);
    clickSet=false;


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
            case 4:
                color.setRgb(0xEE0000);//3画不出来？
                break;
            default:
                color.setRgb(255, 255, 255);
                break;
        }
        itemRect->setBrush(QBrush(color));
        //itemRect->setPen(QPen(color));
    }
}
void MainWindow::drawGridMap()//绘制空白地图
{
    QGraphicsScene *scene = ui->map->scene();
    for (int i = 0; i < 80; i++) {
        for (int j = 0; j < 80; j++) {
            scene->addRect(
                QRectF(i * m_cellSize.width(), j * m_cellSize.height(), m_cellSize.width(), m_cellSize.height()),
                QPen(QColor(194, 194, 194)),//绘制网格
                QBrush(QColor(255, 255, 255)));
        }

    }
}
void MainWindow::mousePressEvent(QMouseEvent *e)
{
    QString str="("+QString::number(e->x())+","+QString::number(e->y())+")";
    if(e->x()>300 && e->x()<871 && e->y()>10 && e->y()<581)//障碍设置条件
    {
        //std::cout<<"mouse"<<e->x()<<' '<<e->y()<<std::endl;
        int x=(e->x()-300)/m_cellSize.width()+1;//修正点击坐标
        int y=(e->y()-10)/m_cellSize.height()-1;
        if(setStart || setEnd)
        {
            if(setStart)
            {
                astar->setStart(x,y);
                char axis[5];
                itoa(x,axis,10);
                ui->start_x->setPlainText(axis);
                itoa(y,axis,10);
                ui->start_y->setPlainText(axis);
            }
            else
            {
                astar->setEnd(x,y);
                char axis[5];
                itoa(x,axis,10);
                ui->end_x->setPlainText(axis);
                itoa(y,axis,10);
                ui->end_y->setPlainText(axis);
            }
        }

        else
        {
            for(int i=-1;i<2;i++)//一次画八个障碍点
            {
                for(int j=-1;j<2;j++)
                {
                    emit obstacleset(x+i,y+j);//发送障碍物设置信号
                    onDrawPose(x+i,y+j,8);
                }
             }
        }
    }

    if(e->button()==Qt::LeftButton)
    {
        MousePosLabel->setText(tr("障碍选择位置：")+str);
    }
}
void MainWindow::on_pushButton_2_clicked()
{
    ui->pushButton->setDisabled(false);
}

void MainWindow::on_start_set_button_clicked()//设置起点
{
    if(firstStart)
    {
        ui->start_set_button->setText(tr("点击完成设置"));
        setStart=true;
        firstStart=false;
        setEnd=false;
        firstEnd=true;
        ui->end_set_button->setDisabled(true);
        ui->pushButton->setDisabled(true);

    }
    else
    {
        ui->start_set_button->setText(tr("点击输入起始点"));
        setStart=false;
        firstStart=true;
        ui->end_set_button->setDisabled(false);
        ui->pushButton->setDisabled(false);

    }
}

void MainWindow::on_end_set_button_clicked()//设置终点
{
    if(firstEnd)
    {
        ui->end_set_button->setText(tr("点击完成设置"));
        setEnd=true;
        firstEnd=false;
        setStart=false;
        firstStart=true;
        ui->start_set_button->setDisabled(true);
        ui->pushButton->setDisabled(true);

    }
    else
    {
        ui->end_set_button->setText(tr("点击输入终点"));
        setEnd=false;
        firstEnd=true;
        clickSet=true;//手动输入模式
        ui->start_set_button->setDisabled(false);
        ui->pushButton->setDisabled(false);
    }
}
void MainWindow::showPath(int len)
{
    char string_len[5];
    itoa(len,string_len,10);
    ui->path->setText(string_len);
}
void MainWindow::resetAxis()//清空操作要重新写
{
    ui->start_x->setPlainText("0");
    ui->start_y->setPlainText("0");
    ui->end_x->setPlainText("0");
    ui->end_y->setPlainText("0");
}
void MainWindow::showState(int type)
{
    switch(type)
    case 0:
        ui->state->setPlainText("can't find path,something wrong");
}
