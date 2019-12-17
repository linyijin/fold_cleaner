#ifndef ATAR_H
#define ATAR_H
#include <QThread>
#include <mutex>
#include <QtCore/QTime>
#include <queue>
#include "qobject.h"
#include "astar.h"
#include<vector>
#include<list>

class Astar : public QObject
{

Q_OBJECT
public:
    Astar();
    Astar(std::vector<std::vector<int>> &costmap);
    ~Astar();
    int getPathLen();
    std::list<Point *> returnPath();
signals:
    void onDrawPose(int x,int y,int type);
    void onDrawPath(int x1,int y1,int x2,int y2,int type);
    void showPath(int len);
    void resetAxis();
    void showState(int type);
    void showSum(int len,int sum);
public slots:
    void calculate();
    void resetMap();
    void reClean();
    //void resetMapStart();
    void resetMapEnd();
    void  obstacleset(int x,int y);
    void setStart(int start_x,int start_y);
    void setEnd(int end_x,int end_y);
private:
    int path_len=0;
    std::list<Point *> path;
   // std::vector<std::vector<int>> costmap_;

};
#endif // ATAR_H
