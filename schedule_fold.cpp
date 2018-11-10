#include "schedule_fold.h"
#include<vector>
#include<iostream>
using std::cout;
using std::endl;
enum
{
    Q_BUMP_SIG=Q_USER_SIG,
    Q_SLIPPING_SIG,
    Q_WALL_SIG,
    Q_STALL_SIG,
    Q_OUT_BOUND_SIG,
    Q_LOST_WALL_SIG
};
typedef enum
{
    INIT_STATE,
    FOLD_STATE,
    MOVE_STATE,
    TURN_STATE,
    FOLLOWY_STATE
}StateType;
static StateType current_state;
static StateType last_state;


FoldSchedule *FoldSchedule::p_inst=0;//创建一个指针
extern std::vector<std::vector<int>> costmap_;
extern Point* curPos;
extern Point* lastPos;
extern std::vector<Point *> body;

const char* HSM_Evt2Str(Event event)//信号转换为字符
{
    switch(event)
    {
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
    }
   return(char *)0;
}
FoldSchedule::FoldSchedule():QHsm(),
    status_(ready),
    direction_(0),
    stall_count_(0),
    heading_(1)
{
    root_=State((QHsm *)this,(QFun)&FoldSchedule::func);
    initstate_=InitState((QHsm *)this,"init",(State *)0,(QFun)&FoldSchedule::initFunc);
    foldstate_=FoldState((QHsm *)this,"fold",(State *)0,(QFun)&FoldSchedule::foldFunc);
    movestate_=MoveState((QHsm *)this,"move",(State *)(&foldstate_),(QFun)&FoldSchedule::moveFunc);
    turnstate_=TurnState((QHsm *)this,"turn",(State *)(&foldstate_),(QFun)&FoldSchedule::turnFunc);
    followystate_=FollowyState((QHsm *)this,"turn",(State *)(&foldstate_),(QFun)&FoldSchedule::followyFunc);
}
void FoldSchedule::init()
{
    status_=ready;
    direction_=0;
    stall_count_=0;
    heading_=1;
    setInitState(&foldstate_);
}
Event FoldSchedule::initFunc(Event event, StateArgs *param)
{
    cout<<"init state"<<endl;
    switch(event)
    {
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
    }
    return event;
}
Event FoldSchedule::foldFunc(Event event, StateArgs *param)
{
    cout<<"fold\n";
    switch(event)
    {
    case Q_ENTRY_SIG:
    {
        cout<<"fold::entry\n";
        current_state=FOLD_STATE;
        if((void *)0==param)//无参数情况自己定义参数
        {
          cout<<"parem==0?"<<endl;
            direction_=0;
            heading_=1;
        }
        else
        {
            FoldState::Args *arg=(FoldState::Args*)param;
            direction_=arg->direction_;
            heading_=arg->heading_;

        }
       // Move::Args args=Move::Args(foldstate_.direction_,foldstate_.heading_);
       // Tran(&movestate_,&args,0);
        status_=ready;
        break;
    }
    case Q_INIT_SIG:
    {
        cout<<"fold::init"<<endl;
        if(status_==ready)
        {
            Move::Args args=Move::Args(foldstate_.direction_,foldstate_.heading_);
            Tran(&movestate_,&args,0);
            break;
        }
        break;
    }
    case Q_BUMP_SIG:
    {
        //BumpArgs *tmp_args=(BumpArgs*) param;
        //bumpHandle(tmp_args);
        if(foldstate_.heading_==1)
            foldstate_.heading_=7;
        else if(foldstate_.heading_==7)
            foldstate_.heading_=1;
        Turn::Args args=Turn::Args(foldstate_.direction_,foldstate_.heading_);
        Tran(&turnstate_,&args,0);
      return 0;
    }
    case Q_EXIT_SIG:
    {
        last_state=FOLD_STATE;
        break;
    }
    default:
        break;
    }
    return event;
}
Event FoldSchedule::moveFunc(Event event, StateArgs *param)
{
    cout<<"move"<<endl;
    switch(event)
    {
    case Q_ENTRY_SIG:
    {
        current_state=MOVE_STATE;
        movestate_.onEnter(param);
        break;
    }
    case Q_INIT_SIG:
    {
        cout<<"move::init"<<endl;
        Status state=movestate_.onInit(param);
        if(state==finish)
        {
            //::Args args=Move::Args(foldstate_.direction_,foldstate_.heading_);
            //Tran(&movestate_,args,0);
            //break;
        }
        break;
    }
    case Q_BUMP_SIG:
    {
        if(foldstate_.heading_==1)
            foldstate_.heading_==7;
        else if(foldstate_.heading_==7)
            foldstate_.heading_=1;
        Turn::Args args=Turn::Args(foldstate_.direction_,foldstate_.heading_);
        Tran(&turnstate_,&args,0);
        break;
    }
    case Q_EXIT_SIG:
        last_state=MOVE_STATE;
        break;
    default:
        break;
    }
    return event;
}
Event FoldSchedule::turnFunc(Event event, StateArgs *param)
{
    cout<<"turn"<<endl;
    switch(event)
    {
    case Q_ENTRY_SIG:
    {

        current_state=TURN_STATE;
        turnstate_.onEnter(param);
        break;
    }
    case Q_INIT_SIG:
    {
        cout<<"turn::init"<<endl;
        Status state=turnstate_.onInit(param);
        if(state==finish)
        {
            cout<<"turn over"<<endl;
            Followy::Args args=Followy::Args(foldstate_.direction_,foldstate_.heading_);
            Tran(&followystate_,&args,0);
            return 0;
        }
        break;
    }
    case Q_BUMP_SIG:
        cout<<"turn::bump"<<endl;
        break;
    case Q_EXIT_SIG:
        last_state=TURN_STATE;
        break;
    default:
        break;
    }
    return event;
}
Event FoldSchedule::followyFunc(Event event, StateArgs *param)
{
    cout<<"follow y"<<endl;
    switch(event)
    {
    case Q_ENTRY_SIG:
    {
        current_state=FOLLOWY_STATE;
        followystate_.onEnter(param);
        break;
    }
    case Q_INIT_SIG:
    {
        cout<<"followy::init"<<endl;
        Status state=followystate_.onInit(param);
        if(state==finish)
        {
            Move::Args args=Move::Args(foldstate_.direction_,foldstate_.heading_);
            Tran(&movestate_,&args,0);
            return 0;
        }
        break;
    }
    case Q_LOST_WALL_SIG:
    {

    }
    case Q_BUMP_SIG:
        cout<<"followy::bump"<<endl;
        break;
    case Q_EXIT_SIG:
        last_state=FOLLOWY_STATE;
        break;
    default:
        break;
    }
    return event;
}
void FoldSchedule::run()
{
    int delta_x=body[curPos->theta]->x-curPos->x;
    int delta_y=body[curPos->theta]->y-curPos->y;

    State *state=GetState();

    if(state==(&movestate_))
    {
        if(costmap_[body[curPos->theta]->x+delta_x*1][body[curPos->theta]->y]==8 )
            {
       // bump_type=3;//中间碰撞
                cout<<"bump_sig"<<endl;
                BumpArgs bump_args=BumpArgs(3);
                Run(Q_BUMP_SIG,&bump_args);
            }
            else if(costmap_[body[curPos->theta]->x+delta_x*1][body[curPos->theta]->y+1]==8)
            {
                 BumpArgs bump_args=BumpArgs(2);
            //bump_type=2;//下方发生碰撞
                Run(Q_BUMP_SIG,&bump_args);
            }
            else if(costmap_[body[curPos->theta]->x+delta_x*1][body[curPos->theta]->y-1]==8)
            {
                BumpArgs bump_args=BumpArgs(1);
        // bump_type=1;//上方发生碰撞
                Run(Q_BUMP_SIG,&bump_args);
            }
    }
    else if(state==(&follwystate_))
    {
        if(costmap_[body[curPos->theta]->x+(direction_?2:-2)*(delta_y)][body[curPos->theta]->y+((direction_)?2:-2)*(delta_x)]!=8
                &&costmap_[curPos->x+(direction_?2:-2)*(delta_y)][curPos->y+((direction_)?2:-2)*(delta_x)]!=8)
            Run(Q_LOST_WALL_SIG,0);
    }
    else
    {
        Tran(state,(StateArgs *)0,nullptr);
    }

}

