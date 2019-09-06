#include "../../include/state.h"
#include "../../include/hsm.h"
State::State():
    hsm_(0),
    handler_(0),
    name_(0),
    level_(0),
    parent_(0)
{
}
State::State(QHsm *hsm,QFun fun):
    hsm_(hsm),
    handler_(fun),
    name_(".Root"),
    level_(0),
    parent_((State *)0)
{
}
State::State(QHsm *hsm,const char *name,State *parent):
    hsm_(hsm),
    name_(name),
    parent_(parent)
{
    if((void *)0==parent)
    {
        parent=&(hsm->root_);
    }
    level_=parent->level_+1;

}
State::State(const State &state)
{
    hsm_=state.hsm_;
    handler_=state.handler_;
    parent_=state.parent_;
    level_=state.level_;
}
void State::initialize()
{}
