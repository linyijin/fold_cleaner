#include "state_nav.h"
extern std::vector<std::vector<int>> costmap_;
extern Point* curPos;
extern Point* lastPos;
extern std::vector<Point *> body;
NavState::NavState(QHsm *hsm,const char *name,State *parent):State(hsm,name,parent)
{
}
void NavState::onEnter(StateArgs *param)
{
}
void NavState::onExit(StallArgs *param)
{
}
void NavState::bumpHandle(StateArgs *param)
{
}
void NavState::stallTrapped(StateArgs *param)
{
}
Status NavState::onInit(StateArgs *param)
{
    Searcher *searcher=new Searcher();//分配内存
    Astar *astar=new Astar();
    //寻找新点
    try
    {
        target=searcher->SearcherFrom(curPos);
    }
    catch(int i)
    {
        cout<<"can't find target"<<endl;
        return clean_finish;
    }
    //获取路径
    end=target.front();

    astar->setStart(curPos->x,curPos->y);
    astar->setEnd(end->x,end->y);
    astar->calculate();
    path=astar->returnPath();

    if(path.empty())//没有路径可以到达
    {
        return failed;
    }
   return success;
}
Status NavState::navControl()
{
    Point *next;
    if(path.size()>=1)
    {
          curPos=path.front();
          path.pop_front();
          next=path.front();
          curPos->theta=(next->y-curPos->y)+(next->x-curPos->x-1)*3+8-1;//delta_y+(delta_x-1)*3+8
          return controlling;
    }
    curPos->theta=path.front()->theta;
    return finish;
}
int NavState::judgeHeading()
{
    int leftSize=0;
    int rightSize=0;
    for(int i=1;i<5;i++)
    {
        for(int j=-1;j<2;j++)
        {
        if(curPos->x+i>=79 || curPos->x-i<=0)
            continue;
        if(costmap_[curPos->x+i][curPos->y+j]==0)
            rightSize++;
        if(costmap_[curPos->x-i][curPos->y+j]==0)
            leftSize++;
        }
    }
    if(rightSize>=leftSize)
        return 7;
    else
        return 1;
}
int NavState::judgeDirection()
{
    int topSize=0;
    int buttomSize=0;
    for(int i=1;i<10;i++)
    {
        if(curPos->y+i>=79 || curPos->y-i<=0)//越界
            continue;
        if(costmap_[curPos->x][curPos->y+i]==0)
            buttomSize++;
        if(costmap_[curPos->x][curPos->y-i]==0)
            topSize++;
    }
    if(topSize>buttomSize)
        return 1;
    else
        return 0;
}
Status NavState::onBump()
{
    Astar *astar=new Astar();
    astar->setStart(curPos->x,curPos->y);
    astar->setEnd(end->x,end->y);
    astar->calculate();
    path=astar->returnPath();
}
