#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QLabel>
#include "atar.h"
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
protected:
    void mousePressEvent(QMouseEvent *e);//添加按键检测事件，用于障碍物添加
private:
    Ui::MainWindow *ui;
    Astar* astar;
    QSize m_sceneSize;
    QSize m_cellSize;
    QLabel *MousePosLabel;//添加显示鼠标点击坐标
};

#endif // MAINWINDOW_H
