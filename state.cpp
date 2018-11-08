#include "state.h"
#include "hsm.h"
QState::QState():
    hsm_(0),
    handler_(0),
    name_(0),
    level_(0),
    parent_(0)
{
}
QState::QState(QHsm *hsm,QFun fun):
    hsm_(hsm),
    handler_(fun),
    name_(".Root"),
    level_(0),
    parent_((QState *)0)
{
}
QState::QState(QHsm *hsm,const char *name,QState *parent):
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
QState::QState(const QState &state)
{
    hsm_=state.hsm_;
    handler_=state.handler_;
    parent_=state.parent_;
    level_=state.level_;
}
void QState::initialize()
{}
