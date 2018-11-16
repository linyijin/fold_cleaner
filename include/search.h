#ifndef SEARCH_H
#define SEARCH_H
#include<vector>
#include "pose2d.h"
#include<list>
extern std::vector<std::vector<int>> costmap_;//取地图

class Searcher
{
public:
    Searcher();
    Searcher(Point *curPos_);
    ~Searcher();
    Point *getTarget() const;//从队列中返回表头目标点,不允许外界变动
    std::vector<Point *>SearcherFrom(Point *cur);
    bool isTarget(Point *Target);//目标点周围必须有至少3个直线点的空间
    std::vector<Point *> buildLine(const Point *Target);//目标点设定方向上无障碍物阻挡点均不再搜索
   // int judgeHeading(const Point *Target);//根据左右未清扫点决定方向
  //  int judgeDirection(const Point *Target);//根据上下无信息点决定弓字方向
    void clearSearch();//路径规划结束，销毁本次搜索的点，下次要搜索
    bool isInQueue(Point *p,std::list<Point *> q);


private:
    Point *curPos;//当前点
    std::vector<Point *> potentialTarget;//备选target点，不一定可达
    std::list<Point *> close;//保存不再搜索的点
};
#endif // SEARCH_H
