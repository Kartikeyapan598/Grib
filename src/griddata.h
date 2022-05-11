#ifndef GRIDDATA_H
#define GRIDDATA_H

#include "data.h"
#include <vector>
#include <unordered_map>
using namespace std;

class GridData : public Data
{

public:

    GridData() = delete;
    GridData(const char* fname);
    ~GridData();

    static GridData& getGribDataInstance() { return *m_gridDataInstance; }

    const vector<double>& getWindSpeed(int fc) { return windSpeed[fc]; }
    const vector<int>& getWindDir(int fc) { return windDir[fc]; }
    const vector<double>& getPressure(int fc) { return pressure[fc]; }
    const vector<double>& getWaveSigHeight(int fc) { return waveSigHeight[fc]; }
    const vector<double>& getWindWaveHeight(int fc) { return windWaveHeight[fc]; }
    const vector<int>& getWindWaveDir(int fc) { return windWaveDir[fc]; }
    const vector<int>& getWindWavePer(int fc) { return windWavePer[fc]; }
    const vector<double>& getSwellHeight(int fc) { return swellWaveHeight[fc]; }
    const vector<int>& getSwellDir(int fc) { return swellWaveDir[fc]; }
    const vector<int>& getSwellPer(int fc) { return swellWavePer[fc]; }
    const vector<double>& getCurrentSpeed(int fc) { return currentSpeed[fc]; }
    const vector<int>& getCurrentDir(int fc) { return currentDir[fc]; }

private:
    static GridData* m_gridDataInstance;

    int     decode(const char* fname);

    int     decode(int* fd);
    void    decodeWindSpeed(int fc, int* fd);
    void    decodeWindDir(int fc, int* fd);
    void    decodePressure(int fc, int* fd);
    void    decodeWaveSigHeight(int fc, int* fd);
    void    decodeWindWaveHeight(int fc, int* fd);
    void    decodeWindWaveDir(int fc, int* fd);
    void    decodeWindWavePeriod(int fc, int* fd);
    void    decodeSwellWaveHeight(int fc, int* fd);
    void    decodeSwellWaveDir(int fc, int* fd);
    void    decodeSwellWavePeriod(int fc, int* fd);
    void    decodeCurrentSpeed(int fc, int* fd);
    void    decodeCurrentDir(int fc, int* fd);

    unordered_map<int, vector<double>>  windSpeed;
    unordered_map<int, vector<int>>     windDir;
    unordered_map<int, vector<double>>  pressure;
    unordered_map<int, vector<double>>  waveSigHeight;
    unordered_map<int, vector<double>>  windWaveHeight;
    unordered_map<int, vector<int>>     windWaveDir;
    unordered_map<int, vector<int>>     windWavePer;
    unordered_map<int, vector<double>>  swellWaveHeight;
    unordered_map<int, vector<int>>     swellWaveDir;
    unordered_map<int, vector<int>>     swellWavePer;
    unordered_map<int, vector<double>>  currentSpeed;
    unordered_map<int, vector<int>>     currentDir;

    void clearMap(unordered_map<int, vector<double>> param);
    void clearMap(unordered_map<int, vector<int>> param);
};

#endif // GRIDDATA_H
