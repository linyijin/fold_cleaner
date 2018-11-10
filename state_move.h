#ifndef STATE_MOVE_H
#define STATE_MOVE_H
#include "state.h"
#include<vector>
struct Move:State
{
public:
    typedef enum
    _Forward
    {
        move_left=1,
        move_right
    }Forward;
    struct Args:StateArgs
    {
        Args(int direction=0,int heading=1):
        direction_(direction),heading_(heading)
        {}
        int direction_;
        int heading_;
    };
    Move();
    Move(const Move&rhs);
    Move(QHsm *hsm,const char *name,State *parent=0);
    ~Move(){}

    void onEnter(StateArgs *param);
    void onExit(StateArgs *param);
    Status onInit(StateArgs *param);//用来移动
public:
    int direction_;
    int heading_;

    Forward state_;
    Point *startPos_;
    Point *curPos_;
    Point *lastPos_;
    std::vector<Point *> body;

};
#endif // STATE_MOVE_H
