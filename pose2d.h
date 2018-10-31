//存放2D状态结构
#ifndef POSE2D_H
#define POSE2D_H
#include<vector>
struct Point{
    int x;
    int y;
    int theta;
    bool operator=(const Point* s)
    {
        if(x==s->x && y==s->y)
            return true;
        return false;
    }
   // Point();
    //Point(int x_,int y_):x(x_),y(y_),theta(0){}
};
std::vector<Point *> nb8( Point* cur);
#endif // POSE2D_H
