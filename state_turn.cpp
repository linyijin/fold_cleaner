#include "state_turn.h"
extern Point *curPos;
extern Point *lastPos;
Turn::Turn():
    direction_(0),
    heading_(1)
{
}
Turn::Turn(const Turn &state):State(state),
    direction_(0),
    heading_(1)
{
    handler_=state.handler_;
}
Turn::Turn(QHsm *hsm,const char *name,State *parent):State(hsm,name,parent),
    direction_(0),
    heading_(1)
{}
void Turn::onEnter(StateArgs *param)
{
    if((void *)0==param)
        return;
    Args *args=(Args*) param;
    direction_=args->direction_;
    heading_=args->heading_;
}
void Turn::onExit(StallArgs *param)
{
    direction_=0;
    heading_=1;
}
Status Turn::onInit(StateArgs *param)
{
    if((void *)0==param)
        return failed;
    Turn::Args *args=(Turn::Args *)param;
    direction_=args->direction_;
    heading_=args->heading_;
    if(direction_)
    {
        curPos->theta=3;
    }
    else
    {
        curPos->theta=5;
    }
    return finish;
}
void Turn::bumpHandle(StateArgs *param)
{}
void Turn::stallTrapped(StateArgs *param)
{}
