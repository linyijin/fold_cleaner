#include "move.h"
#include<QtGui>

Move::Move()
{
    curPos.x=40;
    curPos.y=40;
}
Move::Move(const int vl,const int va)
{
    linear_v=vl;
    angular_v=va;
    //当前位置信息？
    curPos.x=40;
    curPos.y=40;

  //  emit velUpdate(vl,va);
    std::cout<<"initial a move"<<std::endl;

}
Point Move::getPose() const
{
    return curPos;
}
void Move::posUpdate()
{
    std::cout<<curPos.x<<' '<<curPos.y<<std::endl;
    Point test;
    test.x=curPos.x++;
    test.y=curPos.y;
    emit onDrawPose(test.x,test.y,4);//绘制为4
    //delete test;
}
void Move::setCurPos(const int x, const int y)
{
    curPos.x=x;
    curPos.y=y;
}
