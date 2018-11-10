#ifndef STATE_FOLLOWY_H
#define STATE_FOLLOWY_H
#include "state.h"
#include "hsm.h"
struct Followy:State
{
    struct Args:StateArgs
    {
        Args(int direction=0,int heading=1):
            direction_(direction),
            heading_(heading),
        {}
        int direction_;
        int heading_;
    };
    Followy();
    Followy(QHsm *hsm,const char *name,State *parent);
    ~Followy(){}

    void onEnter(StateArgs *param);
    void onExit(StallArgs *param);
    Status onInit(StateArgs *param);
private:
    int direction_;
    int heading_;
    int starty;
};
#endif // STATE_FOLLOWY_H
