#include "atar.h"
#include "astar.h"
#include<iostream>
#include<math.h>
std::vector<std::vector<int>> costmap_(80, std::vector<int>(80, 0));//定义一个接收地图的函数
Point start(0,0);
Point end(0,0);//用全局定义
Astar::Astar(){
}
Astar::~Astar(){

}
<<<<<<< HEAD
void Astar::setStart(int start_x, int start_y)
{
    start.x=start_x;
    start.y=start_y;
    resetMap();
    costmap_[start_x][start_y]=2;
    emit onDrawPose(start_x,start_y,2);//绘制起始点

}
void Astar::setEnd(int end_x,int end_y)
{
    end.x=end_x;
    end.y=end_y;
    resetMapEnd();
    costmap_[end_x][end_y]=4;
    emit onDrawPose(end_x,end_y,4);
}
void Astar::calculate()//相当于main函数
{
=======
void Astar::setStart(int start_x, int start_y, int end_x, int end_y)
{
    start.x=start_x;
    start.y=start_y;
    end.x=end_x;
    end.y=end_y;
}
void Astar::calculate()//相当于main函数
{
>>>>>>> 38aae4596c79052150bb85134eab3c654af7ab02
    Astar_1 astar_1;
    astar_1.initAstar(costmap_);
    std::list<Point *>path_=astar_1.getPath(start, end);
    if(path_.empty())
    {
      //  std::cout<<"path is empty"<<std::endl;
        emit showState(0);
        resetMap();
    }
    path_len=path_.size();
    emit showPath(path_len);//发送路径长度
    for (auto &p : path_)
            costmap_[p->x][p->y] = 1;
    costmap_[start.x][start.y]=2;
<<<<<<< HEAD
    costmap_[end.x][end.y]=4;
=======
    costmap_[end.x][end.y]=2;
>>>>>>> 38aae4596c79052150bb85134eab3c654af7ab02
    for(int i=0;i<80;i++){
        for(int j=0;j<80;j++)
        {
             emit onDrawPose(i,j,costmap_[i][j]);//给mainwidow绘制
        }
    }

}
void Astar::obstacleset(int x, int y)
{
    costmap_[x][y]=8;
}
void Astar::resetMap()
{
    for(int i=0;i<80;i++){
        for(int j=0;j<80;j++)
        {
<<<<<<< HEAD
            costmap_[i][j]=0;
             emit onDrawPose(i,j,costmap_[i][j]);//给mainwidow绘制
        }
    }
    emit showPath(0);
    emit resetAxis();
}
void Astar::resetMapEnd()
{
    for(int i=0;i<80;i++){
        for(int j=0;j<80;j++)
        {
            if(costmap_[i][j]==2)
=======
            if(costmap_[i][j]==8)
>>>>>>> 38aae4596c79052150bb85134eab3c654af7ab02
                continue;
            costmap_[i][j]=0;
             emit onDrawPose(i,j,costmap_[i][j]);//给mainwidow绘制
        }
    }
}
<<<<<<< HEAD
int Astar::getPathLen()
{
    return path_len;
}
=======
//算法成员函数
>>>>>>> 38aae4596c79052150bb85134eab3c654af7ab02


