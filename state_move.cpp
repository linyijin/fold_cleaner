#include "state_move.h"
 Move::Move():
 direction_(0),
   heading_(1),
state_(move_left)
 {}
 Move::Move(const Move&rhs):QState(rhs),
     direction_(0),
    state_(move_left)
 {
 }
Move::Move(QHsm &hsm,const char *name,QState *parent=0):QState(hsm,name,parent),
    direction_(0),
    heading_(1),
   state_(move_left)
{
}
void Move::onEnter(StateArgs *param)
{
    if((void *)0==param)
        return;
    Args* arg=(Arg*)param;
    state_=move_left;
    direction_=arg->direction;
    heading_=arg->heading;

}
void Move::onExit(StateArgs *param)
{
}
Status Move::onInit(StateArgs *param)
{
   // emit showState(1);
    curPos->theta=heading_;
    int delta_x=body[curPos->theta]->x-curPos->x;
    lastPos->x=curPos->x;
    lastPos->y=curPos->y;
    lastPos->theta=curPos->theta;
    curPos->x=curPos->x+delta_x*1;
    return running;
}
