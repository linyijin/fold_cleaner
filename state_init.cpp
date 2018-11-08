#include "state_init.h"
Init::Init()
{}
Init::Init(const Init&rhs):QState(rhs)
{
    handler_=rhs.handler_;
}
Init::Init(QHsm &hsm,const char*name,QState *parent,QFun fun):QState(hsm,name,parent)
{
    handler_=fun;
}
void Init::onEnter(StateArgs *param)
{
}
void Init::onExit(StateArgs *param)
{
}
Status Init::onInit(StateArgs *param)
{
    return running;
}
