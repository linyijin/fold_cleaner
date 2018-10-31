#include "move.h"
#include "planner.h"
#include<QtGui>
extern  std::vector<std::vector<int>> costmap_;
Move::Move()
{
    curPos->x=40;
    curPos->y=40;
    curPos->theta=left;
    body=nb8(curPos);//绘制机体
    lastPos->x=curPos->x;
    lastPos->y=curPos->y;
}
Move::Move(const int vl,const int va)
{
    linear_v=vl;
    angular_v=va;
    //当前位置信息？
    curPos->x=40;
    curPos->y=40;
    curPos->theta=right;
    body=nb8(curPos);
    lastPos->x=curPos->x;
    lastPos->y=curPos->y;
    lastPos->theta=curPos->theta;

}
Move::Move(std::vector<std::vector<int>> &map)
{
    curPos->x=40;
    curPos->y=40;
    curPos->theta=right;
    body=nb8(curPos);
    lastPos->x=curPos->x;
    lastPos->y=curPos->y;
    lastPos->theta=curPos->theta;
    //map_=map;
}
Point *Move::getPose() const
{
    return curPos;
}
void Move::posUpdate()//根据curpos绘制机体,更新机体
{
    //std::cout<<curPos.x<<' '<<curPos.y<<std::endl;
    std::vector<Point *> lastBody=nb8(lastPos);//建立上一个body
    for(auto &iter :lastBody)
    {
        costmap_[iter->x][iter->y]=4;
        emit onDrawPose(iter->x,iter->y,4);
    }
    body=nb8(curPos);
    for(auto &iter : body)
        {
            if(iter==curPos)
            {
                costmap_[curPos->x][curPos->y]=3;
                emit onDrawPose(curPos->x,curPos->y,3);//中心点画为橙色
            }
            else if(iter==body[curPos->theta])
            {
                costmap_[iter->x][iter->y]=3;
                emit onDrawPose(iter->x,iter->y,3);//朝向绘制为橙色
            }
            else
            {
                costmap_[iter->x][iter->y]=1;
                emit onDrawPose(iter->x,iter->y,1);//机体其他部分绘制为蓝色
            }
        }
    //lastPos->x=curPos->x;
    //lastPos->y=curPos->y;
    //lastPos->theta=curPos->theta;
    //curPos->x++;
    //delete test;
}
void Move::setCurPos(const int x, const int y)
{
    curPos->x=x;
    curPos->y=y;
}

//弓字函数
state Move::fold_run()
{
    int delta_x=body[curPos->theta]->x-curPos->x;
    int delta_y=body[curPos->theta]->y-curPos->y;
    //直线碰撞判断
    if(delta_x!=0 && delta_y==0)//x轴上直线位移
    {
        if(costmap_[body[curPos->theta]->x+delta_x*1][body[curPos->theta]->y]==8 )
        {
            bump_type=3;//中间碰撞
            return bump;
        }
        else if(costmap_[body[curPos->theta]->x+delta_x*1][body[curPos->theta]->y+1]==8)
        {
            bump_type=2;//下方发生碰撞
            return bump;
        }
        else if(costmap_[body[curPos->theta]->x+delta_x*1][body[curPos->theta]->y-1]==8)
        {
            bump_type=1;//上方发生碰撞
            return bump;
        }
        else
            return running;
    }
    //转弯碰撞判断
    if(delta_y!=0 && delta_x==0)//y轴上直线运动
    {
        if(costmap_[body[curPos->theta]->x][body[curPos->theta]->y+delta_y*1]==8 ||
                ((costmap_[body[curPos->theta]->x-1][body[curPos->theta]->y+delta_y*1]==8)&&
                 (costmap_[body[curPos->theta]->x+1][body[curPos->theta]->y+delta_y*1]==8)))
            {
                bump_type=6;
                return turn_bump;
            }
        else if(costmap_[body[curPos->theta]->x-1][body[curPos->theta]->y+delta_y*1]==8)
            {
                bump_type=4;//左边碰撞
                return turn_bump;
            }
        else if(costmap_[body[curPos->theta]->x+1][body[curPos->theta]->y+delta_y*1]==8)
        {
            bump_type=5;//右边碰撞
            return turn_bump;
        }

        else
        {
            return turn_finish;
        }
    }


}

void Move::fold_move()
{
     int delta_x=body[curPos->theta]->x-curPos->x;
     lastPos->x=curPos->x;
     lastPos->y=curPos->y;
     lastPos->theta=curPos->theta;
     curPos->x=curPos->x+delta_x*1;
     body=nb8(curPos);//生成新的机体
}
void Move::fold_turn()//转弯
{
    int delta_x=body[curPos->theta]->x-curPos->x;
    if(delta_x>0)
    {
        lastPos->theta=right;//记录上一个姿态
        lastPos->y=curPos->y;
        lastPos->x=lastPos->x;
        curPos->theta=buttom;
        curPos->y=curPos->y;//完成第一步前进
        body=nb8(curPos);

    }
    else if(delta_x<0)
    {
        lastPos->theta=left;
        lastPos->y=curPos->y;
        lastPos->x=lastPos->x;
        curPos->theta=buttom;
        curPos->y=curPos->y;//完成第一步前进
        body=nb8(curPos);
    }

}
void Move::fold_turnBack()//完成转弯
{
    //std::cout<<lastPos->theta<<std::endl;
    if(lastPos->theta==right)
    {
        lastPos->theta=curPos->theta;
        lastPos->x=curPos->x;
        lastPos->y=curPos->y;
        curPos->theta=left;
        curPos->y=curPos->y+1;
        body=nb8(curPos);
    }
    else if(lastPos->theta==left)
    {
        //lastTheta=curPos->theta;
        lastPos->theta=curPos->theta;
        lastPos->x=curPos->x;
        lastPos->y=curPos->y;
        curPos->theta=right;
        curPos->y=curPos->y+1;
        body=nb8(curPos);
    }
}
state Move::fold_follow()
{

}
state Move::fold_nav()//路径规划
{

}

void Move::bumpHandle(const int bump_type)
{
    switch(bump_type)
    {
        case 4://左边有障碍
        {
            lastPos->x=curPos->x;//方向先不更新
            lastPos->y=curPos->y;
            curPos->x=curPos->x+1;
            curPos->y=curPos->y+1;//左边四十五度转弯
            body=nb8(curPos);
            break;
        }
    case 5://右边有障碍
        {
            lastPos->x=curPos->x;
            lastPos->y=curPos->y;
            curPos->x=curPos->x-1;
            curPos->y=curPos->y+1;//右边四十五度转弯
            body=nb8(curPos);
            break;
        }
    case 6://下边有障碍
        {
            lastPos->x=curPos->x;
            lastPos->y=curPos->y;
            if(lastPos->theta==right)
                curPos->theta=left;
            else if(lastPos->theta=left)
                curPos->theta=right;
            body=nb8(curPos);
            break;
        }


    }
}
void Move::fold()
{
    state state_;
    state_=fold_run();
    switch(state_)
    {
    case running:
        fold_move();
        break;
    case bump:
        fold_turn();
        break;
    case turn_finish:
    {
       fold_turnBack();
    }
    case turn_bump:
    {
        bumpHandle(bump_type);
        fold_follow();//转入follow模式
        //fold_nav();
        break;
    }
     default:
        break;
    }
}
