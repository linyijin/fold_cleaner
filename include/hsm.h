#ifndef HSM_H
#define HSM_H
#include "state.h"
class QHsm
{
public:
    QHsm();
    QHsm(const char *name,State *init_state=0);
    virtual ~QHsm(){}
    virtual void setInitState(State *state);
    virtual int isInState(const State *state);
    virtual State *GetState();//获取当前状态
    virtual void Run(Event event,StateArgs *param);//发布信号
    virtual void Tran(State *next_state,StateArgs *param,void (*method)(QHsm *This, StateArgs *param));

    Event *func(Event event,StateArgs *param){return 0;}

    virtual Status run(){}
    virtual void tran(){}

    State root_;
    int terminate_;
protected:
    State *cur_state;
    bool hsm_tran_;
    const char* name_;
};
#endif // HSM_H
