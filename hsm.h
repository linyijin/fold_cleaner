#ifndef HSM_H
#define HSM_H
#include "state.h"
class QHsm
{
public:
    QHsm();
    QHsm(const char *name,QState *init_state=0);
    virtual ~QHsm(){}
    virtual void setInitState(QState *state);
    virtual int isInState(const QState *state);
    virtual QState *GetState();//获取当前状态
    virtual void Run(QEvent event,StateArgs *param);//发布信号
    virtual void Tran(QState *next_state,StateArgs *param,void(*method)(QHsm *This,StateArgs *param));

    QEvent *func(QEvent event,StateArgs *param){return 0;}

    virtual void run(){}
    virtual void tran(){}

    QState root_;
    int terminate_;
protected:
    QState *cur_state;
    bool hsm_tran_;
    const char* name_;
};
#endif // HSM_H
