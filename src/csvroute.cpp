#include "csvroute.h"
#include "qdebug.h"

CsvRoute::CsvRoute(QString csvFileName)
{
    std::fstream f(csvFileName.toStdString(), std::ios::in);

    std::string line, cell;
    std::vector<std::string> row;

    int i = 0;
    // column # of lat, lon in csv file --- change if format changed

    int lat_col = 1;
    int lat_minsec_col = 2;
    int lat_dir_col = 3;

    int lon_col = 4;
    int lon_minsec_col = 5;
    int lon_dir_col = 6;

    if (f)
    {
        qInfo() << "file opened";
        while (std::getline(f, line))
        {
            row.clear();
            std::stringstream str(line);

            while (std::getline(str, cell, ','))
                row.push_back(cell);

            if (i <= 3)
            { // header line skipped
                i++;
                continue;
            }

            std::string lonStr = row.at(lon_col);
            std::string lonMinSecStr = row.at(lon_minsec_col);
            char lonDir = row.at(lon_dir_col)[0];
            float xi = degMinSecToDeg(lonDir, lonStr, lonMinSecStr);

            std::string latStr = row.at(lat_col);
            std::string latMinSecStr = row.at(lat_minsec_col);
            char latDir = row.at(lat_dir_col)[0];
            float yi = degMinSecToDeg(latDir, latStr, latMinSecStr);

            std::pair<float, float> p;
            p.first = xi;
            p.second = yi;
            orig_CsvRoute.push_back(p);
        }
    }

    f.close();
}

float CsvRoute::degMinSecToDeg(char dir, std::string latLonString, std::string latLonMinString)
{
    // Decimal Degrees = d + (min/60) + (sec/3600)
    float deg = 0.0;

    deg += std::stof(latLonString) + std::stof(latLonMinString) / 60.0;

    switch (dir)
    {
    case 78:            // 'N'
        break;
    case 83:            // 'S'
        deg = (-1) * deg;
    case 69:            // 'E'
        break;
    case 87:            // 'W'
        deg = (-1) * deg;
    }

    return deg;
}

