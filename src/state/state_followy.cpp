#include "../../include/state_followy.h"
#include "../../include/controller.h"

#include<vector>
extern Point *curPos;
extern Point *lastPos;
extern std::vector<Point *> body;
extern std::vector<std::vector<int>> costmap_;
Followy::Followy()
{
}
Followy::Followy(QHsm *hsm, const char *name, State *parent):State(hsm,name,parent)
{
}
void Followy::onEnter(StateArgs *param)
{
    starty=curPos->y;
    start=curPos;

    if((void *)0==param)
        return;
    Args *args=(Args*)param;
    direction_=args->direction_;
    heading_=args->heading_;
    last_heading_=args->last_heading_;

}
void Followy::onExit(StallArgs *param)
{
}
Status Followy::onInit(StateArgs *param)
{
    int delta_x=abs(curPos->x-body[curPos->theta]->x);
    int delta_y=abs(curPos->y-body[curPos->theta]->y);
    if(overClean())
        return overclean;
    if(abs(curPos->y-starty)<2)//位移超过2
    {
        drive();
        return running;
    }
    else
    {
        cout<<"followy finish"<<endl;
        return finish;
    }
}
Status Followy::followWall()
{
    if(last_heading_==7&& direction_==0)//右边有墙
    {
        switch(curPos->theta)
        {
            case 0:
                curPos->theta=3;
            break;
        case 3:
            curPos->theta=6;
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
        case 5:
            curPos->theta=2;
            break;
        case 2:
            curPos->theta=1;
            break;
        case 1:
            curPos->theta=0;
            break;
        }
    }
    else if(last_heading_==7&&direction_==1)
    {
        switch (curPos->theta) {
        case 0:
            curPos->theta=1;
            break;
        case 1:
            curPos->theta=2;
            break;
        case 2:
            curPos->theta=5;
            break;
        case 5:
            curPos->theta=8;
            break;
        case 8:
            curPos->theta=7;
            break;
        case 7:
            curPos->theta=6;
            break;
        case 6:
            curPos->theta=3;
            break;
        case 3:
            curPos->theta=0;
            break;
        }
    }
    else if(last_heading_==1 && direction_==0)
    {
        switch (curPos->theta) {
        case 0:
            curPos->theta=1;
            break;
        case 1:
            curPos->theta=2;
            break;
        case 2:
            curPos->theta=5;
            break;
        case 5:
            curPos->theta=8;
            break;
        case 8:
            curPos->theta=7;
            break;
        case 7:
            curPos->theta=6;
            break;
        case 6:
            curPos->theta=3;
            break;
        case 3:
            curPos->theta=0;
            break;
        }
    }
    if(last_heading_==1&& direction_==1)//右边有墙
    {
        switch(curPos->theta)
        {
            case 0:
                curPos->theta=3;
            break;
        case 3:
            curPos->theta=6;
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
        case 5:
            curPos->theta=2;
            break;
        case 2:
            curPos->theta=1;
            break;
        case 1:
            curPos->theta=0;
            break;
        }
    }
   drive();
   return running;
}
void Followy::bumpHandle(StateArgs *param)
{
    BumpArgs *args=(BumpArgs *)param;
    switch(args->bump_type_)
    {
        case 1:
        {
            switch(curPos->theta)
            {
            case 0:
            {
                if(last_heading_==1)
                    curPos->theta=3;
                else
                    curPos->theta=1;
            }
            break;
            case 3:
            {
                if(last_heading_==1)
                    curPos->theta=6;
                else
                    curPos->theta=0;
            }
                break;
            case 6:
            {
                if(last_heading_==1)
                    curPos->theta=7;
                else
                    curPos->theta=3;
            }
                break;
            case 7:
            {
                if(last_heading_==1)
                    curPos->theta=8;
                else
                    curPos->theta=6;
            }
                break;
            case 8:
            {
                if(last_heading_==1)
                    curPos->theta=5;
                else
                    curPos->theta=7;
            }
                break;
            case 5:
            {
                if(last_heading_==1)
                    curPos->theta=2;
                else
                    curPos->theta=6;
            }
                break;
            case 2:
            {
                if(last_heading_==1)
                    curPos->theta=1;
                else
                    curPos->theta=5;
            }
                break;
            case 1:
            {
                if(last_heading_==1)
                    curPos->theta=0;
                else
                    curPos->theta=2;
            }
                break;
            }
            break;
        }
    case 2:
    {
        switch(curPos->theta)
        {
            case 0:
            curPos->theta=3;
            break;
        case 1:
            curPos->theta=3;
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
            curPos->theta=1;
            break;
        case 5:
        {
            if(last_heading_==1)
                curPos->theta=1;
            else
                curPos->theta=7;
            break;
        }
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
            curPos->theta=3;
            break;
        case 8:
            curPos->theta=5;
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
        {
            if(last_heading_==1)
                curPos->theta=6;
            else curPos->theta=0;
            break;
        }
        case 6:
            curPos->theta=7;
            break;
        }
        break;
    }
    }
    drive();
}
bool Followy::overClean()
{
    std::vector<Point *> nbr24=nb24(curPos);
    int no_imformation_size=0;
    for(auto &iter : nbr24)
    {
        if(costmap_[iter->x][iter->y]==0)
            no_imformation_size++;
    }
    if(no_imformation_size<1)
        return true;
    else
        return false;
}
