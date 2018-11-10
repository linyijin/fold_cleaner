#include "state_move.h"
extern Point *curPos;//全局记录变量
extern Point *lastPos;
Move::Move():
 direction_(0),
   heading_(1),
state_(move_left)
 {

 }
Move::Move(const Move&rhs):State(rhs),
     direction_(0),
    state_(move_left)
 {
 }
Move::Move(QHsm *hsm,const char *name,State *parent):State(hsm,name,parent),
    direction_(0),
    heading_(1),
   state_(move_left)
{
}
void Move::onEnter(StateArgs *param)
{
    if((void *)0==param)
        return;
    Args* arg=(Args*)param;
    state_=move_left;
    direction_=arg->direction_;
    heading_=arg->heading_;

}
void Move::onExit(StateArgs *param)
{
}
Status Move::onInit(StateArgs *param)
{
   // emit showState(1);
    curPos->theta=heading_;
   // int delta_x=body[curPos->theta]->x-curPos->x;
}
