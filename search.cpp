#include "search.h"
#include<iostream>
Searcher::Searcher()
{}
Searcher::Searcher(Point *curPos_) :curPos(curPos_)//传递当前节点参数
{
}
Searcher::~Searcher()
{
    for(auto &iter : potentialTarget)//访问标记清空
        iter->visited=0;
    //delete[] potentialTarget;//销毁队列
}
bool Searcher::isTarget( Point *Target)
{
    std::vector<Point *> nbr8=nb8(Target);
    int freeSize=0;
    for(auto &iter : nbr8)
    {
        if(costmap_[iter->x][iter->y]!=8)
            freeSize++;
    }
    if(freeSize==9)
        return true;
    else
        return false;
}
bool Searcher::isInQueue(Point *p, std::list<Point *> q)
{
    for(auto &iter :q)
    {
        if(iter->x==p->x && iter->y==p->y)
            return true;
    }
    return false;
}
std::vector<Point *> Searcher::SearcherFrom(Point *cur)
{
    std::list<Point *> bfs;
    cur->visited=1;
    bfs.push_back(cur);
    Point *current;
    while(!bfs.empty()&& potentialTarget.empty())//有目标点不再搜索
    {
       // std::cout<<bfs.size()<<std::endl;
        current=bfs.front();
        bfs.remove(current);
        close.push_back(current);//保存在close中
        std::vector<Point *> nb_8=nb8(current);
        for(auto &iter : nb_8)
        {
            if(costmap_[iter->x][iter->y]!=8 &&iter->visited==0 && !isInQueue(iter,bfs) && !isInQueue(iter,close))//可通行未访问点，进入层级遍历队列
            {
                iter->visited=1;//置位已经访问过
                bfs.push_back(iter);
               if(costmap_[iter->x][iter->y]==0 && isTarget(iter))//空白，可容纳一个机身点，变为待定目标点
                   potentialTarget.push_back(iter);
            }
        }

    }
    if(potentialTarget.empty())
        throw 1;
   return potentialTarget;//返回队列
}
