#ifndef MOVE_H
#define MOVE_H
#include<QTGui>
#include<iostream>
#include<vector>
#include<list>
#include "pose2d.h"
#include "schedule_fold.h"
#define celldistance 0.2
//extern std::vector<std::vector<int>>
/*
typedef enum
Status{
    running=1,
    bump,//直线碰撞
    turn_bump,//转弯碰撞
    turn_finish,
    follow_finish,
    overclean,
    stall,
    overclean_move,//直线运动中发送重复清扫
    nav_finish,
    controlling,
    finish,
    failed,
    follow_wall,
    follow_judge,
    lostWall,

    navigation
}state;*/
class Move_state:public QObject
{
    Q_OBJECT
public:
    enum Direction{
        top_left=0,left,buttom_left,
        top,mid,buttom,
        top_right,right,buttom_right
    };

    Move_state(std::vector<std::vector<int>> &map);//传入地图初始化
    Move_state(const int vl,const int va);
    Move_state();
    Point *getPose() const;
    void setVel(const int vl,const int va);//速度设置
    void setCurPos(const int x,const int y);//设置当前位置
    //std::vector<Point *> nb8( Point* cur);
    std::vector<Point *> angle(Point* cur,int direc,int head);//三角形障碍判断区域,direc=0：左边，direc=1:右边,head 三角形的正负：0：+，1：-

    //分解的fold函数
    bool overClean();//判断是否重复清扫，重复清扫则要重新找点
    /*state fold_run();//用于判断当前状态选择执行函数
    void fold_move();
    void fold_turn();
    void fold_turnBack();//完成转弯
    state fold_follow();//沿墙:绕过障碍物 重复清扫进入导航，返回
    state fold_nav(Point *target);
    state nav_control();
    state fold_followWall();
    state searchWall();*/

    //碰撞处理函数
    void bumpHandle(const int bump_type);//处理碰撞
    int judgeHeading();//根据左右未清扫点决定方向
    int judgeDirection();//根据上下无信息点决定弓字方向

signals:
    void velUpdate(const int vl,const int va);//通知主窗口更新
    void onDrawPose(int x,int y,int type);//通知画点
    void onDrawPath(int x1,int y1,int x2,int y2,int type);
    void showState(int type);
    void stop();
private slots:
    void posUpdate();//计数刷新
    void fold();

private:
    int linear_v;//线速度
    int angular_v;//角速度


    Point *startPos=new Point;
    Point *stallPos=new Point;
    int bump_type;//碰撞类型
    bool inFollow=false;//follow模式判断
    bool pathOver=false;//规划完毕
    bool navControl=false;//规划路径控制
    bool followwall=false;
    bool underWall=false;
    int direction;
    int stallCount=0;
    int timeCount=0;
    std::list<Point *> path;

};


#endif // MOVE_H
