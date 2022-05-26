#include "gridder.h"

Gridder::Gridder(int fc)
{
    initGridder(fc);
}

void Gridder::initGridder (int fc)
{
    Ni = data.getNi();
    Nj = data.getNj();
    Di = data.getD();
    Dj = data.getD();
    minPoint = data.getMinPoint();
    maxPoint = data.getMaxPoint();
    refDate = data.getRefDate();

    windSpeed = data.getWindSpeed(fc);
    windDir = data.getWindDir(fc);
    pressure = data.getPressure(fc);
    waveSigHeight = data.getWaveSigHeight(fc);
    windWaveHeight = data.getWindWaveHeight(fc);
    windWaveDir = data.getWindWaveDir(fc);
    windWavePer = data.getWindWavePer(fc);
    swellWaveHeight = data.getSwellWaveHeight(fc);
    swellWaveDir = data.getSwellWaveDir(fc);
    swellWavePer = data.getSwellWavePer(fc);
    currentSpeed = data.getCurrentSpeed(fc);
    currentDir = data.getCurrentDir(fc);
}
