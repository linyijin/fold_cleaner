#ifndef STATE_NAV_H
#define STATE_NAV_H
#include "state.h"
#include "planner.h"
#include "search.h"
#include "pose2d.h"

#include<vector>
#include<list>
struct NavState:State
{
    struct Args:StateArgs
    {
        Args();
        Args(int direction=0,int heading=0):direction_(direction),heading_(heading)
        {
        }
        int direction_;
        int heading_;
    };
    NavState(){}
    NavState(QHsm *hsm,const char *name,State *parent);
    NavState(const NavState &state):State(state),
        direction_(0),
        heading_(1)
    {
        handler_=state.handler_;
    }
    void onEnter(StateArgs *param);
    void onExit(StallArgs *param);
    void bumpHandle(StateArgs *param);
    void stallTrapped(StateArgs *param);
    Status onInit(StateArgs *param);
    Status onBump();
    Status navControl();
    int judgeHeading();//根据左右未清扫点决定方向
    int judgeDirection();//根据上下无信息点决定弓字方向

    std::list<Point *> path;
    std::vector<Point *> target;
private:


    int direction_;
    int heading_;
    Point *end=new Point;



};
#endif // STATE_NAV_H
