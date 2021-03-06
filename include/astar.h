#ifndef ASTAR_H
#define ASTAR_H
#include "pose2d.h"
#include<iostream>
#include<vector>
#include<list>

const int lcost = 10;
const int tcost = 14;

class Astar_1{//定义算法用的对象
public:
    int sum = 0;//记录总里程
    void initAstar(std::vector<std::vector<int>> &map);
    std::list<Point *> getPath(Point start, Point end);
    std::vector<Point *> searchFild;
private:
    int calG(Point *p,Point *start);
    int calF(Point *p);
    int calH(Point *p, Point *end);
    std::vector<Point *> nb_8(Point *cur);//八近邻，放着八个指针
    std::vector<Point *> simple_nb(Point *cur);
    std::list<Point*> openlist;
    std::list<Point*> closelist;
    Point* isInList(Point *p, std::list<Point*> &q);
    bool inMap(Point *p);//判断是否在地图内
    std::vector<std::vector<int>> map_;//地图数据
    Point *getLeastPoint(std::list<Point*> p);
    Point * findPath(Point start, Point end);

};
#endif // ASTAR_H
