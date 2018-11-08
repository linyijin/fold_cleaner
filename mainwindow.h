#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QLabel>
#include "planner.h"
#include "move.h"
#include<qtimer.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
Q_OBJECT
public:
    explicit MainWindow(QMainWindow*parent = nullptr);
    ~MainWindow();
    void drawGridMap();
signals:
    void obstacleset(int x,int y);//设置障碍物显示信号

private slots:
    void on_pushButton_clicked();
    void onDrawPose(int x,int y,int type);
    void onDrawPath(int x1,int y1,int x2,int y2,int type);

    void on_pushButton_2_clicked();

    void on_start_set_button_clicked();

    void on_end_set_button_clicked();
    void showPath(int len);//路径长度显示
    void resetAxis();//起始点信息栏显示重置
    void showState(int type);//显示信息状态

    void VelUpdate(const int vl,const int va);//接收速度设置信号
    void stop();//接收move发来的停止信号
    void reStart();

  //  void posUpdate();


    void on_foldstart_clicked();


    void on_stop_clicked();

protected:
    void mousePressEvent(QMouseEvent *e);//添加按键检测事件，用于障碍物添加

private:
    Ui::MainWindow *ui;
    Astar* astar;
    Move *move;
    QSize m_sceneSize;
    QSize m_cellSize;
    QLabel *MousePosLabel;//添加显示鼠标点击坐标
    bool refresh;//清空
    bool setStart=false;
    bool firstStart=true;
    bool firstStop=true;
    bool setEnd=false;
    bool firstEnd=true;
    bool clickSet=false;
    QTimer *timer;//定义一个更新时钟
    QTimer *timer2;//定义第二个时钟用来刷新运动

};

#endif // MAINWINDOW_H
