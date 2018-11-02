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
    if ((p->x<1) || (p->y<1) || (p->x>79)|| (p->y>79))
        return false;
    return true;

}
Point *Astar_1::isInList(Point *p, std::list<Point*> &q)
{
    if(q.empty())
        return NULL;
    for (auto iter : q)
    {
        if (iter->x == p->x && iter->y == p->y)
            return iter;
    }
    return NULL;
}
std::vector<Point *> Astar_1::nb_8(Point *cur)//找下一个点
{
    std::vector<Point *> nbr;
    nbr.reserve(8);

    int x = cur->x ;
    int y = cur->y ;
    int right=2;
    int left=-1;
    int top=-1;
    int down=2;
    if(map_[x+1][y]==8)//旁边有障碍物直接将整行抛弃，无法穿行
       right=1;
    if(map_[x-1][y]==8)
        left=0;
    if(map_[x][y+1]==8)
        down=1;
    if(map_[x][y-1]==8)
        top=0;
    for (int i = left; i < right; i++)
    {
        for (int j = top; j < down; j++)
        {

            if (i == 0 && j == 0)//跳过当前重合节点
                continue;
//            if(abs(i)+abs(j)>1)//变为四近邻
  //              continue;
            int nx = cur->x+i ;
            int ny = cur->y+j ;
            Point *p = new Point(nx,ny);//p只承载x,y
            if (inMap(p) && !isInList(p,closelist) && map_[p->x][p->y]!=8)//选取在地图中，不在close中的八近邻,open创建了一个新的副本，要注意区别
            {
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
   // cout<<start_.x<<' '<<start_.y<<endl;
    //cout<<end_.x<<' '<<end_.y<<endl;
    openlist.push_back(new Point(start_.x, start_.y));//创建新的start点:参数是两个int数据，需要开辟point空间
    while (!openlist.empty())
    {
        //std::cout<<openlist.size()<<std::endl;
        auto current = getLeastPoint(openlist);//取出最小F值的open
        //cout<<current->x<<' '<<current->y<<endl;
        openlist.remove(current);
        closelist.push_back(current);
        auto nbr = nb_8(current);
        if(nbr.empty())
        {
            continue;//没有八近邻，不计算
        }
       // std::cout<<"find nb8"<<std::endl;
        for (auto &iter : nbr)//iter取出的是原来的值？
        {
            if (map_[iter->x][iter->y] == 8)//跳过障碍
                continue;
            bool passable=false;
            auto nbr_nb=simple_nb(iter);//判断是否是个可以通过的点
            for(auto &iter_nbr : nbr_nb)
            {
                if(map_[iter_nbr->x][iter_nbr->y]==8)
                    passable=true;
            }
          // if(nbr_nb.size()==9)
            //   passable=true;
            if(passable)//八近邻有障碍，不能通过
            {
               // std::cout<<"not a passable point"<<std::endl;
                continue;
            }
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
            Point * resPoint = isInList(&end_, openlist);//判断是否在队列中
            if (resPoint)
                return resPoint;//找到终点，直接返回终点
        }
    }
    std::cout<<"failed"<<std::endl;
    throw 1;//抛出异常
    return NULL;
}
std::list<Point *> Astar_1::getPath(Point start,Point end)
{
    std::list<Point*> path;//创建一个链表
    Point *result;
    try
    {
        result = findPath(start, end);
    }
    catch(int i)
    {
         std::cout<<"error ocur"<<std::endl;//检验异常是否抛出
         return path;//直接将path返回
    }
    while (result)//起点的父节点为NULL，可以作为退出循环条件
    {
        path.push_front(result);
        if (result->G == 14)
            sum += 14;
        else
            sum += 10;//判定总里程
        result = result->parent;
    }
    if (!path.empty())
        return path;
}
std::vector<Point *> Astar_1::simple_nb(Point *cur)//简单地收入八近邻，包括自身
{
            std::vector<Point *> nbr8;
            for(int i=-1;i<2;i++)
            {
                for(int j=-1;j<2;j++)
                {
                    if(i==0 && j==0)
                        nbr8.push_back(cur);//当前点不再建立空间
                    else
                    {
                        Point *nbr=new Point(cur->x+i,cur->y+j);
                        nbr8.push_back(nbr);//存入数组
                    }
                }
            }
            return nbr8;
}
