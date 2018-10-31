#ifndef MOVE_H
#define MOVE_H
#include<QTGui>
#include<iostream>
#include<vector>

#include "pose2d.h"
#define celldistance 0.2
//extern std::vector<std::vector<int>>

typedef enum
Status{
    running=1,
    bump,//直线碰撞
    turn_bump,//转弯碰撞
    turn_finish,
    finish,
    failed,
    navigation
}state;
class Move:public QObject
{
    Q_OBJECT
public:
    enum Direction{
        top_left=0,left,buttom_left,
        top,mid,buttom,
        top_right,right,buttom_right
    };

    Move(std::vector<std::vector<int>> &map);//传入地图初始化
    Move(const int vl,const int va);
    Move();
    Point *getPose() const;
    void setVel(const int vl,const int va);//速度设置
    void setCurPos(const int x,const int y);//设置当前位置
    //std::vector<Point *> nb8( Point* cur);

    //分解的fold函数
    state fold_run();//用于判断当前状态选择执行函数
    void fold_move();
    void fold_turn();
    void fold_turnBack();//完成转弯
    state fold_nav();

    //碰撞处理函数
    void bumpHandle(const int bump_type);//处理碰撞

signals:
    void velUpdate(const int vl,const int va);//通知主窗口更新
    void onDrawPose(int x,int y,int type);//通知画点
    void showState(int type);
private slots:
    void posUpdate();//计数刷新
    void fold();

private:
    int linear_v;//线速度
    int angular_v;//角速度
    std::vector<Point *> body;//机体信息
    Point *curPos=new Point;
    Point *lastPos=new Point;
    int lastTheta;
    int bump_type;//碰撞类型
};


#endif // MOVE_H
