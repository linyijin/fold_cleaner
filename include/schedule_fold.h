#ifndef SCHEDULE_FOLD_H
#define SCHEDULE_FOLD_H
#include "state.h"
#include "hsm.h"
#include "pose2d.h"
#include "state_init.h"
#include "state_move.h"
#include "state_turn.h"
#include "state_followy.h"
#include "state_nav.h"
#include "state_followwall.h"
//fold状态
struct FoldState:State
{
    struct Args:StateArgs
    {
        Args();
        Args(int direction,int heading=1,int last_heading=1):
            direction_(direction),
            heading_(heading),
          last_heading_(last_heading)
            {}
        int direction_;
        int heading_;
        int last_heading_;
    };

    FoldState():State(),
     status_(ready),
    direction_(1),
      heading_(1),//默认朝左
    last_heading_(1)
     {
    }
    FoldState(QHsm *hsm,const char *name,State *parent,QFun fun):State(hsm,name,parent),
      status_(ready),
     direction_(1),
       heading_(1),
        last_heading_(1)
    {
        handler_=fun;
    }
    FoldState(const FoldState& rhs):State(rhs),
        status_(ready),
       direction_(1),
         heading_(1),
          last_heading_(1)
    {
        handler_=rhs.handler_;
    }
    Status status_;
    int direction_;
    int heading_;
    int last_heading_;

};
//nav状态

//init状态
struct InitState:State
{
    InitState():State()
    {}
    InitState(QHsm *hsm,const char *name,State *parent,QFun fun):State(hsm,name,parent)
    {
        handler_=fun;
    }
    InitState(const FoldState &rhs):State(rhs)
    {
        handler_=rhs.handler_;
    }
};
struct MoveState:Move
{
    MoveState(QHsm *hsm,const char *name,State *parent,QFun fun):Move(hsm,name,parent)
    {
        handler_=fun;
    }
    MoveState():Move()
    {}
    MoveState(const MoveState& rhs):Move(rhs)
    {
        handler_=rhs.handler_;
    }
};
struct TurnState:Turn
{
  TurnState(QHsm *hsm,const char *name,State *parent,QFun fun):Turn(hsm,name,parent)
  {
      handler_=fun;
  }
  TurnState():Turn()
  {}
  TurnState(const TurnState& rhs):Turn(rhs)
  {
      handler_=rhs.handler_;
  }
};
struct FollowyState:Followy
{
    FollowyState()
    {}
    FollowyState(QHsm *hsm,const char *name,State *parent,QFun fun):Followy(hsm,name,parent)
    {
        handler_=fun;
    }
};
struct Navstate:NavState
{
    Navstate()
    {}
    Navstate(QHsm *hsm,const char *name,State *parent,QFun fun):NavState(hsm,name,parent)
    {
        handler_=fun;
    }
};
struct Followwall:FollowWall
{
    Followwall(){}
    Followwall(QHsm *hsm,const char *name,State *parent,QFun fun):FollowWall(hsm,name,parent)
    {
        handler_=fun;
    }
};
struct SearchWall:State//用于寻找墙
{
    SearchWall(){}
    SearchWall(QHsm *hsm,const char *name,State *parent,QFun fun):State(hsm,name,parent)
    {
        handler_=fun;
    }
};
class FoldSchedule :public QHsm
{
private:
    static FoldSchedule *p_inst;

public:
    FoldSchedule();
    static FoldSchedule* instance()//实例化
    {
        if(p_inst)
            p_inst=new FoldSchedule();
        return p_inst;
    }
public:
    Event initFunc(Event event,StateArgs *param);
    Event foldFunc(Event event,StateArgs *param);
    Event moveFunc(Event event,StateArgs *param);
    Event turnFunc(Event event,StateArgs *param);
    Event followyFunc(Event event,StateArgs *param);
    Event navFunc(Event event,StateArgs *param);
    Event followwallFunc(Event event,StateArgs *param);
    Event SearchwallFunc(Event event,StateArgs *param);

    void bumpHandle(StateArgs *param);
     void init(void);
     Status run(void);

     InitState initstate_;
     FoldState foldstate_;
     MoveState movestate_;
     TurnState turnstate_;
     FollowyState followystate_;
     Navstate navstate_;
     Followwall followwallstate_;
     SearchWall searchwallstate_;

     Status status_;

     int direction_;
     int heading_;
     bool draw_path;

};
#endif // SCHEDULE_FOLD_H
