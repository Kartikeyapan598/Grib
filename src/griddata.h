#ifndef GRIDDATA_H
#define GRIDDATA_H

#include "data.h"
#include "qpoint.h"

using namespace std;

class GridData : public Data
{

public:

    GridData () = delete;
    GridData (const char* fname);
    ~GridData ();

    static GridData &getGribDataInstance () { return *m_gridDataInstance; }

    const vector<vector<double>>& getWindSpeed (int fc)       { return windSpeed[fc];       }
    const vector<vector<int>>&    getWindDir (int fc)         { return windDir[fc];         }
    const vector<vector<double>>& getPressure (int fc)        { return pressure[fc];        }
    const vector<vector<double>>& getWaveSigHeight (int fc)   { return waveSigHeight[fc];   }
    const vector<vector<double>>& getWindWaveHeight (int fc)  { return windWaveHeight[fc];  }
    const vector<vector<int>>&    getWindWaveDir (int fc)     { return windWaveDir[fc];     }
    const vector<vector<int>>&    getWindWavePer (int fc)     { return windWavePer[fc];     }
    const vector<vector<double>>& getSwellWaveHeight (int fc)     { return swellWaveHeight[fc]; }
    const vector<vector<int>>&    getSwellWaveDir (int fc)        { return swellWaveDir[fc];    }
    const vector<vector<int>>&    getSwellWavePer (int fc)        { return swellWavePer[fc];    }
    const vector<vector<double>>& getCurrentSpeed (int fc)    { return currentSpeed[fc];    }
    const vector<vector<int>>&    getCurrentDir (int fc)      { return currentDir[fc];      }


    QPoint getMinPoint () const { return QPoint(xMin, yMin); }
    QPoint getMaxPoint () const { return QPoint(xMax, yMax); }
    int getD  () const { return Di; }
    int getNi () const { return Ni; }
    int getNj () const { return Nj; }

private:
    static GridData* m_gridDataInstance;

    int     decode (const char* fname);

    int     decode (int* fd);
    void    decodeWindSpeed (int fc, int* fd);
    void    decodeWindDir (int fc, int* fd);
    void    decodePressure (int fc, int* fd);
    void    decodeWaveSigHeight (int fc, int* fd);
    void    decodeWindWaveHeight (int fc, int* fd);
    void    decodeWindWaveDir (int fc, int* fd);
    void    decodeWindWavePeriod (int fc, int* fd);
    void    decodeSwellWaveHeight (int fc, int* fd);
    void    decodeSwellWaveDir (int fc, int* fd);
    void    decodeSwellWavePeriod (int fc, int* fd);
    void    decodeCurrentSpeed (int fc, int* fd);
    void    decodeCurrentDir (int fc, int* fd);

    unordered_map<int, vector<vector<double>>>  windSpeed;
    unordered_map<int, vector<vector<int>>>     windDir;
    unordered_map<int, vector<vector<double>>>  pressure;
    unordered_map<int, vector<vector<double>>>  waveSigHeight;
    unordered_map<int, vector<vector<double>>>  windWaveHeight;
    unordered_map<int, vector<vector<int>>>     windWaveDir;
    unordered_map<int, vector<vector<int>>>     windWavePer;
    unordered_map<int, vector<vector<double>>>  swellWaveHeight;
    unordered_map<int, vector<vector<int>>>     swellWaveDir;
    unordered_map<int, vector<vector<int>>>     swellWavePer;
    unordered_map<int, vector<vector<double>>>  currentSpeed;
    unordered_map<int, vector<vector<int>>>     currentDir;

    const int currentRotationFactor = 20;

};

#endif // GRIDDATA_H
