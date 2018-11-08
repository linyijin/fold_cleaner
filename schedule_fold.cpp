#include "schedule_fold.h"
#include<vector>
enum
{
    Q_BUMP_SIG=Q_USER_SIG,
    Q_SLIPPING_SIG,
    Q_WALL_SIG,
    Q_STALL_SIG,
    Q_OUT_BOUND_SIG
};
typedef enum
{
    INIT_STATE,
    FOLD_STATE,
    MOVE_STATE
}StateType;
static StateType current_state;
static StateType last_state;

FoldSchedule *FoldSchedule::p_inst=0;//创建一个指针
extern std::vector<std::vector<int>> costmap_;

const char* HSM_Evt2Str(QEvent event)//信号转换为字符
{
    switch(event)
    case Q_INIT_SIG:
        return "Q_INIT_SIG";
    case Q_ENTRY_SIG:
        return "Q_ENTRY_SIG";
    case Q_EXIT_SIG:
        return "Q_EXIT_SIG";
    case Q_BUMP_SIG:
        return "Q_BUMP_SIG";
    case Q_SLIPPING_SIG:
        return "Q_SLIPPING_SIG";
    case Q_WALL_SIG:
        return "Q_WALL_SIG";
    case Q_STALL_SIG:
        return "Q_STALL_SIG";
    case Q_OUT_BOUND_SIG:
        return "Q_OUT_BOUND_SIG";
   return(char *)0;
}
FoldSchedule::FoldSchedule():QHsm(),
    Status(ready),
    direction_(0),
    stall_count_(0),
    heading_(1)
{
    root_=QState((QHsm *)this,(QFun)&FoldSchedule::func);
    initstate_=InitState((QHsm *)this,"init",(QState *)0,(QFun)&FoldSchedule::initFunc);
    foldstate_=FoldState((QHsm *)this,"fold",(QState *)0,(QFun)&FoldSchedule::foldFunc);
    movestate_=MoveState((QHsm *)this,"move",(QState *)(&foldstate_),(QFun)&FoldSchedule::moveFunc);
}
void FoldSchedule::init()
{
    status_=ready;
    direction_=0;
    stall_count_=0;
    heading_=1;
}
QEvent FoldSchedule::initFunc(QEvent event, StateArgs *param)
{
    switch(event)
    case Q_ENTRY_SIG:
        current_state=INIT_STATE;
        break;
    case Q_INIT_SIG:
        break;
    case Q_BUMP_SIG:
        break;
    case Q_EXIT_SIG:
        last_state=INIT_STATE;
        break;
    default:
        break;
    return event;
}
QEvent FoldSchedule::foldFunc(QEvent event, StateArgs *param)
{
    switch(event)
    case Q_ENTRY_SIG:
        current_state=FOLD_STATE;
        if((void *)0==param)//无参数情况自己定义参数
        {
            direction_=0;
            heading_=1;
        }
        else
        {
            FoldState::Args *arg=(FoldState::Args*)param;
            direction_=arg->direction_;
            heading_=arg->heading_;
        }
        status_=ready;
        break;
    case Q_INIT_SIG:
        if(status_==ready)
        {
            Move::Args args=Move::Args(foldstate_.direction_,foldstate_.heading_);
            Tran(&movestate_,args,0);
            break;
        }
        break;
    case Q_BUMP_SIG:
        break;
    case Q_EXIT_SIG:
        last_state=FOLD_STATE;
        break;
    default:
        break;
    return event;
}
QEvent FoldSchedule::moveFunc(QEvent event, StateArgs *param)
{
    switch(event)
    case Q_ENTRY_SIG:
        current_state=MOVE_STATE;
        movestate_.onEnter();
        break;
    case Q_INIT_SIG:
        Status state=movestate_.onInit();
        if(state==finish)
        {
            //::Args args=Move::Args(foldstate_.direction_,foldstate_.heading_);
            //Tran(&movestate_,args,0);
            //break;
        }
        break;
    case Q_BUMP_SIG:
        break;
    case Q_EXIT_SIG:
        last_state=MOVE_STATE;
        break;
    default:
        break;
    return event;
}
void FoldSchedule::run()
{
    int delta_x=body[cur_pos->theta]->x-curPos->x;
    int delta_y=body[curPos->theta]->y-curPos->y;
    if(terminate_)
    {
        QState *state=GetState();
    }

}
