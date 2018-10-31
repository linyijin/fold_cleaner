#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "planner.h"
#include "move.h"

#include<cstdlib>
#include<QtGui>//mousetrack
#include <QGraphicsRectItem>
#include<iostream>
#include<QTextStream>
#include<stdio.h>
#include<QTWidgets/QVBoxLayout>

std::vector<std::vector<int>> costmap_(80, std::vector<int>(80, 0));
using std::cout;
MainWindow::MainWindow(QMainWindow*parent) :
    QMainWindow(parent),
    //QWidget(parent),
    ui(new Ui::MainWindow)
{
    timer=new QTimer(this);
    timer->start(50);
    timer2=new QTimer(this);
   // setWindowTitle(tr("路径规划演示"));
    ui->setupUi(this);
    astar=new Astar();
    move=new Move();//设置一个新的运动对象,包括地图

    //设置当前位置
   // move->setCurPos(40,40);
    //绑定信号/槽
   // connect(ui->pushButton,SIGNAL(clicked()),astar,SLOT(setStart(int,int,int,int)));//设置起点终点
    connect(ui->pushButton,SIGNAL(clicked()),astar,SLOT(calculate()));
    connect(ui->pushButton_2,SIGNAL(clicked()),astar,SLOT(resetMap()));
    connect(this,SIGNAL(obstacleset(int,int)),astar,SLOT(obstacleset(int,int)));//绑定当前函数与astar的函数
    connect(astar,SIGNAL(onDrawPose(int,int,int)),this,SLOT(onDrawPose(int,int,int)));
    connect(astar,SIGNAL(onDrawPath(int,int,int,int,int)),this,SLOT(onDrawPath(int,int,int,int,int)));
    connect(astar,SIGNAL(showPath(int)),this,SLOT(showPath(int)));//绑定显示信号
    connect(astar,SIGNAL(resetAxis()),this,SLOT(resetAxis()));
    connect(astar,SIGNAL(showState(int)),this,SLOT(showState(int)));
    connect(move,SIGNAL(showState(int)),this,SLOT(showState(int)));

    //connect(move,SIGNAL(velUpdate(int,int)),this,SLOT(VelUpdate(int,int)));//绑定运动对象更新事件
    connect(timer,SIGNAL(timeout()),move,SLOT(posUpdate()));//计数结束通知Move更新
    connect(move,SIGNAL(onDrawPose(int,int,int)),this,SLOT(onDrawPose(int,int,int)));//绑定move的绘制事件

    connect(timer2,SIGNAL(timeout()),move,SLOT(fold()));



    //绘制网格地图
    m_sceneSize =QSize(581,581);
    m_cellSize=m_sceneSize/80;
    QGraphicsScene *scene=new QGraphicsScene();
    scene->setSceneRect(0,0,m_sceneSize.width(),m_sceneSize.height());
    ui->map->setScene(scene);
    drawGridMap();
    //鼠标坐标显示标签
/*
    MousePosLabel=new QLabel;
    MousePosLabel->setText(tr("obstacle point"));
    QVBoxLayout *layoutV1=new QVBoxLayout();
    layoutV1->addWidget(MousePosLabel);//子窗口显示
    layoutV1->addStretch();
    QHBoxLayout *mainlayout=new QHBoxLayout();
    mainlayout->addStretch();
    mainlayout->addLayout(layoutV1);
    setLayout(mainlayout);
*/
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
//绘制函数
//绘制点
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
                color.setRgb(0xFFFFFF);//白色
                break;
            case 8:
                color.setRgb(0, 0, 0);//黑色
                break;
            case 1:
                color.setRgb(0, 128, 255);//蓝色00EEEE
               // color.setRgb(0x00EEEE);
                break;
            case 2:
                color.setRgb(0xADFF2F);//绿色
                break;
            case 3:
                color.setRgb(0xEE9A00);//橙色
              //  color.setRgb(0xADFF2F);
                break;
            case 4:
                color.setRgb(0xEE0000);//红色
                break;
            default:
                color.setRgb(255, 255, 255);//白色
                break;
        }
        itemRect->setBrush(QBrush(color));
        //itemRect->setPen(QPen(color));
    }
}
void MainWindow::onDrawPath(int x1, int y1, int x2, int y2, int type)
{
    QColor color(255,255,255);
    switch(type)
    {
        case 0:
            color.setRgb(0xFFFFFF);
             break;
       case 1:
            color.setRgb(0,128,255);
            break;
        default:
            break;
    }
    QGraphicsScene *scene=ui->map->scene();
    scene->addLine(x1*m_cellSize.width()+4,y1*m_cellSize.height()+4,x2*m_cellSize.width()+4,y2*m_cellSize.height()+4,QPen(color));
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
//鼠标点击事件
void MainWindow::mousePressEvent(QMouseEvent *e)
{
    QString str="("+QString::number(e->x())+","+QString::number(e->y())+")";
    if(e->x()>225 && e->x()<871 && e->y()>10 && e->y()<581)//障碍设置条件
    {
       // std::cout<<"mouse"<<e->x()<<' '<<e->y()<<std::endl;
        int x=(e->x()-225)/m_cellSize.width()+1;//修正点击坐标200
        int y=(e->y()-8)/m_cellSize.height()-1;//10
        if(setStart || setEnd)
        {
            if(setStart)
            {
                astar->setStart(x,y);
                move->setCurPos(x,y);
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
/*
    if(e->button()==Qt::LeftButton)
    {
        MousePosLabel->setText(tr("障碍选择位置：")+str);
    }
    */
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
    {
    case 0:
        ui->state->setPlainText("can't find path,something wrong");
        break;
    case 1:
        ui->state->setPlainText("move is running");
        break;
    case 2:
        ui->state->setPlainText("bumps");
        break;
    default:
        ui->state->setPlainText(("ok"));
        break;
     }
}
//时钟更新
void MainWindow::VelUpdate(const int vl, const int va)
{
    //std::cout<<"set timer"<<std::endl;
    timer->start(0.2/vl*100000);//设置计时扩大到秒级

}

void MainWindow::on_foldstart_clicked()
{
    timer2->start(100);
}
