#ifndef MOVE_H
#define MOVE_H
#include<QTGui>
#include<iostream>
#define celldistance 0.2
struct Point{
    int x;
    int y;
};

class Move:public QObject
{
    Q_OBJECT
public:
    Move(const int vl,const int va);
    Move();
    Point getPose() const;
    void setVel(const int vl,const int va);//速度设置
    void setCurPos(const int x,const int y);//设置当前位置

signals:
    void velUpdate(const int vl,const int va);//通知主窗口更新
    void onDrawPose(int x,int y,int type);//通知画点
private slots:
    void posUpdate();//计数刷新

private:
    int linear_v;//线速度
    int angular_v;//角速度

    Point curPos;
    Point *lastPos;
};


#endif // MOVE_H
