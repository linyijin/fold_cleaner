#ifndef STATE_H
#define STATE_H
#include "pose2d.h"
enum//状态机标志信号
{
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
    nav_finish,
    controlling,
    finish,
    failed,
    follow_wall,
    follow_judge,
    lostWall,
    ready

    navigation
}Status;
struct StateArgs
{
    virtual ~StateArgs(){}
};
struct BumpArgs:StateArgs
{
    int bump_type;
};
struct StallArgs:StateArgs
{

};
class QHsm;
typedef int QSignal;
typedef QSignal QEvent;
typedef QEvent (QHsm::*QFun)(QEvent event,StateArgs *param);//难点？？
struct QState
{
    QState();
    QState(QHsm *hsm,QFun fun);
    QState(QHsm *hsm,const char *name,QState *parent);
    QState(const QState &state);
    virtual ~QState(){}

    virtual void onEnter(StateArgs *param){return;}
    virtual void onExit(StallArgs *param){return;}
    virtual void bumpHandle(StateArgs *param){return;}
    virtual void stallTrapped(StateArgs *param){return;}

    virtual Status onInit(StateArgs *param){return running;}
    void setName(const char* name){name_=name;}
    void setHsm(QHsm *hsm){hsm_=hsm;}
    void setParentState(QState *parent){parent_=parent;}

    const char *name_;
    QHsm *hsm_;
    QState *parent_;
    QFun handler_;
    int level_;

    static void initialize();
};
#endif // STATE_H