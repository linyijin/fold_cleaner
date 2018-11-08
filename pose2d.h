//存放2D状态结构
#ifndef POSE2D_H
#define POSE2D_H
#include<vector>
#include<cmath>
#include<iostream>
using std::cout;
using std::endl;

struct Point{
    int x;
    int y;
    int theta;

    bool visited;//用于bfs搜索目标点时减少不必要的搜索
    int F, G, H;
    Point *parent;
    Point(){}
    Point (int _x, int _y) : x(_x), y(_y), F(0), G(0), H(0), parent(NULL) {}//初始化
    bool operator=(const Point* s)
    {
        if(x==s->x && y==s->y)
            return true;
        return false;
    }
   // Point();
    //Point(int x_,int y_):x(x_),y(y_),theta(0){}
};
bool insideMap(const Point *p);//判断是否未超出地图
std::vector<Point *> nb8( Point* cur);
std::vector<Point *> nb4(Point* cur);//四近邻
std::vector<Point *> nb24(Point* cur);//创建24近邻数组，出界不考虑


#endif // POSE2D_H
