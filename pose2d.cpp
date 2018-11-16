#include "pose2d.h"

bool insideMap(const Point *p)
{
    if ((p->x<1) || (p->y<1) || (p->x>79)|| (p->y>79))
        return false;
    return true;
}

std::vector<Point *> nb8( Point* cur)
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
                    Point *nbr=new Point;
                    nbr->x=cur->x+i;
                    nbr->y=cur->y+j;
                    if(!insideMap(nbr))//严禁越界
                    {
                        delete nbr;
                        continue;
                    }
                    nbr8.push_back(nbr);//存入数组
                }
            }
        }
        return nbr8;
}
std::vector<Point *> nb4(Point *cur)//四近邻
{
    std::vector<Point *> nbr4;
    for(int i=-1;i<2;i++)
    {
        for(int j=-1;j<2;j++)
        {
            if((abs(i)+abs(j))<1 && abs(i+j)!=0)//不收入当前点，
            {
                Point *nbr=new Point;
                nbr->x=cur->x+i;
                nbr->y=cur->y+j;
                if(!insideMap(nbr))
                {
                    delete nbr;
                    continue;
                }
                nbr4.push_back(nbr);
            }
        }
    }
    return nbr4;
}
std::vector<Point *> nb24(Point* cur)
{
    std::vector<Point *> nbr24;
    for(int i=-2;i<3;i++)
    {
        for(int j=-2;j<3;j++)
        {
            if(i==0 && j==0)//自身
            {
                nbr24.push_back(cur);//不再创建新的内存空间
                continue;
            }
            Point *nbr=new Point;
            nbr->x=cur->x+i;
            nbr->y=cur->y+j;
            if(!insideMap(nbr))
            {
                delete nbr;
                continue;
            }
            else
                nbr24.push_back(nbr);
        }
    }
    return nbr24;
}

