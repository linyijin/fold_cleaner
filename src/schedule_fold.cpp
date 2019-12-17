#include "../include/schedule_fold.h"
#include "../include/controller.h"
#include<vector>
#include<iostream>
using std::cout;
using std::endl;
int stall_count_;
enum
{
    Q_BUMP_SIG=Q_USER_SIG,
    Q_SLIPPING_SIG,
    Q_WALL_SIG,
    Q_STALL_SIG,
    Q_OUT_BOUND_SIG,
    Q_LOST_WALL_SIG,
    Q_FOLLOW_PATH_SIG,
    Q_RESEARCH_SIG
};
/*
typedef enum
{
    INIT_STATE,
    FOLD_STATE,
    MOVE_STATE,
    TURN_STATE,
    FOLLOWY_STATE,
    NAV_STATE,
    FOLLOWWALL_STATE,
    SEARCH_WALL_SATATE
}StateType;
static StateType current_state;
static StateType last_state;
*/

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
    heading_(1)
{
    root_=State((QHsm *)this,(QFun)&FoldSchedule::func);
    initstate_=InitState((QHsm *)this,"init",(State *)0,(QFun)&FoldSchedule::initFunc);
    foldstate_=FoldState((QHsm *)this,"fold",(State *)0,(QFun)&FoldSchedule::foldFunc);
    movestate_=MoveState((QHsm *)this,"move",(State *)(&foldstate_),(QFun)&FoldSchedule::moveFunc);
    turnstate_=TurnState((QHsm *)this,"turn",(State *)(&foldstate_),(QFun)&FoldSchedule::turnFunc);
    followystate_=FollowyState((QHsm *)this,"followy",(State *)(&foldstate_),(QFun)&FoldSchedule::followyFunc);
    searchwallstate_=SearchWall((QHsm *)this,"searchwall",(State *)(&foldstate_),(QFun)&FoldSchedule::SearchwallFunc);
    followwallstate_=Followwall((QHsm *)this,"followwall",(State *)(&searchwallstate_),(QFun)&FoldSchedule::followwallFunc);


    navstate_=Navstate((QHsm *)this,"nav",(State *)(&foldstate_),(QFun)&FoldSchedule::navFunc);
}
StateType FoldSchedule::GetCurrentState()
{
    return current_state;
}
void FoldSchedule::init()
{
    status_=ready;
    direction_=0;
    stall_count_=0;
    heading_=1;
    draw_path=false;
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
        cout<<"fold::bump"<<endl;
         return 0;
        //break;
    }
    case Q_STALL_SIG:
    {
        cout<<"stall"<<endl;
        Tran(&navstate_,nullptr,0);
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

    switch(event)
    {
    case Q_ENTRY_SIG:
    {
        cout<<"move::enter"<<endl;
        current_state=MOVE_STATE;
        movestate_.onEnter(param);
        break;
    }
    case Q_INIT_SIG:
    {
        cout<<"move::init"<<endl;
        Status state=movestate_.onInit(param);
        switch(state)
        {
        case overclean:
        {
            cout<<"move::over clean"<<endl;
            Tran(&navstate_,nullptr,0);
            return 0;
        }
        case finish:
            return 0;
        default:
            break;
        }
        return 0;
    }
    case Q_BUMP_SIG:
    {
        if(foldstate_.heading_==1)
            foldstate_.heading_=7;
        else if(foldstate_.heading_==7)
            foldstate_.heading_=1;
        Turn::Args args=Turn::Args(foldstate_.direction_,foldstate_.heading_);
        Tran(&turnstate_,&args,0);
        return 0;
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

    switch(event)
    {
    case Q_ENTRY_SIG:
    {
        cout<<"turn::enter"<<endl;
        current_state=TURN_STATE;
        turnstate_.onEnter(param);
        break;
    }
    case Q_INIT_SIG:
    {
        cout<<"turn::init"<<endl;
        if(curPos->theta==1)
            foldstate_.last_heading_=1;
        else
            foldstate_.last_heading_=7;
        Status state=turnstate_.onInit(param);
        if(state==finish)
        {
            Followy::Args args=Followy::Args(foldstate_.direction_,foldstate_.heading_,foldstate_.last_heading_);
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

    switch(event)
    {
    case Q_ENTRY_SIG:
    {
         cout<<"followy::enter"<<endl;
        current_state=FOLLOWY_STATE;
        followystate_.onEnter(param);
        break;
    }
    case Q_INIT_SIG:
    {
        if(followy_count>10){
            Tran(&navstate_,nullptr,0);
            return 0;
        }
        if(abs(followystate_.start->x-curPos->x)==0)//在同一条线上没有变化
            stall_count_++;
        else
            stall_count_=0;
        Status state=followystate_.onInit(param);
        switch(state)
        {
        case finish:
        {
            cout<<"followy::init state finish"<<endl;
            Move::Args args=Move::Args(foldstate_.direction_,foldstate_.heading_);
            Tran(&movestate_,&args,0);
            return 0;
        }
        case overclean:
        {
            cout<<"followy::over clean"<<endl;
            Tran(&navstate_,nullptr,0);
            return 0;
        }
        default:
            break;
        }
        break;
    }
    case Q_LOST_WALL_SIG:
    {
       if(abs(curPos->y-followystate_.starty)>1)
       {
           Move::Args args=Move::Args(foldstate_.direction_,foldstate_.heading_);
           Tran(&movestate_,&args,0);
       }
       else
       Status state_=followystate_.followWall();
        return 0;
    }
    case Q_BUMP_SIG:
    {
        cout<<"followy::bump"<<endl;
        if(abs(curPos->y-followystate_.starty)>1)
        {
            Move::Args args=Move::Args(foldstate_.direction_,foldstate_.heading_);
            Tran(&movestate_,&args,0);
        }
        else
            followystate_.bumpHandle(param);
        return 0;
    }
    case Q_EXIT_SIG:
        followy_count=0;
        last_state=FOLLOWY_STATE;
        break;
    default:
        break;
    }
    return event;
}
Event FoldSchedule::navFunc(Event event, StateArgs *param)
{
    switch(event)
    {
    case Q_ENTRY_SIG:
    {
        current_state=NAV_STATE;
        navstate_.path.clear();
        navstate_.target.clear();
        break;
    }
    case Q_INIT_SIG:
    {
        cout<<"nav::init"<<endl;
        Status state=navstate_.onInit(param);
        switch(state)
        {
            case success:
            {
                plan_count++;
                draw_path=true;
                for(auto &iter : navstate_.path)
                    cout<<iter<<endl;
                 navstate_.path.pop_front();
                return 0;
            }
            case clean_finish:
            {
                cout<<"nav::clean finish"<<endl;
                 Tran(&searchwallstate_,nullptr,0);
                 return 0;
            }
        case failed:
        {
            cout<<"nav::navigation failed"<<endl;
             Tran(&searchwallstate_,nullptr,0);
             return 0;
        }
        default:
            break;
        }

        break;
    }
    case Q_FOLLOW_PATH_SIG:
    {
        cout<<"nav::follow path"<<endl;
        if(navstate_.path.empty())
        {
            cout<<"nav::follow path finish,tran to move"<<endl;
            curPos->theta=navstate_.judgeHeading();
            foldstate_.direction_=navstate_.judgeDirection();
            foldstate_.heading_=navstate_.judgeHeading();
            cout<<"judge direction="<<foldstate_.direction_<<endl;
            cout<<"judge heading="<<foldstate_.heading_<<endl;
            Move::Args args=Move::Args(foldstate_.direction_,foldstate_.heading_);
            Tran(&movestate_,&args,0);
        }
        else
        {
            if(navstate_.path.size()>1)
            {
                Point *next=new Point;
                next=navstate_.path.front();
                curPos->theta=(next->y-curPos->y)+(next->x-curPos->x-1)*3+8-1;//delta_y+(delta_x-1)*3+8
                drive();
               navstate_.path.pop_front();
            }
            else
            {
                curPos->theta=(navstate_.path.front()->y-curPos->y)+(navstate_.path.front()->x-curPos->x-1)*3+8-1;
                drive();
                navstate_.path.pop_front();
            }
        }
        return 0;
    }
    case Q_BUMP_SIG:
    {
        cout<<"nav::bump"<<endl;
        navstate_.onBump();
        return 0;
    }
    case Q_EXIT_SIG:
        cout<<"nav::exit"<<endl;
        last_state=NAV_STATE;
       // navstate_.path.clear();
        navstate_.target.clear();
        break;
    default:
        break;
    }
    return event;
}
Event FoldSchedule::followwallFunc(Event event, StateArgs *param)
{
    switch(event)
    {
    case Q_ENTRY_SIG:
    {
        cout<<"followwall::enter"<<endl;
        current_state=FOLLOWWALL_STATE;
        break;
    }
    case Q_INIT_SIG:
    {
        cout<<"followwall::init"<<endl;
        wall_lost_count_=0;
        Status state=followwallstate_.onInit(param);
        switch(state)
        {
        case success:{
            Tran(&navstate_,nullptr,0);
            return 0;
        }
        default:
            break;
        }
        break;
    }
    case Q_BUMP_SIG:
    {
        cout<<"followwall::bump"<<endl;
        wall_lost_count_=0;
        followwallstate_.bumpHandle(param);
        return 0;
    }
    case Q_LOST_WALL_SIG:
    {
        wall_lost_count_++;
        cout<<"followwall::lost wall"<<endl;
        if(wall_lost_count_>10)//避免执行两次drive
        {
            wall_lost_count_=0;
            cout<<"followwall::wall disapear"<<endl;
             Tran(&searchwallstate_,nullptr,0);
             return 0;
        }
        followwallstate_.followWall();
        return 0;
    }
   case Q_RESEARCH_SIG:
    {
        cout<<"followwall::try to find path"<<endl;
        followwall_count=0;
        Tran(&navstate_,nullptr,0);
        return 0;
    }
    case Q_EXIT_SIG:
        cout<<"followwall::exit"<<endl;
        last_state=FOLLOWWALL_STATE;
        break;
    default:
        break;
    }
    return event;
}
Event FoldSchedule::SearchwallFunc(Event event, StateArgs *param)
{
    switch(event)
    {
    case Q_ENTRY_SIG:
    {
        cout<<"searche wall::enter"<<endl;
        current_state=SEARCH_WALL_SATATE;
        break;
    }
    case Q_INIT_SIG:
    {
       drive();
        return 0;
    }
    case Q_RESEARCH_SIG:
    {
        cout<<"search wall::research"<<endl;
        return 0;
    }
    case Q_BUMP_SIG:
    {
        cout<<"search wall::find the wall"<<endl;
        Tran(&followwallstate_,nullptr,0);
        return 0;
    }
    case Q_LOST_WALL_SIG:
    {
        return 0;
    }
    case Q_EXIT_SIG:
        cout<<"followwall::exit"<<endl;
        last_state=SEARCH_WALL_SATATE;
        break;
    default:
        break;
    }
    return event;
}

//run函数
Status FoldSchedule::run()
{
    int delta_x=body[curPos->theta]->x-curPos->x;
    int delta_y=body[curPos->theta]->y-curPos->y;
    int tilt=abs(delta_x)+abs(delta_y);//运动方向存在倾角状态
    State *state=GetState();
    if(state==&followystate_)
        followy_count++;
    if(state==&followwallstate_)
    {
        followwall_count++;
        if(followwall_count>300)
        {
            cout<<"wall research"<<endl;
            Run(Q_RESEARCH_SIG,0);
        }
    }
    if(stall_count_>10)
    {

        stall_count_=0;
        Run(Q_STALL_SIG,0);
        return stall;
    }
    cout<<"stall count="<<stall_count_<<endl;

   if(state==&navstate_)
    {
        cout<<"run::follow path"<<endl;
        Run(Q_FOLLOW_PATH_SIG,0);
        //return stall;
    }
    if(costmap_[body[curPos->theta]->x+(delta_x)][body[curPos->theta]->y+(delta_y)]==8)//正面碰撞
    {
        cout<<"bump::1"<<endl;
        BumpArgs args=BumpArgs(1);
        Run(Q_BUMP_SIG,&args);
    }
    else if(delta_x==-1&&
            (
            (delta_y==0&&
             (costmap_[body[curPos->theta]->x+(delta_x)][body[curPos->theta]->y+1]==8||
            costmap_[body[curPos->theta]->x+(delta_x)][body[curPos->theta]->y-1]==8))||//判断正向上下
            costmap_[body[curPos->theta]->x+(delta_x)][body[curPos->theta]->y+(delta_y)-delta_y*1]==8||
            costmap_[body[curPos->theta]->x+(delta_x)][body[curPos->theta]->y+(delta_y)-delta_y*2]==8)
            )//左边碰撞
    {
        cout<<"bump::2"<<endl;
            BumpArgs args=BumpArgs(2);
            Run(Q_BUMP_SIG,&args);
    }
    else if(delta_x==1&&
            (
                (delta_y==0&&
                 (costmap_[body[curPos->theta]->x+(delta_x)][body[curPos->theta]->y+1]==8||
                costmap_[body[curPos->theta]->x+(delta_x)][body[curPos->theta]->y-1]==8))||//判断正向上下
                costmap_[body[curPos->theta]->x+(delta_x)][body[curPos->theta]->y+(delta_y)-delta_y*1]==8||
                costmap_[body[curPos->theta]->x+(delta_x)][body[curPos->theta]->y+(delta_y)-delta_y*2]==8)
            )//右边碰撞
    {
        cout<<"bump::4"<<endl;
        BumpArgs args=BumpArgs(4);
        Run(Q_BUMP_SIG,&args);
    }
    else if(delta_y==-1&&
            (
                (delta_x==0&&
                 (costmap_[body[curPos->theta]->x+(delta_x)-1][body[curPos->theta]->y+(delta_y)]==8||
                 costmap_[body[curPos->theta]->x+(delta_x)+1][body[curPos->theta]->y+(delta_y)]==8)
                 )||//判断正向上下
                 costmap_[body[curPos->theta]->x+(delta_x)-delta_x*1][body[curPos->theta]->y+(delta_y)]==8||
                 costmap_[body[curPos->theta]->x+(delta_x)-delta_x*2][body[curPos->theta]->y+(delta_y)]==8))//上面碰撞
    {
        cout<<"bump::5"<<endl;
        BumpArgs args=BumpArgs(5);
        Run(Q_BUMP_SIG,&args);
    }
    else if(delta_y==1&&
            (
                (delta_x==0&&
                 (costmap_[body[curPos->theta]->x+(delta_x)-1][body[curPos->theta]->y+(delta_y)]==8||
                 costmap_[body[curPos->theta]->x+(delta_x)+1][body[curPos->theta]->y+(delta_y)]==8)
                 )||//判断正向上下
                 costmap_[body[curPos->theta]->x+(delta_x)-delta_x*1][body[curPos->theta]->y+(delta_y)]==8||
                 costmap_[body[curPos->theta]->x+(delta_x)-delta_x*2][body[curPos->theta]->y+(delta_y)]==8))//下面碰撞
    {
        cout<<"bump::3"<<endl;
        BumpArgs args=BumpArgs(3);
        Run(Q_BUMP_SIG,&args);
    }
    else if((state==(&followystate_))&&(costmap_[body[curPos->theta]->x+(foldstate_.heading_==7?-2:2)*(delta_y)][body[curPos->theta]->y+((foldstate_.heading_==7)?-2:2)*(delta_x)]!=8
                                      &&costmap_[curPos->x+(foldstate_.heading_==1?-2:2)*(delta_y)][curPos->y+((foldstate_.heading_==1)?-2:2)*(delta_x)]!=8))
    {
            //cout<<"lost wall\n";
            Run(Q_LOST_WALL_SIG,0);
    }
    else if((state==(&followwallstate_))&&(abs(delta_x+delta_y)<2)&&(costmap_[body[curPos->theta]->x-2*(delta_y)][body[curPos->theta]->y+2*(delta_x)]!=8
                                      &&costmap_[curPos->x-2*(delta_y)][curPos->y+2*(delta_x)]!=8))
    {
           //cout<<"lost wall\n";
            Run(Q_LOST_WALL_SIG,0);
    }
    else if((state==(&followwallstate_))&&(delta_x==delta_y)&&(costmap_[curPos->x-2*(delta_y)+delta_x][curPos->y+2*(delta_x)]!=8
                                      ||costmap_[curPos->x-2*(delta_y)][curPos->y+2*(delta_x)]!=8))
    {
         // cout<<"lost wall\n";
            Run(Q_LOST_WALL_SIG,0);
    }
    else if((state==(&followwallstate_))&&(costmap_[curPos->x-2*(delta_y)][curPos->y+2*(delta_x)+delta_y]!=8
                                      ||costmap_[curPos->x-2*(delta_y)][curPos->y+2*(delta_x)]!=8))
    {

            Run(Q_LOST_WALL_SIG,0);
    }

    else if(state!=&navstate_)
    {
     Tran(state,(StateArgs *)0,nullptr);
    }
    return running;
}

