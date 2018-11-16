#include "state_init.h"
Init::Init()
{}
Init::Init(const Init&rhs):State(rhs)
{
    handler_=rhs.handler_;
}
Init::Init(QHsm *hsm,const char*name,State *parent,QFun fun):State(hsm,name,parent)
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
