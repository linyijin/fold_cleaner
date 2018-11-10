#include "state_followy.h"
#include<vector>
extern Point *curPos;
extern Point *lastPos;
extern std::vector<Point *> body;
extern std::vector<std::vector<int>> costmap_;
Followy::Followy()
{
}
Followy::Followy(QHsm *hsm, const char *name, State *parent):State(hsm,name,parent)
{
}
void Followy::onEnter(StateArgs *param)
{
    starty=curPos->y;
    if((void *)0==param)
        return;
    Args *args=(Args*)param;
    direction_=args->direction_;
    heading_=args->heading_;

}
void Followy::onExit(StallArgs *param)
{
}
Status Followy::onInit(StateArgs *param)
{
    int delta_x=abs(curPos->x-body[curPos->theta]->x);
    int delta_y=abs(curPos->y-body[curPos->theta]->y);
    if(abs(curPos->y-starty)<=2)//位移超过2
    {
        return running;
    }
    else
        return finish;
}
