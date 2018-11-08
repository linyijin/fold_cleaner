#ifndef SCHEDULE_FOLD_H
#define SCHEDULE_FOLD_H
#include "state.h"
#include "hsm.h"
#include "pose2d.h"
#include "state_init.h"
#include "state_move.h"
//fold状态
struct FoldState:QState
{
    struct Args:StateArgs
    {
        Args();
        Args(int direction,int heading=1):
            direction_(direction),
            heading_(heading)
            {}
        int direction_;
        int heading_;
    };

    FoldState():QState(),
     status_(ready),
    direction_(1),
      heading_(1)//默认朝左
     {
    }
    FoldState(QHsm *hsm,const char *name,QState *parent,QFun fun):QState(hsm,name,parent),
      status_(ready),
     direction_(1),
       heading_(1)
    {
        handler_=fun;
    }
    FoldState(const FoldState& rhs):QState(rhs),
        status_(ready),
       direction_(1),
         heading_(1)
    {
        handler_=rhs.handler_;
    }
    Status status_;
    int direction_;
    int heading_;
};
//init状态
struct InitState:QState
{
    InitState():QState()
    {}
    InitState(QHsm *hsm,const char *name,QState *parent,QFun fun):QState(hsm,name,parent),
    {
        handler_=fun;
    }
    InitState(const FoldState &rhs):QState(rhs)
    {
        handler_=rhs.handler_;
    }
};
struct MoveState:Move
{
    MoveState(QHsm *hsm,const char *name,QState *parent,QFun fun):Move(hsm,name,parent,fun)
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
class FoldSchedule :public QHsm
{
private:
    static FoldSchedule *p_inst;
    FoldSchedule();
public:
    static FoldSchedule* instance()//实例化
    {
        if(p_inst)
            p_inst=new FoldSchedule();
        return p_inst;
    }
public:
    QEvent initFunc(QEvent event,StateArgs *param);
    QEvent foldFunc(QEvent event,StateArgs *param);
    QEvent moveFunc(QEvent event,StateArgs *param);
    void bumpHandle(StateArgs *param);


     void run(void);
     void init(void);

     InitState initstate_;
     FoldState foldstate_;
     MoveState movestate_;
     Status status_;

     int direction_;
     int heading_;
     int stall_count_;
}
#endif // SCHEDULE_FOLD_H
