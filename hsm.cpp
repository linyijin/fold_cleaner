#include "hsm.h"
QHsm::QHsm():
  cur_state(0),
  name_(0),
  terminate_(0),
  hsm_tran_(0)
{
};
QHsm::QHsm(const char *name,QState *init_state)
    :name_(name),
      //root_(init_state),
      terminate_(0),
      hsm_tran_(0),
      cur_state(0)
{
    cur_state=init_state;
}
void QHsm::setInitState(QState *state)
{
    if(state)
        cur_state=state;
    (this->*(cur_state->handler_))(Q_ENTRY_SIG,(StateArgs *)0);//执行当前状态的handler
}
int QHsm::isInState(const QState *state)
{
    QState *temp;
    for(temp=cur_state;temp;temp=temp->parent_)
    {
        if(state==temp)
        {
            return 1;
        }
    }
    return 0;
}
QState* QHsm::GetState()
{
    return cur_state;
}
void QHsm::Run(QEvent event, StateArgs *param)
{
    QState *state=cur_state;
    while(event)
    {
        event=(this->*(state->handler_))(event,param);//状态的handler接收event处理并返回一个event
        state=state->parent_;//处理结果呈报给父节点继续处理
    }
}
void QHsm::Tran(QState *next_state, StateArgs *param, void (*method)(QHsm *, StateArgs *))
{
    if(hsm_tran_)
        return;
    hsm_tran_=1;//置位转换标志
    QState *list_exit[5];
    QState *list_entry[5];
    int cnt_exit=0;
    int cnt_entry=0;
    int idx;
    QState *src=cur_state;
    QState *dst=next_state;
    while(src->level_!=dst->level_)
    {
        if(src->level_>dst->level_)
        {
            list_exit[cnt_exit++]=src;
            src=src->parent_;
        }
        else
        {
            list_entry[cnt_entry++]=dst;
            dst=dst->parent_;
        }
    }
    while(src!=dst)
    {
        list_exit[cnt_exit++]=src;
        list_entry[cnt_entry++]=dst;
        src=src->parent_;
        dst=dst->parent_;
    }
    for(idx=0;idx<cnt_exit;idx++)
    {
        src=list_exit[idx];
        (this->*(src->handler_))(Q_EXIT_SIG,param);
    }
    if(method)
    {
        method(this,param);
    }
    for(idx=0;idx<cnt_entry;idx++)
    {
        dst=list_entry[idx];
        (this->*(dst->handler_))(Q_ENTRY_SIG,param);
    }
    cur_state=next_state;
    hsm_tran_=0;
    (this->*(cur_state->handler_))(Q_INIT_SIG,param);
}
