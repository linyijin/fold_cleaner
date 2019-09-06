#include "../../include/state_move.h"
#include "../../include/controller.h"
extern Point *curPos;//全局记录变量
extern Point *lastPos;
extern std::vector<Point *> body;
extern std::vector<std::vector<int>> costmap_;
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
    //state_=move_left;
    direction_=arg->direction_;
    heading_=arg->heading_;

}
void Move::onExit(StateArgs *param)
{
}
Status Move::onInit(StateArgs *param)
{
    curPos->theta=heading_;
    if(overClean())
        return overclean;
    drive();
    return finish;
}
bool Move::overClean()
{
    int delta_x=body[curPos->theta]->x-curPos->x;
    int move_no_imformation=0;
    for(int i=1;i<4;i++)//在转弯之后判断直线运动中是否重复清扫，防止越界
    {
        if(body[curPos->theta]->x+delta_x*i>79||body[curPos->theta]->x+delta_x*i<=0)//越界判断
            continue;
        for(int j=-1;j<2;j++)
        {
            if(costmap_[body[curPos->theta]->x+delta_x*i][body[curPos->theta]->y+j]==0)
                move_no_imformation++;
        }
    }
    if(move_no_imformation==0)
        return true;
     return false;
}
