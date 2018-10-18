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

signals:
    void onDrawPose(int x,int y,int type);
public slots:
    void calculate();
    void  obstacleset(int x,int y);
};
#endif // ATAR_H
