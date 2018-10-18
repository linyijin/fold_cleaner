#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H
#include<QtGui>
#include<QMainWindow>
#include<QLabel>
#include<QMouseEvent>

class MouseEvent: public QMainWindow//继承主窗口
{
    Q_OBJECT
public:
    MouseEvent(QMainWindow *parent=0);
};
#endif // MOUSEEVENT_H
