#ifndef STATE_INIT_H
#define STATE_INIT_H
#include "state.h"
struct Init:State
{
public:
    Init();
    Init(const Init&rhs);
    Init(QHsm *hsm,const char*name,State *parent,QFun fun);
    ~Init(){}

    void onEnter(StateArgs *param);
    void onExit(StateArgs *param);
    Status onInit(StateArgs *param);
private:
};
#endif // STATE_INIT_H
