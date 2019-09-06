#include "../../include/state_followwall.h"
#include "../../include/controller.h"
#include<iostream>
#include<vector>
extern Point *curPos;//全局记录变量
extern Point *lastPos;
extern std::vector<Point *> body;
extern std::vector<std::vector<int>> costmap_;
void FollowWall::onEnter(StateArgs *param)
{}
void FollowWall::onExit(StateArgs *param)
{}
Status FollowWall::onInit(StateArgs *param)
{
    if(!overClean())
    {
        std::cout<<"followwall::find empty space"<<std::endl;
        return success;
    }
    drive();
    return running;
}
void FollowWall::bumpHandle(StateArgs *param)
{
    BumpArgs *args=(BumpArgs *)param;
    switch(args->bump_type_)
    {
        case 1:
        {
            switch(curPos->theta)
            {
                case 0:
                curPos->theta=1;
                break;
            case 3:
                curPos->theta=0;
                break;
            case 6:
                curPos->theta=3;
                break;
            case 7:
                curPos->theta=6;
                break;
            case 8:
                curPos->theta=7;
                break;
            case 5:
                curPos->theta=8;
                break;
            case 2:
                curPos->theta=5;
                break;
            case 1:
                curPos->theta=2;
                break;
            }
            break;
        }
    case 2:
    {
        switch(curPos->theta)
        {
            case 0:
            curPos->theta=5;
            break;
        case 1:
            curPos->theta=2;
            break;
        case 2:
            curPos->theta=5;
            break;
        }
        break;
    }
    case 3:
    {
        switch(curPos->theta)
        {
            case 2:
            curPos->theta=7;
            break;
        case 5:
            curPos->theta=8;
            break;
        case 8:
            curPos->theta=7;
            break;
        }
        break;
    }
    case 4:
    {
        switch(curPos->theta)
        {
            case 6:
            curPos->theta=3;
            break;
        case 7:
            curPos->theta=6;
            break;
        case 8:
            curPos->theta=3;
            break;
        }
        break;
    }
    case 5:
    {
        switch(curPos->theta)
        {
            case 0:
            curPos->theta=1;
            break;
        case 3:
            curPos->theta=0;
            break;
        case 6:
            curPos->theta=1;
            break;
        }
        break;
    }
    }
    drive();
}
Status FollowWall::followWall()//寻找右墙
{

   switch(curPos->theta)
   {
    case 0:
        curPos->theta=3;
       break;
   case 1:
       curPos->theta=0;
       break;
   case 2:
       curPos->theta=1;
       break;
   case 3:
       curPos->theta=6;
       break;
   case 5:
       curPos->theta=2;
       break;
   case 6:
       curPos->theta=7;
       break;
   case 7:
       curPos->theta=8;
       break;
   case 8:
       curPos->theta=5;
       break;
   }
     drive();
    return running;
}
bool FollowWall::overClean()
{
    std::vector<Point *> nbr24=nb24(curPos);
    int no_imformation_size=0;
    for(auto &iter : nbr24)
    {
        if(costmap_[iter->x][iter->y]==0)
            no_imformation_size++;
    }
    if(no_imformation_size<8)
        return true;
    else
        return false;
}
