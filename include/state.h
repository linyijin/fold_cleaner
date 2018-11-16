#ifndef STATE_H
#define STATE_H
#include "pose2d.h"
typedef int QSignal;
typedef QSignal Event;
enum//状态机标志信号
{
    Q_EMPTY_SIG=0,
    Q_INIT_SIG,
    Q_ENTRY_SIG,
    Q_EXIT_SIG,
    Q_USER_SIG
};

typedef enum//状态
_Status{
    running=1,
    bump,//直线碰撞
    turn_bump,//转弯碰撞
    turn_finish,
    follow_finish,
    overclean,
    stall,
    overclean_move,//直线运动中发送重复清扫
   // nav_finish,
    controlling,
    finish,
    failed,
    follow_wall,
    follow_judge,
    lostWall,
    ready,
    navigation,
    clean_finish,
    success
}Status;
struct StateArgs
{
    virtual ~StateArgs(){}
};
struct BumpArgs:StateArgs
{
    BumpArgs(int bump_type=0):
        bump_type_(bump_type){}
    int bump_type_;
};
struct StallArgs:StateArgs
{

};
class QHsm;

typedef Event (QHsm::*QFun)(Event event,StateArgs *param);//难点？？
struct State
{
    State();
    State(QHsm *hsm,QFun fun);
    State(QHsm *hsm,const char *name,State *parent);
    State(const State &state);
    virtual ~State(){}

    virtual void onEnter(StateArgs *param){return;}
    virtual void onExit(StallArgs *param){return;}
    virtual void bumpHandle(StateArgs *param){return;}
    virtual void stallTrapped(StateArgs *param){return;}

    virtual Status onInit(StateArgs *param){return running;}
    void setName(const char* name){name_=name;}
    void setHsm(QHsm *hsm){hsm_=hsm;}
    void setParentState(State *parent){parent_=parent;}

    const char *name_;
    QHsm *hsm_;
    State *parent_;
    QFun handler_;
    int level_;

    static void initialize();
};
#endif // STATE_H
