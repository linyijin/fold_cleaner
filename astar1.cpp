#include "astar.h"
#include<iostream>
#include<math.h>

void Astar_1::initAstar(std::vector<std::vector<int>> &map)//引用
{
    map_ = map;//将地图给予astar对象
}
int Astar_1::calG(Point *p, Point *start)//计算两点之间的G
{
    int externG= abs(start->x - p->x) + abs(start->y - p->y) == 1 ? lcost : tcost;
    int parentG= start->parent == NULL ? 0 :start->parent->G;//此处逻辑有问题，应该考虑的是当前点的G代价，而不是之前算过的parent
    return externG+parentG;
}
int Astar_1::calH(Point *p, Point *end)
{
    return sqrt((double)(end->x - p->x)*(double)(end->x - p->x) + (double)(end->y - p->y)*(double)(end->y - p->y))*lcost;
    //return p->H;
}
int Astar_1::calF(Point *p)
{
    return p->F = p->G + p->H;
}
bool Astar_1::inMap(Point *p)
{
   // if ((p->x<0) || (p->y<0) || (p->x>79)|| (p->y>79) ||(map_[p->x][p->y]==8)  )
    if ((p->x<0) || (p->y<0) || (p->x>79)|| (p->y>79))
        return false;

}
Point *Astar_1::isInList(Point *p, std::list<Point*> &q)
{
    for (auto iter : q)
    {
        if (iter->x == p->x && iter->y == p->y)
            return iter;
    }
    return NULL;
}
std::vector<Point *> Astar_1::nb8(Point *cur)
{
    std::vector<Point *> nbr;
    nbr.reserve(8);
    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {

            if (i == 0 && j == 0)//跳过当前重合节点
                continue;

            int x = cur->x + i;
            int y = cur->y + j;
            Point *p = new Point(x,y);
            std::cout<<"before push the ner"<<std::endl;
            std::cout<<x<<' '<<y<<std::endl;
            if (inMap(p) && !isInList(p,closelist))//选取在地图中，不在close中的八近邻,open创建了一个新的副本，要注意区别
            {
                std::cout<<"push the ner"<<std::endl;
                nbr.push_back(new Point(p->x, p->y));
            }
            delete p;
        }
    }
    return nbr;
}
Point *Astar_1::getLeastPoint(std::list<Point*> p)
{
    if (!openlist.empty())
    {
        auto min = p.front();
        for (auto &iter : p)//??
        {
            if (iter->F < min->F)
                min = iter;
        }
        return min;
    }
    return NULL;
}
Point * Astar_1::findPath(Point start_, Point end_)
{
    std::cout<<"search for path"<<std::endl;
    openlist.push_back(new Point(start_.x, start_.y));//创建新的start点:参数是两个int数据，需要开辟point空间
    while (!openlist.empty())
    {
        std::cout<<"calculate openlist"<<std::endl;
        auto current = getLeastPoint(openlist);//取出最小F值的open
        openlist.remove(current);
        closelist.push_back(current);

        auto nbr = nb8(current);
        for (auto &iter : nbr)//iter取出的是原来的值？
        {
            if (map_[iter->x][iter->y] == 8)//跳过障碍
                continue;
            if (!isInList(iter,openlist))//不在openlist中
            {

                iter->G = calG(iter, current);
                iter->H = calH(iter, &end_);
                iter->F = calF(iter);
                iter->parent = current;
                //std::cout << "out openlist" << std::endl;
                //std::cout << "x=" << iter->x << "y=" << iter->y << ' ' << iter->G << std::endl;
                openlist.push_back(iter);
            }
            else
            {

                int tempG=calG(iter, current);//从current计算
                Point *temp = isInList(iter, openlist);//应该取出open中保持的值进行比较
                if (tempG < temp->G)//更新G值和F值
                //if (tempG < iter->G)//更新G值和F值
                {
                    iter->G = tempG;
                    iter->F = calF(iter);
                    iter->parent = current;
                }
            }
            Point * resPoint = isInList(&end_, openlist);
            if (resPoint)
                return resPoint;//找到终点，直接返回终点
        }
    }
    return NULL;
}
std::list<Point *> Astar_1::getPath(Point start,Point end)
{
    std::cout<<"start calculate path"<<std::endl;
    std::list<Point*> path;//创建一个链表
    Point * result = findPath(start, end);
    //std::cout<<result->x<<' '<<result->y<<std::endl;
    while (result)//起点的父节点为NULL，可以作为退出循环条件
    {
        path.push_front(result);
        if (result->G == 14)
            sum += 14;
        else
            sum += 10;//判定总里程
        result = result->parent;
    }
    std::cout << "sum=" << sum << std::endl;
    if (!path.empty())
        return path;
}
