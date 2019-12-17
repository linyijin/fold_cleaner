#include "../include/mainwindow.h"
#include "ui_mainwindow.h"
#include "../include/planner.h"
#include "../include/move.h"
#include "../include/pose2d.h"

#include<cstdlib>
#include<QtGui>//mousetrack
#include <QGraphicsRectItem>
#include<iostream>
#include<QTextStream>
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<QTWidgets/QVBoxLayout>

std::vector<std::vector<int>> costmap_(80, std::vector<int>(80, 0));
using std::cout;
MainWindow::MainWindow(QMainWindow*parent) :
    QMainWindow(parent),
    //QWidget(parent),
    ui(new Ui::MainWindow)
{
    timer=new QTimer(this);
    //timer->start(10);
    timer2=new QTimer(this);
   // setWindowTitle(tr("路径规划演示"));
    ui->setupUi(this);
    astar=new Astar();
    move=new Move_state();//设置一个新的运动对象,包括地图
  //  fold_schedule=new FoldSchedule();

    //设置当前位置
   // move->setCurPos(40,40);
    //绑定信号/槽
   // connect(ui->pushButton,SIGNAL(clicked()),astar,SLOT(setStart(int,int,int,int)));//设置起点终点
    connect(ui->pushButton,SIGNAL(clicked()),astar,SLOT(calculate()));
    connect(ui->reClean,SIGNAL(clicked()),astar,SLOT(reClean()));
    connect(ui->resetMap,SIGNAL(clicked()),astar,SLOT(resetMap()));
    connect(this,SIGNAL(obstacleset(int,int)),astar,SLOT(obstacleset(int,int)));//绑定当前函数与astar的函数
    connect(astar,SIGNAL(onDrawPose(int,int,int)),this,SLOT(onDrawPose(int,int,int)));
    connect(astar,SIGNAL(onDrawPath(int,int,int,int,int)),this,SLOT(onDrawPath(int,int,int,int,int)));
    connect(astar,SIGNAL(showPath(int)),this,SLOT(showPath(int)));//绑定显示信号
    connect(astar,SIGNAL(resetAxis()),this,SLOT(resetAxis()));
    connect(astar,SIGNAL(showState(int)),this,SLOT(showState(int)));
    connect(move,SIGNAL(showState(int,int)),this,SLOT(showState(int,int)));

    //connect(move,SIGNAL(velUpdate(int,int)),this,SLOT(VelUpdate(int,int)));//绑定运动对象更新事件
    connect(timer,SIGNAL(timeout()),move,SLOT(posUpdate()));//计数结束通知Move更新
    connect(move,SIGNAL(onDrawPose(int,int,int)),this,SLOT(onDrawPose(int,int,int)));//绑定move的绘制事件
     connect(move,SIGNAL(onDrawPath(int,int,int,int,int)),this,SLOT(onDrawPath(int,int,int,int,int)));
    connect(move,SIGNAL(showPlanCount(int)),this,SLOT(showPlanCount(int)));
    connect(this,SIGNAL(fold_start()),move,SLOT(fold_start()));
    connect(this,SIGNAL(fold_start()),astar,SLOT(resetMap()));
     connect(move,SIGNAL(stop()),this,SLOT(stop()));

    connect(timer2,SIGNAL(timeout()),move,SLOT(fold()));
    connect(astar,SIGNAL(showSum(int,int)),this,SLOT(showSum(int,int)));




    //绘制网格地图
    m_sceneSize =QSize(580,580);
    m_cellSize=m_sceneSize/80;
    QGraphicsScene *scene=new QGraphicsScene();
    scene->setSceneRect(0,0,m_sceneSize.width(),m_sceneSize.height());
    ui->map->setScene(scene);
    drawGridMap();
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
    //std::cout<<e->x()<<' '<<e->y()<<endl;
    if(e->x()>225 && e->x()<(79*m_cellSize.width()+220) && e->y()>10 && e->y()<581)//障碍设置条件
    {
       // std::cout<<"mouse"<<e->x()<<' '<<e->y()<<std::endl;
        int x=(e->x()-220)/m_cellSize.width()+1;//修正点击坐标200
        int y=(e->y()-20)/m_cellSize.height()-1;//10
       // cout<<"map="<<x<<' '<<y<<endl;
        if(x>=79)
        {
            x=78;
        }
        if(y>=79)
        {
            y=78;
        }
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
    //ui->path->setText(string_len);
}
void MainWindow::resetAxis()//清空操作要重新写
{
    ui->start_x->setPlainText("0");
    ui->start_y->setPlainText("0");
    ui->end_x->setPlainText("0");
    ui->end_y->setPlainText("0");
}
void MainWindow::showState(int type,int count_)
{
    switch(type)
    {
    case 0:
        ui->state->setPlainText("fold::init");
        break;
    case 1:
        ui->state->setPlainText("enter fold:");
        break;
    case 2:
        ui->state->setPlainText("直线");
        break;
    case 3:
        ui->state->setPlainText("转向");
        break;
    case 4:
        ui->state->setPlainText("拐弯");
        break;
    case 5:
         ui->state->setPlainText("路径规划中，可能需要较长时间，请等待");
        break;
    case 6:
    {
        char count[2];
         ui->state->setPlainText("沿墙清扫中,30s后将开始路径规划，若重新清扫请点击'重新清扫'");
        break;
    }
    case 7:
         ui->state->setPlainText("路径规划失败，寻墙");
        break;
    case 8:
         ui->state->setPlainText("clean is finish");
        break;
    default:
        ui->state->setPlainText(("ok"));
        break;
     }
}
//时钟更新
void MainWindow::VelUpdate(const int vl, const int va)
{
}

void MainWindow::on_foldstart_clicked()
{
    timer2->start(100);
    timer->start(100);
    fold_start();
    ui->foldstart->setDisabled(true);
}

void MainWindow::on_stop_clicked()
{
    if(firstStop)
    {
         ui->stop->setText("继续运行");
         stop();
         firstStop=false;
    }
    else
    {
        ui->stop->setText("暂停");
        reStart();
        firstStop=true;
    }
    showArea();
}
void MainWindow::showSum(int len,int sum)
{
    QString lenstr=QString::number(len);
    QString sumstr=QString::number(sum);
    QString str=QString(lenstr+'='+sumstr);
    ui->path->setText(str);
}
void MainWindow::showArea()//显示清扫区域面积
{
    int _area=0;
    for(int i=0;i<80;i++){
        for(int j=0;j<80;j++)
        {
             if(costmap_[i][j]==4)
                 _area++;
        }
    }
    QString _strArea=QString::number(_area);
    ui->area->setText(_strArea);
}
void MainWindow::showPlanCount(int count)//显示规划路径次数
{
    QString _strPlan=QString::number(count);
    ui->planCount->setText(_strPlan);
}
void MainWindow::stop()
{
       timer2->stop();
       timer->stop();
}
void MainWindow::reStart()
{
    timer2->start();
    timer->start();
}

void MainWindow::on_resetMap_clicked()
{
    //ui->map->scene()->clear();
}

void MainWindow::on_reClean_clicked()
{
    ui->pushButton->setDisabled(false);
   // ui->map->scene()->clear();//全部清除
    ui->foldstart->setDisabled(false);
    if(!firstStop)
    {
        ui->stop->setText("暂停");
        reStart();
        firstStop=true;
    }
    drawGridMap();
}
