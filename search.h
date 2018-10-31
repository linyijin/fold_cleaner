#ifndef SEARCH_H
#define SEARCH_H
#include<vector>
#include "pose2d.h"
extern std::vector<std::vector<int>> costmap_;//取地图

class Searcher
{
public:
    Searcher();
    Searcher(const Point *curPos);
    ~Searcher();
    Point *target();//返回目标点
private:
    Point *curPos;//当前点
};
#endif // SEARCH_H
