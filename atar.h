#ifndef ATAR_H
#define ATAR_H
#include <QThread>
#include <mutex>
#include <QtCore/QTime>
#include <queue>
#include "qobject.h"
#include<vector>
#include<list>

class Astar : public QObject
{

Q_OBJECT
public:
    Astar();
    ~Astar();
    int getPathLen();
signals:
    void onDrawPose(int x,int y,int type);
    void showPath(int len);
    void resetAxis();
    void showState(int type);
public slots:
    void calculate();
    void resetMap();
    void resetMapEnd();
    void  obstacleset(int x,int y);
    void setStart(int start_x,int start_y);
    void setEnd(int end_x,int end_y);
private:
    int path_len=0;

};
#endif // ATAR_H
