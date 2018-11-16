#ifndef STATE_FOLLOWWALL_H
#define STATE_FOLLOWWALL_H
#include "state.h"
struct FollowWall:State
{
    struct Args:StateArgs
    {
        Args();
    };
    FollowWall():State(){}
    FollowWall(const FollowWall&rhs):State(rhs){}
    FollowWall(QHsm *hsm,const char *name,State *parent=0):State(hsm,name,parent){}
    ~FollowWall(){}

    void onEnter(StateArgs *param);
    void onExit(StateArgs *param);
    Status onInit(StateArgs *param);

    void bumpHandle(StateArgs *param);
    Status followWall();
};
#endif // STATE_FOLLOWWALL_H
