#include "move.h"
#include "planner.h"

#include "search.h"
#include<QtGui>
Point *curPos=new Point;
Point *lastPos=new Point;
std::vector<Point *> body;//机体信息
FoldSchedule *fold_schedule=new FoldSchedule();
extern  std::vector<std::vector<int>> costmap_;
typedef enum{
    INIT,
    FOLD,
    MOVE,
    TURN,
    FOLLOWY,
    NAV,
    FOLLOWWALL,
    SEARCH_WALL
}ShowState;
Move_state::Move_state()
{
    curPos->x=40;
    curPos->y=40;
    startPos->x=curPos->x;
    startPos->y=curPos->y;
    curPos->theta=Direction::left;
    body=nb8(curPos);//绘制机体
    lastPos->x=curPos->x;
    lastPos->y=curPos->y;
    direction=0;
    fold_schedule->init();
}
Move_state::Move_state(const int vl,const int va)
{
    linear_v=vl;
    angular_v=va;
    //当前位置信息？
    curPos->x=40;
    curPos->y=40;
    curPos->theta=Direction::right;
    body=nb8(curPos);
    lastPos->x=curPos->x;
    lastPos->y=curPos->y;
    lastPos->theta=curPos->theta;
    direction=0;
}
Move_state::Move_state(std::vector<std::vector<int>> &map)
{
    curPos->x=40;
    curPos->y=40;
    curPos->theta=Direction::right;
    body=nb8(curPos);
    lastPos->x=curPos->x;
    lastPos->y=curPos->y;
    lastPos->theta=curPos->theta;
    direction=0;
    //map_=map;
}
Point *Move_state::getPose() const
{
    return curPos;
}
void Move_state::posUpdate()//根据curpos绘制机体,更新机体
{
    //cout<<"pos update"<<endl;

    std::vector<Point *> lastBody=nb8(lastPos);//建立上一个body
    for(auto &iter :lastBody)
    {
        costmap_[iter->x][iter->y]=4;
        emit onDrawPose(iter->x,iter->y,4);
    }
   // cout<<"pos update"<<endl;
    body=nb8(curPos);
    for(auto &iter : body)
        {
            if(costmap_[iter->x][iter->y]==2)//起点，终点不绘制
                continue;
            if(iter==curPos)
            {
                costmap_[curPos->x][curPos->y]=3;
                emit onDrawPose(curPos->x,curPos->y,3);//中心点画为橙色
            }
            else if(iter->x==body[curPos->theta]->x && iter->y==body[curPos->theta]->y)
            {
                costmap_[iter->x][iter->y]=3;
                emit onDrawPose(iter->x,iter->y,3);//朝向绘制为橙色
            }
            else
            {
                costmap_[iter->x][iter->y]=1;
                emit onDrawPose(iter->x,iter->y,1);//机体其他部分绘制为蓝色
            }
        }
}
void Move_state::setCurPos(const int x, const int y)
{
    curPos->x=x;
    curPos->y=y;
    startPos->x=x;
    startPos->y=y;
    posUpdate();
}
void Move_state::fold()
{
    Status state=fold_schedule->run();
    //状态显示
    switch (fold_schedule->GetCurrentState()) {
    case INIT:
        emit showState(0);
        break;
   /* case FOLD:
        emit showState(1);
        break;*/
    case MOVE:
        emit showState(2);
        break;
    case TURN:
        emit showState(3);
        break;
    case FOLLOWY:
        emit showState(4);
        break;
    case NAV:
        emit showState(5);
        break;
    case FOLLOWWALL:
        emit showState(6);
        break;
    case SEARCH_WALL:
        emit showState(7);
        break;
    default:
        break;
    }
    if(fold_schedule->draw_path)
    {
        fold_schedule->draw_path=false;
        path=fold_schedule->navstate_.path;
        path.push_front(curPos);
    //绘制路径
    Point *last=path.front();//记录上一个点
    for (auto &p : path)
    {
      emit onDrawPath(last->x,last->y,p->x,p->y,1);
        last=p;
    }
    }
}
void Move_state::fold_start()
{
    fold_schedule->init();
}
