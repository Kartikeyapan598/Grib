#ifndef GRIDDER_H
#define GRIDDER_H

#include "griddata.h"
#include <vector>

//class Plotter;

class Gridder
{
public :

    Gridder(int fc);

    friend class Plotter;

private :

    GridData data = GridData::getGribDataInstance();

    void initGridder (int fc);

    QPointF minPoint, maxPoint;
    int Ni, Nj;
    float Di, Dj;
    time_t refDate;

    std::vector<std::vector<double>>  windSpeed;
    std::vector<std::vector<int>>     windDir;
    std::vector<std::vector<double>>  pressure;
    std::vector<std::vector<double>>  waveSigHeight;
    std::vector<std::vector<double>>  windWaveHeight;
    std::vector<std::vector<int>>     windWaveDir;
    std::vector<std::vector<int>>     windWavePer;
    std::vector<std::vector<double>>  swellWaveHeight;
    std::vector<std::vector<int>>     swellWaveDir;
    std::vector<std::vector<int>>     swellWavePer;
    std::vector<std::vector<double>>  currentSpeed;
    std::vector<std::vector<int>>     currentDir;
};

#endif // GRIDDER_H
