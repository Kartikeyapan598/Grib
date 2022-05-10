#ifndef DATA_H
#define DATA_H

#include <time.h>
class Data
{
public:
    Data();
    void initData ();

protected:
    int xMin, xMax, yMin, yMax;
    int Ni, Nj;
    float Di, Dj;
    time_t refDate;
    int nbForecasts;
};

#endif // DATA_H
