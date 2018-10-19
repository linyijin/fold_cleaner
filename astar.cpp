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
void Astar::setStart(int start_x, int start_y, int end_x, int end_y)
{
    start.x=start_x;
    start.y=start_y;
    end.x=end_x;
    end.y=end_y;
}
void Astar::calculate()//相当于main函数
{
    Astar_1 astar_1;
    astar_1.initAstar(costmap_);
    std::list<Point *>path_=astar_1.getPath(start, end);
    for (auto &p : path_)
            costmap_[p->x][p->y] = 1;
    costmap_[start.x][start.y]=2;
    costmap_[end.x][end.y]=2;
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
            if(costmap_[i][j]==8)
                continue;
            costmap_[i][j]=0;
             emit onDrawPose(i,j,costmap_[i][j]);//给mainwidow绘制
        }
    }
}
//算法成员函数


