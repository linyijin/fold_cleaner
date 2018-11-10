#ifndef STATE_TURN_H
#define STATE_TURN_H
#include "state.h"
struct Turn:State
{
public:
    struct Args:StateArgs
    {
        Args(int direction=0,int heading=1,int bump_tyoe=0):direction_(direction),heading_(heading)
        {}
        int direction_;//弓字方向
        int heading_;//机器朝向
        //int bump_type;
    };
    Turn();
    Turn(QHsm *hsm,QFun fun);
    Turn(QHsm *hsm,const char *name,State *parent);
    Turn(const Turn &state);
    ~Turn(){}

     void onEnter(StateArgs *param);
     void onExit(StallArgs *param);
     void bumpHandle(StateArgs *param);
     void stallTrapped(StateArgs *param);
     Status onInit(StateArgs *param);
private:
     int heading_;
     int direction_;
};
#endif // STATE_TURN_H
