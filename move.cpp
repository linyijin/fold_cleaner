#include "move.h"
#include "planner.h"

#include "search.h"
#include<QtGui>
Point *curPos=new Point;
Point *lastPos=new Point;
std::vector<Point *> body;//机体信息
FoldSchedule *fold_schedule=new FoldSchedule();
extern  std::vector<std::vector<int>> costmap_;
Move_state::Move_state()
{
    curPos->x=40;
    curPos->y=40;
    startPos->x=curPos->x;
    startPos->y=curPos->y;
    curPos->theta=Direction::left;
    body=nb8(curPos);//绘制机体
    lastPos->x=curPos->x;
    lastPos->y=curPos->y;
    direction=0;
    fold_schedule->init();
}
Move_state::Move_state(const int vl,const int va)
{
    linear_v=vl;
    angular_v=va;
    //当前位置信息？
    curPos->x=40;
    curPos->y=40;
    curPos->theta=Direction::right;
    body=nb8(curPos);
    lastPos->x=curPos->x;
    lastPos->y=curPos->y;
    lastPos->theta=curPos->theta;
    direction=0;
}
Move_state::Move_state(std::vector<std::vector<int>> &map)
{
    curPos->x=40;
    curPos->y=40;
    curPos->theta=Direction::right;
    body=nb8(curPos);
    lastPos->x=curPos->x;
    lastPos->y=curPos->y;
    lastPos->theta=curPos->theta;
    direction=0;
    //map_=map;
}
Point *Move_state::getPose() const
{
    return curPos;
}
void Move_state::posUpdate()//根据curpos绘制机体,更新机体
{
    //cout<<"pos update"<<endl;

    std::vector<Point *> lastBody=nb8(lastPos);//建立上一个body
    for(auto &iter :lastBody)
    {
        costmap_[iter->x][iter->y]=4;
        emit onDrawPose(iter->x,iter->y,4);
    }
   // cout<<"pos update"<<endl;
    body=nb8(curPos);
    for(auto &iter : body)
        {
            if(costmap_[iter->x][iter->y]==2)//起点，终点不绘制
                continue;
            if(iter==curPos)
            {
                costmap_[curPos->x][curPos->y]=3;
                emit onDrawPose(curPos->x,curPos->y,3);//中心点画为橙色
            }
            else if(iter->x==body[curPos->theta]->x && iter->y==body[curPos->theta]->y)
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
}
void Move_state::setCurPos(const int x, const int y)
{
    curPos->x=x;
    curPos->y=y;
    startPos->x=x;
    startPos->y=y;
}
void Move_state::fold()
{
    Status state=fold_schedule->run();
    if(fold_schedule->draw_path)
    {
        fold_schedule->draw_path=false;
        path=fold_schedule->navstate_.path;
        path.push_front(curPos);
    //绘制路径
    Point *last=path.front();//记录上一个点
    for (auto &p : path)
    {
      emit onDrawPath(last->x,last->y,p->x,p->y,1);
        last=p;
    }
    }
}
void Move_state::fold_start()
{
    fold_schedule->init();
}
/*
std::vector<Point *> Move::angle(Point* cur,int direc,int head)//以0位置的左边三角为基准三角:上：head=1,左：direc=1
{
   // cout<<"start to create an angle"<<endl;
    std::vector<Point *> nbr6;
    for(int i=0;i<3;i++)
    {
       for(int j=0;j<3-i;j++)//逐渐递减
       {

           int x=body[cur->theta]->x+direc*i;
           int y=body[cur->theta]->y+head*j;
           //cout<<"x,y="<<x<<' '<<y<<endl;
           if(x<0 || x>79 || y<0 || y>79)//越界判断
               continue;
           Point *nbr=new Point;
           nbr->x=x;
           nbr->y=y;
           nbr6.push_back(nbr);
       }
    }
   // cout<<"create an angle"<<endl;
    return nbr6;
}

//弓字函数
bool Move::overClean()
{
    std::vector<Point *> nbr_24=nb24(curPos);
    int no_imformation=0;
    for(auto &iter : nbr_24)
    {
        if(costmap_[iter->x][iter->y]==0)
            no_imformation++;
    }
    if(no_imformation>=1)
        return false;
    else
        return true;

}
state Move::searchWall()//顺时针搜索
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
        //cout<<"move by search"<<endl;
        lastPos->x=curPos->x;
        lastPos->y=curPos->y;
        curPos->x=body[curPos->theta]->x;
        curPos->y=body[curPos->theta]->y;
    }

    switch(curPos->theta)
    {
        case top_left:
            curPos->theta=top;
        break;
    case top:
        curPos->theta=top_right;
        break;
    case top_right:
        curPos->theta=right;
        break;
    case right:
        curPos->theta=buttom_right;
        break;
    case buttom_right:
        curPos->theta=buttom;
        break;
    case buttom:
        curPos->theta=buttom_left;
        break;
    case buttom_left:
        curPos->theta=left;
        break;
    case left:
        curPos->theta=top_left;
        break;
    }

    stallCount++;

}
state Move::fold_run()
{
    int delta_x=body[curPos->theta]->x-curPos->x;
    int delta_y=body[curPos->theta]->y-curPos->y;
    timeCount++;
    if(timeCount>=100)//监视，10秒更新一次卡住检查位置
    {
        timeCount=0;
        stallPos->x=curPos->x;
        stallPos->y=curPos->y;
    }
    if(curPos->x==lastPos->x && curPos->y==lastPos->y || (abs(curPos->x-stallPos->x)<2&&abs(curPos->y-stallPos->y)<2))//困住判断
        stallCount++;
    else
        stallCount=0;
    cout<<"count="<<stallCount<<endl;
    if(stallCount>50)
    {
        stallPos->x=curPos->x;
        stallPos->y=curPos->y;
        stallCount=0;
        return stall;
    }
    if(followwall)//状态机判断,永远右边先判断，左转优先
    {
      //  cout<<"start to follow wall"<<endl;
        underWall=false;

        lastPos->theta=curPos->theta;//用于判断是否发送变化
        switch(curPos->theta)
        {
        case top_left:
        {
            cout<<"top left"<<endl;
            std::vector<Point *> left_nbr=angle(curPos,-1,1);
            std::vector<Point *> right_nbr=angle(curPos,1,-1);
            for(auto &iter :left_nbr)
            {
                if(costmap_[iter->x][iter->y]==8)
                 {

                   curPos->theta=buttom_left;
                   break;
                 }

            }

            for(auto &iter : right_nbr)
            {
                if(costmap_[iter->x][iter->y]==8)
                {
                    underWall=true;
                    curPos->theta=left;
                    break;
                }
            }


            break;
        }
        case top:
        {
             cout<<"top"<<endl;
            if(costmap_[curPos->x][curPos->y-2]==8 ||costmap_[curPos->x+1][curPos->y-2]==8 )
                curPos->theta=top_left;

            else if(costmap_[curPos->x-1][curPos->y-2]==8)
                curPos->theta=left;
            if(costmap_[curPos->x+2][curPos->y-1]==8 ||costmap_[curPos->x+2][curPos->y]==8 ||costmap_[curPos->x+2][curPos->y+1]==8 )
                underWall=true;
            break;
        }
        case top_right:
        {
             cout<<"top right"<<endl;
            std::vector<Point *> left_nbr=angle(curPos,-1,-1);
            std::vector<Point *> right_nbr=angle(curPos,1,1);
            for(auto &iter :left_nbr)
            {
                if(costmap_[iter->x][iter->y]==8)
                 {
                  underWall=true;
                  curPos->theta=top_left;
                   break;
                 }

            }

            for(auto &iter : right_nbr)
            {
                if(costmap_[iter->x][iter->y]==8)
                {
                    curPos->theta=top;
                    break;
                }
            }


            break;
        }
        case left:
        {
             cout<<"left"<<endl;
            if(costmap_[curPos->x-2][curPos->y]==8 ||costmap_[curPos->x-2][curPos->y-1]==8 )
                curPos->theta=buttom_left;

            else if(costmap_[curPos->x-2][curPos->y+1]==8)
                curPos->theta=buttom;
            if(costmap_[curPos->x][curPos->y-2]==8 ||costmap_[curPos->x+1][curPos->y-2]==8 ||costmap_[curPos->x-1][curPos->y-2]==8)
                underWall=true;
            break;
        }
        case right:
        {
             cout<<"right"<<endl;
            if(costmap_[curPos->x+2][curPos->y]==8 ||costmap_[curPos->x+2][curPos->y+1]==8 )
                curPos->theta=top_right;

            else if(costmap_[curPos->x+2][curPos->y-1]==8)
                curPos->theta=top;
            if(costmap_[curPos->x][curPos->y+2]==8 ||costmap_[curPos->x+1][curPos->y+2]==8 ||costmap_[curPos->x-1][curPos->y+2]==8)
                underWall=true;
            break;
        }
        case buttom_left:
        {
             cout<<"buttom left"<<endl;
            std::vector<Point *> left_nbr=angle(curPos,1,1);
            std::vector<Point *> right_nbr=angle(curPos,-1,-1);
            for(auto &iter :left_nbr)
            {
                if(costmap_[iter->x][iter->y]==8)
                 {
                   curPos->theta=buttom_right;
                   break;
                 }

            }

            for(auto &iter : right_nbr)
            {
                if(costmap_[iter->x][iter->y]==8)
                {
                    underWall=true;
                    curPos->theta=buttom;
                    break;
                }
            }
            break;
        }
        case buttom:
        {
             cout<<"buttom"<<endl;
            if(costmap_[curPos->x][curPos->y+2]==8 ||costmap_[curPos->x-1][curPos->y+2]==8 )
                curPos->theta=buttom_right;

            else if(costmap_[curPos->x+1][curPos->y+2]==8)
                curPos->theta=right;
            if(costmap_[curPos->x-2][curPos->y-1]==8 ||costmap_[curPos->x-2][curPos->y]==8 ||costmap_[curPos->x-2][curPos->y+1]==8)
                underWall=true;

            break;
        }
        case buttom_right:
        {
             cout<<"buttom right"<<endl;
            std::vector<Point *> left_nbr=angle(curPos,1,-1);
            std::vector<Point *> right_nbr=angle(curPos,-1,1);
            for(auto &iter :left_nbr)
            {
                if(costmap_[iter->x][iter->y]==8)
                 {
                   curPos->theta=right;
                   break;q
                 }

            }

            for(auto &iter : right_nbr)
            {
                if(costmap_[iter->x][iter->y]==8)
                {
                    underWall=true;
                    curPos->theta=right;
                    break;
                }
            }
            break;
        }
        }
        if(stallCount>9)
        {
            stallCount=0;
            return follow_wall;
        }
        if(!underWall)
            return lostWall;
       if(curPos->theta==lastPos->theta)
       {
            return follow_wall;
       }
       else
       {
          // cout<<"theta is change"<<endl;
           return follow_judge;
       }

    }

    if(pathOver)//路径规划完成
        return nav_finish;
    if(navControl)
    {
        return nav_finish;
    }
    //直线碰撞判断
    if(delta_x!=0 && delta_y==0)//x轴上直线位移
    {
        if(inFollow)//沿墙放在直线运动中判断
        {
            if(overClean())//在沿墙时判断重复清扫
            {
                return overclean;
            }
            if(costmap_[body[curPos->theta]->x][body[curPos->theta]->y+(direction?2:-2)]!=8 &&costmap_[curPos->x][curPos->y+(direction?2:-2)]!=8)
            {
                inFollow=false;
                return follow_finish;
            }
        }

        if(costmap_[body[curPos->theta]->x+delta_x*1][body[curPos->theta]->y]==8 )
        {
            emit showState(2);
            bump_type=3;//中间碰撞
            return bump;
        }
        else if(costmap_[body[curPos->theta]->x+delta_x*1][body[curPos->theta]->y+1]==8)
        {
            emit showState(2);
            bump_type=2;//下方发生碰撞
            return bump;
        }
        else if(costmap_[body[curPos->theta]->x+delta_x*1][body[curPos->theta]->y-1]==8)
        {
            emit showState(2);
            bump_type=1;//上方发生碰撞
            return bump;
        }
        else
        {
            int move_no_imformation=0;
            for(int i=1;i<4;i++)//在转弯之后判断直线运动中是否重复清扫，防止越界
            {
                if(body[curPos->theta]->x+delta_x*i>79)//越界判断
                    continue;
                for(int j=-1;j<2;j++)
                {
                    if(costmap_[body[curPos->theta]->x+delta_x*i][body[curPos->theta]->y+j]==0)
                        move_no_imformation++;
                }
            }
            if(move_no_imformation==0)
                return overclean;
             return running;
        }

    }
    //转弯碰撞判断
    if(delta_y!=0 && delta_x==0 )//y轴上直线运动
    {

        if(costmap_[body[curPos->theta]->x][body[curPos->theta]->y+delta_y*1]==8 ||
                ((costmap_[body[curPos->theta]->x-1][body[curPos->theta]->y+delta_y*1]==8)&&
                 (costmap_[body[curPos->theta]->x+1][body[curPos->theta]->y+delta_y*1]==8)))
            {
            emit showState(2);
                bump_type=6;
                return turn_bump;
            }
        else if(costmap_[body[curPos->theta]->x-1][body[curPos->theta]->y+delta_y*1]==8)
            {
            emit showState(2);
                bump_type=4;//左边碰撞
                return turn_bump;
            }
        else if(costmap_[body[curPos->theta]->x+1][body[curPos->theta]->y+delta_y*1]==8)
        {
            emit showState(2);
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
     emit showState(1);
     int delta_x=body[curPos->theta]->x-curPos->x;
     lastPos->x=curPos->x;
     lastPos->y=curPos->y;
     lastPos->theta=curPos->theta;
     curPos->x=curPos->x+delta_x*1;

}
void Move::fold_turn()//转弯
{
    emit showState(3);
    int delta_x=body[curPos->theta]->x-curPos->x;
    if(delta_x>0)
    {
        lastPos->theta=Direction::right;//记录上一个姿态
        lastPos->y=curPos->y;
        lastPos->x=lastPos->x;
        curPos->theta=direction?Direction::buttom:Direction::top;
        curPos->y=curPos->y;//完成第一步前进


    }
    else if(delta_x<0)
    {
        lastPos->theta=Direction::left;
        lastPos->y=curPos->y;
        lastPos->x=lastPos->x;
        curPos->theta=direction?Direction::buttom:Direction::top;
        curPos->y=curPos->y;//完成第一步前进
        //body=nb8(curPos);
    }

}
void Move::fold_turnBack()//完成转弯
{
    emit showState(2);
    if(lastPos->theta==Direction::right)
    {
        lastPos->theta=curPos->theta;
        lastPos->x=curPos->x;
        lastPos->y=curPos->y;
        curPos->theta=Direction::left;
        curPos->y=curPos->y+(direction?1:-1);

    }
    else if(lastPos->theta==Direction::left)
    {

        lastPos->theta=curPos->theta;
        lastPos->x=curPos->x;
        lastPos->y=curPos->y;
        curPos->theta=Direction::right;
        curPos->y=curPos->y+(direction?1:-1);

    }
}
state Move::fold_follow()
{
    bumpHandle(curPos->theta==Direction::right?4:5);
}
state Move::fold_nav(Point *target)//路径规划,不进行运动
{
    Astar *move_astar=new Astar;
    move_astar->setStart(curPos->x,curPos->y);
    emit onDrawPose(curPos->x,curPos->y,2);
    move_astar->setEnd(target->x,target->y);
    emit onDrawPose(target->x,target->y,2);
    move_astar->calculate();
    path=move_astar->returnPath();//直接定义为对象的数组
    if(path.empty())//没有路径可以到达
    {
        return failed;
    }
    Point *last=path.front();//记录上一个点
    for (auto &p : path)
    {
      emit onDrawPath(last->x,last->y,p->x,p->y,1);
        last=p;
    }
    return nav_finish;
}
state Move::nav_control()
{
    Point *next;
    if(path.size()>=2)
    {
        lastPos->x=curPos->x;
        lastPos->y=curPos->y;
          curPos=path.front();
          path.pop_front();
          next=path.front();
          curPos->theta=(next->y-curPos->y)+(next->x-curPos->x-1)*3+8-1;//delta_y+(delta_x-1)*3+8
          return controlling;
    }
    lastPos->x=curPos->x;
    lastPos->y=curPos->y;
    curPos->x=path.front()->x;
    curPos->y=path.front()->y;
    cout<<"path follow"<<curPos->x<<' '<<curPos->y<<endl;
    return finish;
}
state Move::fold_followWall()
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
        lastPos->x=curPos->x;
        lastPos->y=curPos->y;
        curPos->x=body[curPos->theta]->x;
        curPos->y=body[curPos->theta]->y;//跟随theta前进
    }
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
            curPos->y=curPos->y+(direction?1:-1);//左边四十五度转弯
            //body=nb8(curPos);
            break;
        }
    case 5://右边有障碍
        {
            lastPos->x=curPos->x;
            lastPos->y=curPos->y;
            curPos->x=curPos->x-1;
            curPos->y=curPos->y+(direction?1:-1);//右边四十五度转弯
            //body=nb8(curPos);
            break;
        }
    case 6://下边有障碍
        {
            lastPos->x=curPos->x;
            lastPos->y=curPos->y;
            if(lastPos->theta==Direction::right)
                curPos->theta=Direction::left;
            else if(lastPos->theta=Direction::left)
                curPos->theta=Direction::right;
            //body=nb8(curPos);
            break;
        }


    }
}
int Move::judgeHeading()
{
    int leftSize=0;
    int rightSize=0;
   // cout<<"judege heading"<<endl;
    for(int i=1;i<10;i++)
    {

        if(curPos->x+i>=79 || curPos->x-i<=0)
            continue;
        if(costmap_[curPos->x+i][curPos->y]==0)
            rightSize++;
        else if(costmap_[curPos->x-i][curPos->y]==0)
            leftSize++;
    }
    if(rightSize>=leftSize)
        curPos->theta=right;
    else
        curPos->theta=left;
}
int Move::judgeDirection()
{
    int topSize=0;
    int buttomSize=0;
    for(int i=1;i<10;i++)
    {
        if(curPos->y+i>=79 || curPos->y-i<=0)//越界
            continue;
        if(costmap_[curPos->x][curPos->y+i]==0)
            buttomSize++;
        else if(costmap_[curPos->x][curPos->y-i]==0)
            topSize++;
    }
    if(topSize>buttomSize)
        direction=0;
    else
        direction=1;
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
       break;
    }
    case turn_bump:
    {
        bumpHandle(bump_type);
        if(bump_type==6)
            inFollow=true;
        //fold_nav();
        break;
    }
    case follow_finish:
        {
            fold_follow();
            break;
        }
    case overclean:
    {
        emit showState(5);

        Searcher *searcher=new Searcher(curPos);
        std::vector<Point *> target;
        try
        {
            target=searcher->SearcherFrom(curPos);
        }
        catch(int i)
        {
            cout<<"can't find target"<<endl;
            //emit stop();
            emit showState(8);
            followwall=true;
            break;
        }
        Point *targetPoint=target.front();
        emit onDrawPose(targetPoint->x,targetPoint->y,2);
        state state_=fold_nav(targetPoint);
        if(state_==nav_finish)
            pathOver=true;
        else if(state_==failed)
        {
            costmap_[targetPoint->x][targetPoint->y]=8;//直接变成障碍点
            onDrawPose(targetPoint->x,targetPoint->y,8);
           // emit stop();
            inFollow=true;


        }

        break;
    }
    case overclean_move:
    {
        emit showState(6);
        Searcher *searcher=new Searcher(curPos);
        std::vector<Point *> target=searcher->SearcherFrom(curPos);
        Point *targetPoint=target.front();
        emit onDrawPose(targetPoint->x,targetPoint->y,3);
        break;
    }
    case nav_finish:
    {
        navControl=true;//进入导航控制模式
        emit showState(7);
        inFollow=false;
        pathOver=false;
        state state_=nav_control();
        if(state_==finish)
        {
          //  cout<<"nav is over"<<endl;
//            cout<<curPos->x<<' '<<curPos->y<<endl;
            navControl=false;
            judgeDirection();

            judgeHeading();

        }
        break;
    }
    case finish:
    {
        cout<<"ready to move"<<endl;
        break;
    }
    case follow_wall:
    {
        fold_followWall();
        break;
    }
    case follow_judge:
    {
        break;
    }
    case lostWall:
    {
        searchWall();
        break;
    }
    case stall:
    {
        state state_=fold_nav(startPos);
        if(state_==nav_finish)
            pathOver=true;
        break;
    }
    default:
    {
        break;
    }
    }
    return;

}
*/
