#ifndef STATE_FOLLOWY_H
#define STATE_FOLLOWY_H
#include "state.h"
#include "hsm.h"
struct Followy:State
{
    struct Args:StateArgs
    {
        Args(int direction,int heading,int lastHeading):
            direction_(direction),
            heading_(heading),
            last_heading_(lastHeading)
        {}
        int direction_;
        int heading_;
        int last_heading_;
    };
    Followy();
    Followy(QHsm *hsm,const char *name,State *parent);
    ~Followy(){}

    void onEnter(StateArgs *param);
    void onExit(StallArgs *param);
    Status onInit(StateArgs *param);
    void bumpHandle(StateArgs *param);
    bool overClean();

    Status followWall();

    Point *start;
    int starty;


private:
    int direction_;
    int heading_;

    int last_heading_;
};
#endif // STATE_FOLLOWY_H
