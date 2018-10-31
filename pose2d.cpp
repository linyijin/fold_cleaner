#include "pose2d.h"
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
                    nbr8.push_back(nbr);//存入数组
                }
            }
        }
        return nbr8;
}
