#include "../../include/planner.h"
#include "time.h"
#include<iostream>
#include<math.h>
//extern std::vector<std::vector<int>> costmap_(80, std::vector<int>(80, 0));//定义一个接收地图的函数
extern std::vector<std::vector<int>> costmap_;
Point start(0,0);
Point end(0,0);//用全局定义
Astar::Astar(){
}
Astar::Astar(std::vector<std::vector<int>> &costmap)//将主窗口地图送来,送地址
{
    costmap_=costmap;
}
Astar::~Astar(){
std::cout<<"destroy astar"<<std::endl;
}
void Astar::setStart(int start_x, int start_y)
{
    start.x=start_x;
    start.y=start_y;
    //resetMap();
    costmap_[start_x][start_y]=2;
    emit onDrawPose(start_x,start_y,2);//绘制起始点

}
void Astar::setEnd(int end_x,int end_y)
{
    end.x=end_x;
    end.y=end_y;
    //resetMapEnd();
    costmap_[end_x][end_y]=2;
    emit onDrawPose(end_x,end_y,2);
}
void Astar::calculate()//相当于main函数
{
    clock_t tiemStart=clock();//计时开始
    Astar_1 astar_1;
    astar_1.initAstar(costmap_);
    std::list<Point *>path_=astar_1.getPath(start, end);
    if(path_.empty())
    {
        std::cout<<"path is empty"<<std::endl;
        emit showState(0);
      //  resetMap();
    }
    clock_t timeFinish=clock();
    double duration=(double)(timeFinish-tiemStart)/CLOCKS_PER_SEC;
    cout<<"during time is"<<duration<<endl;
    int sum_=astar_1.sum;
    //std::cout<<"success"<<path_.size()<<std::endl;
    path_len=path_.size();
    for(auto &iter : path_)//把点直接放入数组
    {
        path.push_back(iter);
    }
    emit showSum(path_len,sum_);
    emit showPath(path_len);//发送路径长度
    Point *last=path_.front();//记录上一个点
    for (auto &p : path_)
    {
      emit onDrawPath(last->x,last->y,p->x,p->y,1);//修改规划路径颜色
        last=p;
     //costmap_[p->x][p->y] = 1;
    }
    int searchCount=0;
    for(auto &p:astar_1.searchFild)//绘制搜索区域
    {
        costmap_[p->x][p->y]=4;
    }
    costmap_[start.x][start.y]=2;
    costmap_[end.x][end.y]=4;
    for(int i=0;i<80;i++){
        for(int j=0;j<80;j++)
        {
            if(costmap_[i][j]==4)
                searchCount++;
             emit onDrawPose(i,j,costmap_[i][j]);//给mainwidow绘制
        }
    }
    cout<<"search Filed is"<<searchCount<<endl;

}
void Astar::obstacleset(int x, int y)
{
    costmap_[x][y]=8;
}
void Astar::resetMap()
{
   // cout<<"reset map"<<endl;
    for(int i=0;i<80;i++){
        for(int j=0;j<80;j++)
        {
            if(i==0 || j==0||i==79 ||j==79)
                costmap_[i][j]=8;
            else
                costmap_[i][j]=0;
             emit onDrawPose(i,j,costmap_[i][j]);//给mainwidow绘制

        }
    }
    emit showPath(0);
    emit resetAxis();
}
void Astar::reClean()
{
   // cout<<"reset map"<<endl;
    for(int i=0;i<80;i++){
        for(int j=0;j<80;j++)
        {
            if(i==0 || j==0||i==79 ||j==79)
                costmap_[i][j]=8;
            else if(costmap_[i][j]!=8)
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
            if(costmap_[i][j]==2 || costmap_[i][j]==8 || costmap_[i][j]==4)
                continue;
            costmap_[i][j]=0;
             emit onDrawPose(i,j,costmap_[i][j]);//给mainwidow绘制
        }
    }
}
int Astar::getPathLen()
{
    return path_len;
}
std::list<Point *> Astar::returnPath()
{
    return path;
}


