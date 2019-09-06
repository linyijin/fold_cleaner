#include "../../include/controller.h"
extern std::vector<std::vector<int>> costmap_;
extern Point* curPos;
extern Point* lastPos;
extern std::vector<Point *> body;
extern int stall_count_;
void drive()//前进一格
{
    std::vector<Point *> nb_8=nb8(body[curPos->theta]);
    bool pass=true;
    for(auto &iter : nb_8)
    {
        if(costmap_[iter->x][iter->y]==8)
            pass=false;
    }
    if(nb_8.size()==9 && pass)
    {
        stall_count_=0;
        lastPos->x=curPos->x;
        lastPos->y=curPos->y;
        curPos->x=body[curPos->theta]->x;
        curPos->y=body[curPos->theta]->y;
    }
    else
        stall_count_++;
}
