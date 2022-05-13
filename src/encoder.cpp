#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

namespace Constants
{
    const int WD_FLAG = 0;
    const int WV_FLAG = 1;
    const int CR_FLAG = 2;

    const int WD_ROWS = 181;
    const int WD_COLS = 360;

    const int WV_ROWS = 721;
    const int WV_COLS = 1440;

    const int CR_ROWS = 481;
    const int CR_COLS = 1080;

    const int HEADER_LEN = 10;

    const std::string ST_FILE = "wd_1.txt";
}

using namespace std;
using namespace Constants;

// file descriptor for writing to output binary file
int _fd;
int _flag;
std::string INV = "NaN";

// header info for output file
float xMin;
float yMin;
float xMax;
float yMax;
int Ni;
int Nj;
int Di;
int Dj;
time_t refTime;
int nbForecasts;

// header info from optimized csv
float xMinB;
float yMinB;
float xMaxB;
float yMaxB;
int NiB;
int NjB;
int DiB;
int DjB;
std::string refTimeB;
int nbForecastsB;

// storage
std::vector<std::vector<float>> values(WD_ROWS, std::vector<float>(WD_COLS, 0.0f));
std::vector<std::vector<float>> wv_values(WV_ROWS, std::vector<float>(WV_COLS, 0.0f));
std::vector<std::vector<float>> cr_values(CR_ROWS, std::vector<float>(CR_COLS, 0.0f));

/* UTIL funcs */

void skipHeaderInitial(std::fstream& inpstream) // do not mutate
{
    std::string line;

    for (int i = 0; i < HEADER_LEN; ++i)
    {
        std::getline(inpstream, line);
        if (i == 8)
        {
            refTimeB = line;
            break;
        }
        line.clear();
    }
}

void skipHeader(std::fstream& inpstream)
{
    std::string line;

    for (int i = 0; i < HEADER_LEN; ++i)
        std::getline(inpstream, line);
}

void extractAll(std::fstream& inpstream)
{
    std::string line;

    if (_flag == WD_FLAG)
    {
        for (int i = 0; i < WD_ROWS; ++i)
        {
            for (int j = 0; j < WD_COLS; ++j)
            {
                std::getline(inpstream, line);

                if (line != INV)
                    values[i][j] = std::stof(line);
                else
                    values[i][j] = 0.0f;
                // cout << values[i][j] << '\n';
                line.clear();
            }
        }
    }
    else if (_flag == WV_FLAG)
    {
        int skip = 4;

        for (int i = 0; i < WV_ROWS; ++i)
        {
            for (int j = 0; j < WV_COLS; ++j)
            {
                std::getline(inpstream, line);

                if (line != INV)
                    wv_values[i][j] = std::stof(line);
                else
                    wv_values[i][j] = 0.0f;
                // cout << wv_values[i][j] << '\n';
                line.clear();
            }
        }

        for (int i = 0; i < WV_ROWS; i += skip)
            for (int j = 0; j < WV_COLS; j += skip)
                values[i / skip][j / skip] = wv_values[i][j];
    }
    else if (_flag == CR_FLAG)
    {
        int skip = 3;

        // 481 * 1080 = 519480 values
        for (int i = 0; i < CR_ROWS; ++i)
        {
            for (int j = 0; j < CR_COLS; ++j)
            {
                std::getline(inpstream, line);
                // cout << line << '\n';

                if (line != INV)
                    cr_values[i][j] = std::stof(line);

                // cout << cr_values[i][j] << '\n';
                line.clear();
            }
        }

        for (int i = 0; i < CR_ROWS; i += skip)
            for (int j = 0; j < CR_COLS; j += skip)
                values[i / skip][j / skip] = cr_values[i][j];
    }
    // cout << "Header consumed\n";
}

void countNiNj()
{
    std::fstream inpstream;
    inpstream.open(ST_FILE);

    if (!inpstream.is_open())
    {
        std::cerr << "Error opening file " << ST_FILE << '\n';
        return;
    }

    skipHeaderInitial(inpstream);
    inpstream.close();

    int add = 90;
    NiB = NjB = 0;

    for (int j = yMin + add; j <= yMax + add; j += Dj)
        ++NjB;

    if (!std::signbit(xMin) && !std::signbit(xMax))
    {
        if (xMin < xMax)
        {
            for (int i = xMin; i <= xMax; i += Di)
                ++NiB;
        }
        else
        {
            int skip = 0;
            int skipped = 0;

            for (int i = xMin; i <= 359; i += Di)
            {
                ++NiB;
                if (i + Di > 359)
                    skipped = 359 - i;
            }

            skip = Di - 1 - skipped;

            for (int i = 0 + skip; i <= xMax; i += Di)
                ++NiB;
        }
    }
    else if (std::signbit(xMin) && std::signbit(xMax))
    {
        if (xMin < xMax)
        {
            for (int i = xMin + 360; i <= xMax + 360; i += Di)
                ++NiB;
        }
        else
        {
            int skip = 0;
            int skipped = 0;

            for (int i = xMin + 360; i <= 359; i += Di)
            {
                ++NiB;
                if (i + Di > 359)
                    skipped = 359 - i;
            }

            skip = Di - 1 - skipped;

            for (int i = 0 + skip; i <= xMax + 360; i += Di)
                ++NiB;
        }
    }
    else if (std::signbit(xMin) && !std::signbit(xMax))
    {
        int skip = 0;
        int skipped = 0;

        for (int i = xMin + 360; i <= 359; i += Di)
        {
            ++NiB;
            if (i + Di > 359)
                skipped = 359 - i;
        }

        skip = Di - 1 - skipped;

        for (int i = 0; i <= xMax; i += Di)
            ++NiB;
    }
    else if (!std::signbit(xMin) && std::signbit(xMax))
    {
        for (int i = xMin; i <= xMax + 360; i += Di)
            ++NiB;
    }
}

float degMinSecToDeg(char dir, std::string latLonString)
{
    // Decimal Degrees = d + (min/60) + (sec/3600)
    int k = 0;
    float deg = 0.0;

    for (int i = 0; i < latLonString.length(); i++)
    {
        string sub = latLonString.substr(k, i - k);
        if (latLonString[i] == -62)
        { // '°' is multi-char -> -62, -80
            deg += stoi(sub);
            k = i + 2;
            ++i;
        }
        else if (latLonString[i] == 39)
        {
            deg += stoi(sub) / 60.0;
            k = i + 1;
        }
        else if (latLonString[i] == 34)
        {
            deg += stof(sub) / 3600.0;
            break;
        }
    }

    switch (dir)
    {
    case 78: // 'N'
        break;
    case 83: // 'S'
        deg = (-1) * deg;
    case 69: // 'E'
        break;
    case 87: // 'W'
        deg = (-1) * deg;
    }

    return deg;
}

std::vector<float> getBoundingRect(const std::string& fileName)
{
    vector<float> boundingRect;
    float xMin = 0.0, yMin = 0.0, xMax = 0.0, yMax = 0.0;

    fstream f;
    f.open(fileName, ios::in);
    std::string line, cell;
    std::vector<std::string> row;

    int i = 0;
    // column # of lat, lon in csv file --- change if format changed
    int lad_col = 1;
    int la_col = 2;
    int lod_col = 3;
    int lo_col = 4;

    if (f.is_open())
    {
        while (std::getline(f, line))
        {
            row.clear();
            stringstream str(line);

            while (std::getline(str, cell, ','))
                row.push_back(cell);

            if (i == 0)
            { // header line skipped
                i++;
                continue;
            }

            char loDir = row.at(lod_col)[0];
            char laDir = row.at(lad_col)[0];
            std::string loStr = row.at(lo_col);
            std::string laStr = row.at(la_col);
            float xi = degMinSecToDeg(loDir, loStr);
            float yi = degMinSecToDeg(laDir, laStr);

            if (i == 1)
            {
                xMin = xi;
                yMin = yi;
                xMax = xi;
                yMax = yi;
                i++;
                continue;
            }

            if (xi < xMin)
                xMin = xi;
            if (xi > xMax)
                xMax = xi;
            if (yi < yMin)
                yMin = yi;
            if (yi > yMax)
                yMax = yi;
            i++;
        }
    }

    f.close();

    boundingRect.push_back(floor(xMin) - 3);
    boundingRect.push_back(floor(yMin) - 3);
    boundingRect.push_back(ceil(xMax) + 3);
    boundingRect.push_back(ceil(yMax) + 3);

    return boundingRect;
}

/* ENCODING funcs */

void encodeWind(const std::string& fname)
{
    std::fstream inpstream;
    inpstream.open(fname);

    if (!inpstream.is_open())
    {
        std::cerr << "Error opening file " << fname << '\n';
        return;
    }

    int add = 90;
    std::string line;

    skipHeader(inpstream);

    // cout << "Skipped header\n";

    if (!std::signbit(xMin) && !std::signbit(xMax)) // both >= 0
    {
        if (xMin < xMax)
        {
            // wind speed
            // int c = 0;
            extractAll(inpstream);

            for (int j = yMin + add; j <= yMax + add; j += Dj)
            {
                for (int i = xMin; i <= xMax; i += Di)
                {
                    // ++c;
                    uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 100.0);
                    write(_fd, &encoded, 1);
                }
            }

            // wind dir.
            extractAll(inpstream);

            for (int j = yMin + add; j <= yMax + add; j += Dj)
            {
                for (int i = xMin; i <= xMax; i += Di)
                {
                    if (std::signbit(values[i][j]))
                        values[i][j] = 360.0 + values[i][j];

                    uint8_t encoded = (uint8_t)std::round(values[i][j] * 255 / 360.0);
                    write(_fd, &encoded, 1);
                }
            }
        }
        else
        {
            // wind speed
            extractAll(inpstream);
            int skip = 0;
            int skipped = 0;

            for (int j = yMin + add; j <= yMax + add; j += Dj)
            {
                for (int i = xMin; i <= 359; i += Di)
                {
                    uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 100.0);
                    write(_fd, &encoded, 1);
                    if (i + Di > 359)
                        skipped = 359 - i;
                }
            }

            skip = Di - 1 - skipped;

            for (int j = yMin + add; j <= yMax + add; j += Dj)
            {
                for (int i = 0 + skip; i <= xMax; i += Di)
                {
                    uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 100.0);
                    write(_fd, &encoded, 1);
                }
            }


            // wind dir.
            skip = 0;
            skipped = 0;
            extractAll(inpstream); // consumes (181 * 360) wind dir values
            for (int j = yMin + add; j <= yMax + add; j += Dj)
            {
                for (int i = xMin; i <= 359; i += Di)
                {
                    if (std::signbit(values[i][j]))
                        values[i][j] = 360.0 + values[i][j];

                    uint8_t encoded = (uint8_t)std::round(values[i][j] * 255 / 360.0);
                    write(_fd, &encoded, 1);
                    if (i + Di > 359)
                        skipped = 359 - i;
                }
            }

            skip = Di - 1 - skipped;

            for (int j = yMin + add; j <= yMax + add; j += Dj)
            {
                for (int i = 0 + skip; i <= xMax; i += Di)
                {
                    if (std::signbit(values[i][j]))
                        values[i][j] = 360.0 + values[i][j];

                    uint8_t encoded = (uint8_t)std::round(values[i][j] * 255 / 360.0);
                    write(_fd, &encoded, 1);
                }
            }
        }
    }
    else if (std::signbit(xMin) && std::signbit(xMax)) // both < 0
    {
        if (xMin < xMax)
        {
            // wind speed
            extractAll(inpstream);

            for (int j = yMin + add; j <= yMax + add; j += Dj)
            {
                for (int i = xMin + 360; i <= xMax + 360; i += Di)
                {
                    uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 100.0);
                    write(_fd, &encoded, 1);
                }
            }

            // wind dir.
            extractAll(inpstream);

            for (int j = yMin + add; j <= yMax + add; j += Dj)
            {
                for (int i = xMin + 360; i <= xMax + 360; i += Di)
                {
                    if (std::signbit(values[i][j]))
                        values[i][j] = 360.0 + values[i][j];

                    uint8_t encoded = (uint8_t)std::round(values[i][j] * 255 / 360.0);
                    write(_fd, &encoded, 1);
                }
            }
        }
        else
        {
            // wind speed
            int skip = 0;
            int skipped = 0;
            extractAll(inpstream);

            for (int j = yMin + add; j <= yMax + add; j += Dj)
            {
                for (int i = xMin + 360; i <= 359; i += Di)
                {
                    uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 100.0);
                    write(_fd, &encoded, 1);
                    if (i + Di > 359)
                        skipped = 359 - i;
                }
            }

            skip = Di - 1 - skipped;

            for (int j = yMin + add; j <= yMax + add; j += Dj)
            {
                for (int i = 0 + skip; i <= xMax + 360; i += Di)
                {
                    uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 100.0);
                    write(_fd, &encoded, 1);
                }
            }

            // wind dir.
            skip = 0;
            skipped = 0;
            extractAll(inpstream);

            for (int j = yMin + add; j <= yMax + add; j += Dj)
            {
                for (int i = xMin + 360; i <= 359; i += Di)
                {
                    if (std::signbit(values[i][j]))
                        values[i][j] = 360.0 + values[i][j];

                    uint8_t encoded = (uint8_t)std::round(values[i][j] * 255 / 360.0);
                    write(_fd, &encoded, 1);
                    if (i + Di > 359)
                        skipped = 359 - i;
                }
            }

            skip = Di - 1 - skipped;

            for (int j = yMin + add; j <= yMax + add; j += Dj)
            {
                for (int i = 0 + skip; i <= xMax + 360; i += Di)
                {
                    if (std::signbit(values[i][j]))
                        values[i][j] = 360.0 + values[i][j];

                    uint8_t encoded = (uint8_t)std::round(values[i][j] * 255 / 360.0);
                    write(_fd, &encoded, 1);
                }
            }
        }
    }
    else if (std::signbit(xMin) && !std::signbit(xMax)) // < 0 && >= 0
    {
        // wind speed
        int skip = 0;
        int skipped = 0;
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin + 360; i <= 359; i += Di)
            {
                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 100.0);
                write(_fd, &encoded, 1);
                if (i + Di > 359)
                    skipped = 359 - i;
            }
        }

        skip = Di - 1 - skipped;

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = 0 + skip; i <= xMax; i += Di)
            {
                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 100.0);
                write(_fd, &encoded, 1);
            }
        }

        // wind dir.
        skip = 0;
        skipped = 0;
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin + 360; i <= 359; i += Di)
            {
                if (std::signbit(values[i][j]))
                    values[i][j] = 360.0 + values[i][j];

                uint8_t encoded = (uint8_t)std::round(values[i][j] * 255 / 360.0);
                write(_fd, &encoded, 1);
                if (i + Di > 359)
                    skipped = 359 - i;
            }
        }

        skip = Di - 1 - skipped;

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = 0 + skip; i <= xMax; i += Di)
            {
                if (std::signbit(values[i][j]))
                    values[i][j] = 360.0 + values[i][j];

                uint8_t encoded = (uint8_t)std::round(values[i][j] * 255 / 360.0);
                write(_fd, &encoded, 1);
            }
        }
    }
    else if (!std::signbit(xMin) && std::signbit(xMax)) // >= 0 && < 0
    {
        // wind speed
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin; i <= xMax + 360; i += Di)
            {
                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 100.0);
                write(_fd, &encoded, 1);
            }
        }

        // wind dir.
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin; i <= xMax + 360; i += Di)
            {
                if (std::signbit(values[i][j]))
                    values[i][j] = 360.0 + values[i][j];

                uint8_t encoded = (uint8_t)std::round(values[i][j] * 255 / 360.0);
                write(_fd, &encoded, 1);
            }
        }
    }

    inpstream.close(); // close i-th wind file
}

void encodePressure(const std::string& fname)
{
    std::fstream inpstream;
    inpstream.open(fname);

    if (!inpstream.is_open())
    {
        std::cerr << "Error opening file " << fname << '\n';
        return;
    }

    int add = 90;

    skipHeader(inpstream);

    extractAll(inpstream);

    if (!std::signbit(xMin) && !std::signbit(xMax))
    {
        if (xMin < xMax)
        {
            for (int j = yMin + add; j <= yMax + add; j += Dj)
            {
                for (int i = xMin; i <= xMax; i += Di)
                {
                    uint16_t encoded = (uint16_t)std::round(values[i][j]);
                    write(_fd, &encoded, 2);
                }
            }
        }
        else
        {
            int skip = 0;
            int skipped = 0;

            for (int j = yMin + add; j <= yMax + add; j += Dj)
            {
                for (int i = xMin; i <= 359; i += Di)
                {
                    uint16_t encoded = (uint16_t)std::round(values[i][j]);
                    write(_fd, &encoded, 2);
                    if (i + Di > 359)
                        skipped = 359 - i;
                }
            }

            skip = Di - 1 - skipped;

            for (int j = yMin + add; j <= yMax + add; j += Dj)
            {
                for (int i = 0 + skip; i <= xMax; i += Di)
                {
                    uint16_t encoded = (uint16_t)std::round((values[i][j] * 255) / 15000.0);
                    write(_fd, &encoded, 2);
                }
            }
        }
    }
    else if (std::signbit(xMin) && std::signbit(xMax))
    {
        if (xMin < xMax)
        {
            for (int j = yMin + add; j <= yMax + add; j += Dj)
            {
                for (int i = xMin + 360; i <= xMax + 360; i += Di)
                {
                    uint16_t encoded = (uint16_t)std::round(values[i][j]);
                    write(_fd, &encoded, 2);
                }
            }
        }
        else
        {
            int skip = 0;
            int skipped = 0;

            for (int j = yMin + add; j <= yMax + add; j += Dj)
            {
                for (int i = xMin + 360; i <= 359; i += Di)
                {
                    uint16_t encoded = (uint16_t)std::round(values[i][j]);
                    write(_fd, &encoded, 2);
                    if (i + Di > 359)
                        skipped = 359 - i;
                }
            }

            skip = Di - 1 - skipped;

            for (int j = yMin + add; j <= yMax + add; j += Dj)
            {
                for (int i = 0 + skip; i <= xMax + 360; i += Di)
                {
                    uint16_t encoded = (uint16_t)std::round(values[i][j]);
                    write(_fd, &encoded, 2);
                }
            }
        }
    }
    else if (std::signbit(xMin) && !std::signbit(xMax))
    {
        int skip = 0;
        int skipped = 0;

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin + 360; i <= 359; i += Di)
            {
                uint16_t encoded = (uint16_t)std::round(values[i][j]);
                write(_fd, &encoded, 2);
                if (i + Di > 359)
                    skipped = 359 - i;
            }
        }

        skip = Di - 1 - skipped;

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = 0 + skip; i <= xMax; i += Di)
            {
                uint16_t encoded = (uint16_t)std::round(values[i][j]);
                write(_fd, &encoded, 2);
            }
        }
    }
    else if (!std::signbit(xMin) && std::signbit(xMax))
    {
        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin; i <= xMax + 360; i += Di)
            {
                uint16_t encoded = (uint16_t)std::round(values[i][j]);
                write(_fd, &encoded, 2);
            }
        }
    }

    inpstream.close();
}

void encodeWave4(std::fstream& inpstream)
{
    // wave sig height values
    int add = 90;
    extractAll(inpstream);

    for (int j = yMin + add; j <= yMax + add; j += Dj)
    {
        for (int i = xMin; i <= xMax + 360; i += Di)
        {
            uint8_t encoded = (uint8_t)std::round(values[i][j] * 10.0);
            write(_fd, &encoded, 1);
        }
    }

    // wind wave height values
    extractAll(inpstream);

    for (int j = yMin + add; j <= yMax + add; j += Dj)
    {
        for (int i = xMin; i <= xMax + 360; i += Di)
        {
            uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 30.0);
            write(_fd, &encoded, 1);
        }
    }

    // wind wave dir. values
    extractAll(inpstream);

    for (int j = yMin + add; j <= yMax + add; j += Dj)
    {
        for (int i = xMin; i <= xMax + 360; i += Di)
        {
            if (std::signbit(values[i][j]))
                values[i][j] = 360.0 + values[i][j];

            uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 360.0);
            write(_fd, &encoded, 1);
        }
    }

    // wind wave period values
    extractAll(inpstream);
    uint8_t period = 0;
    std::vector<uint8_t> lin;

    for (int j = yMin + add; j <= yMax + add; j += Dj)
        for (int i = xMin; i <= xMax + 360; i += Di)
            lin.push_back((uint8_t)std::round((values[i][j] * 20) / 15.0));

    if (lin.size() % 2 == 0)
    {
        for (int i = 1; i < lin.size(); i += 2)
        {
            period |= (lin[i - 1] << 4);
            period |= (lin[i]);
            write(_fd, &period, 1);
            period = 0;
        }
    }
    else
    {
        for (int i = 1; i < lin.size(); i += 2)
        {
            period |= (lin[i - 1] << 4);
            period |= (lin[i]);
            write(_fd, &period, 1);
            period = 0;
        }
        period |= lin.back();
        write(_fd, &period, 1);
    }
}

void encodeWave3(std::fstream& inpstream)
{
    // wave sig height
    int add = 90;
    int skip = 0;
    int skipped = 0;
    extractAll(inpstream);

    for (int j = yMin + add; j <= yMax + add; j += Dj)
    {
        for (int i = xMin + 360; i <= 359; i += Di)
        {
            uint8_t encoded = (uint8_t)std::round(values[i][j] * 10.0);
            write(_fd, &encoded, 1);
            if (i + Di > 359)
                skipped = 359 - i;
        }
    }

    skip = Di - 1 - skipped;

    for (int j = yMin + add; j <= yMax + add; j += Dj)
    {
        for (int i = 0 + skip; i <= xMax; i += Di)
        {
            uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 20.0);
            write(_fd, &encoded, 1);
        }
    }

    // wind wave height
    skip = 0;
    skipped = 0;
    extractAll(inpstream);

    for (int j = yMin + add; j <= yMax + add; j += Dj)
    {
        for (int i = xMin + 360; i <= 359; i += Di)
        {
            uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 30.0);
            write(_fd, &encoded, 1);
            if (i + Di > 359)
                skipped = 359 - i;
        }
    }

    skip = Di - 1 - skipped;

    for (int j = yMin + add; j <= yMax + add; j += Dj)
    {
        for (int i = 0 + skip; i <= xMax; i += Di)
        {
            uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 30.0);
            write(_fd, &encoded, 1);
        }
    }

    // wind wave dir.
    skip = 0;
    skipped = 0;
    extractAll(inpstream);

    for (int j = yMin + add; j <= yMax + add; j += Dj)
    {
        for (int i = xMin + 360; i <= 359; i += Di)
        {
            if (std::signbit(values[i][j]))
                values[i][j] = 360.0 + values[i][j];

            uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 360.0);
            write(_fd, &encoded, 1);
            if (i + Di > 359)
                skipped = 359 - i;
        }
    }

    skip = Di - 1 - skipped;

    for (int j = yMin + add; j <= yMax + add; j += Dj)
    {
        for (int i = 0 + skip; i <= xMax; i += Di)
        {
            uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 360.0);
            write(_fd, &encoded, 1);
        }
    }

    // wind wave period
    skip = 0;
    skipped = 0;
    uint8_t period = 0;
    std::vector<uint8_t> lin;
    extractAll(inpstream);

    for (int j = yMin + add; j <= yMax + add; j += Dj)
    {
        for (int i = xMin + 360; i <= 359; i += Di)
        {
            lin.push_back((uint8_t)std::round((values[i][j] * 20) / 15.0));
            if (i + Di > 359)
                skipped = 359 - i;
        }
    }

    skip = Di - 1 - skipped;

    for (int j = yMin + add; j <= yMax + add; j += Dj)
        for (int i = 0 + skip; i <= xMax; i += Di)
            lin.push_back((uint8_t)std::round((values[i][j] * 20) / 15.0));

    if (lin.size() % 2 == 0)
    {
        for (int i = 1; i < lin.size(); i += 2)
        {
            period |= (lin[i - 1] << 4);
            period |= (lin[i]);
            write(_fd, &period, 1);
            period = 0;
        }
    }
    else
    {
        for (int i = 1; i < lin.size(); i += 2)
        {
            period |= (lin[i - 1] << 4);
            period |= (lin[i]);
            write(_fd, &period, 1);
            period = 0;
        }
        period |= lin.back();
        write(_fd, &period, 1);
    }
}

void encodeWave2(std::fstream& inpstream)
{
    int add = 90;

    if (xMin < xMax)
    {
        // wave sig height values
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin + 360; i <= xMax + 360; i += Di)
            {
                uint8_t encoded = (uint8_t)std::round(values[i][j] * 10.0);
                write(_fd, &encoded, 1);
            }
        }

        // wind wave height values
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin + 360; i <= xMax + 360; i += Di)
            {
                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 30.0);
                write(_fd, &encoded, 1);
            }
        }

        // wind wave dir. values
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin + 360; i <= xMax + 360; i += Di)
            {
                if (std::signbit(values[i][j]))
                    values[i][j] = 360.0 + values[i][j];

                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 360.0);
                write(_fd, &encoded, 1);
            }
        }

        // wind wave period values
        extractAll(inpstream);
        uint8_t period = 0;
        std::vector<uint8_t> lin;

        for (int j = yMin + add; j <= yMax + add; j += Dj)
            for (int i = xMin + 360; i <= xMax + 360; i += Di)
                lin.push_back((uint8_t)std::round((values[i][j] * 20) / 15.0));

        if (lin.size() % 2 == 0)
        {
            for (int i = 1; i < lin.size(); i += 2)
            {
                period |= (lin[i - 1] << 4);
                period |= (lin[i]);
                write(_fd, &period, 1);
                period = 0;
            }
        }
        else
        {
            for (int i = 1; i < lin.size(); i += 2)
            {
                period |= (lin[i - 1] << 4);
                period |= (lin[i]);
                write(_fd, &period, 1);
                period = 0;
            }
            period |= lin.back();
            write(_fd, &period, 1);
        }
    }
    else
    {
        // wave sig height
        int skip = 0;
        int skipped = 0;
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin + 360; i <= 359; i += Di)
            {
                uint8_t encoded = (uint8_t)std::round(values[i][j] * 10.0);
                write(_fd, &encoded, 1);
                if (i + Di > 359)
                    skipped = 359 - i;
            }
        }

        skip = Di - 1 - skipped;

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = 0 + skip; i <= xMax + 360; i += Di)
            {
                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 20.0);
                write(_fd, &encoded, 1);
            }
        }

        // wind wave height
        skip = 0;
        skipped = 0;
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin + 360; i <= 359; i += Di)
            {
                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 30.0);
                write(_fd, &encoded, 1);
                if (i + Di > 359)
                    skipped = 359 - i;
            }
        }

        skip = Di - 1 - skipped;

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = 0 + skip; i <= xMax + 360; i += Di)
            {
                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 30.0);
                write(_fd, &encoded, 1);
            }
        }

        // wind wave dir.
        skip = 0;
        skipped = 0;
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin + 360; i <= 359; i += Di)
            {
                if (std::signbit(values[i][j]))
                    values[i][j] = 360.0 + values[i][j];

                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 360.0);
                write(_fd, &encoded, 1);
                if (i + Di > 359)
                    skipped = 359 - i;
            }
        }

        skip = Di - 1 - skipped;

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = 0 + skip; i <= xMax + 360; i += Di)
            {
                if (std::signbit(values[i][j]))
                    values[i][j] = 360.0 + values[i][j];

                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 360.0);
                write(_fd, &encoded, 1);
            }
        }

        // wind wave period
        skip = 0;
        skipped = 0;
        uint8_t period = 0;
        std::vector<uint8_t> lin;
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin + 360; i <= 359; i += Di)
            {
                lin.push_back((uint8_t)std::round((values[i][j] * 20) / 15.0));
                if (i + Di > 359)
                    skipped = 359 - i;
            }
        }

        skip = Di - 1 - skipped;

        for (int j = yMin + add; j <= yMax + add; j += Dj)
            for (int i = 0 + skip; i <= xMax + 360; i += Di)
                lin.push_back((uint8_t)std::round((values[i][j] * 20) / 15.0));

        if (lin.size() % 2 == 0)
        {
            for (int i = 1; i < lin.size(); i += 2)
            {
                period |= (lin[i - 1] << 4);
                period |= (lin[i]);
                write(_fd, &period, 1);
                period = 0;
            }
        }
        else
        {
            for (int i = 1; i < lin.size(); i += 2)
            {
                period |= (lin[i - 1] << 4);
                period |= (lin[i]);
                write(_fd, &period, 1);
                period = 0;
            }
            period |= lin.back();
            write(_fd, &period, 1);
        }
    }
}

void encodeWave1(std::fstream& inpstream)
{
    int add = 90;

    if (xMin < xMax)
    {
        // wave sig height values
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin; i <= xMax; i += Di)
            {
                uint8_t encoded = (uint8_t)std::round(values[i][j] * 10.0);
                // cout << (uint32_t)encoded << '\n';
                write(_fd, &encoded, 1);
            }
        }

        // wind wave height values
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin; i <= xMax; i += Di)
            {
                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 30.0);
                // cout << (uint32_t)encoded << '\n';
                write(_fd, &encoded, 1);
            }
        }

        // wind wave dir. values
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin; i <= xMax; i += Di)
            {
                if (std::signbit(values[i][j]))
                    values[i][j] = 360.0 + values[i][j];

                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 360.0);
                // cout << (uint32_t)encoded << '\n';
                write(_fd, &encoded, 1);
            }
        }

        // wind wave period values
        extractAll(inpstream);
        uint8_t c = 1;
        uint8_t period = 0;
        std::vector<uint8_t> lin;

        for (int j = yMin + add; j <= yMax + add; j += Dj)
            for (int i = xMin; i <= xMax; i += Di)
                lin.push_back((uint8_t)std::round((values[i][j] * 20) / 15.0));

        if (lin.size() % 2 == 0)
        {
            for (int i = 1; i < lin.size(); i += 2)
            {
                period |= (lin[i - 1] << 4);
                period |= (lin[i]);
                // cout << (uint32_t)period << '\n';
                write(_fd, &period, 1);
                period = 0;
            }
        }
        else
        {
            for (int i = 1; i < lin.size(); i += 2)
            {
                period |= (lin[i - 1] << 4);
                period |= (lin[i]);
                // cout << (uint32_t)period << '\n';
                write(_fd, &period, 1);
                period = 0;
            }
            period |= lin.back();
            // cout << (uint32_t)period << '\n';
            write(_fd, &period, 1);
        }
    }
    else
    {
        // wave sig height
        int skip = 0;
        int skipped = 0;
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin; i <= 359; i += Di)
            {
                uint8_t encoded = (uint8_t)std::round(values[i][j] * 10.0);
                write(_fd, &encoded, 1);
                if (i + Di > 359)
                    skipped = 359 - i;
            }
        }

        skip = Di - 1 - skipped;

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = 0 + skip; i <= xMax; i += Di)
            {
                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 20.0);
                write(_fd, &encoded, 1);
            }
        }

        // wind wave height
        skip = 0;
        skipped = 0;
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin; i <= 359; i += Di)
            {
                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 30.0);
                write(_fd, &encoded, 1);
                if (i + Di > 359)
                    skipped = 359 - i;
            }
        }

        skip = Di - 1 - skipped;

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = 0 + skip; i <= xMax; i += Di)
            {
                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 30.0);
                write(_fd, &encoded, 1);
            }
        }

        // wind wave dir.
        skip = 0;
        skipped = 0;
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin; i <= 359; i += Di)
            {
                if (std::signbit(values[i][j]))
                    values[i][j] = 360.0 + values[i][j];

                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 360.0);
                write(_fd, &encoded, 1);
                if (i + Di > 359)
                    skipped = 359 - i;
            }
        }

        skip = Di - 1 - skipped;

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = 0 + skip; i <= xMax; i += Di)
            {
                if (std::signbit(values[i][j]))
                    values[i][j] = 360.0 + values[i][j];

                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 360.0);
                write(_fd, &encoded, 1);
            }
        }

        // wind wave period
        skip = 0;
        skipped = 0;
        uint8_t c = 1;
        uint8_t period = 0;
        std::vector<uint8_t> lin;
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin; i <= 359; i += Di)
            {
                lin.push_back((uint8_t)std::round((values[i][j] * 20) / 15.0));
                if (i + Di > 359)
                    skipped = 359 - i;
            }
        }

        skip = Di - 1 - skipped;

        for (int j = yMin + add; j <= yMax + add; j += Dj)
            for (int i = xMin; i <= 359; i += Di)
                lin.push_back((uint8_t)std::round((values[i][j] * 20) / 15.0));

        if (lin.size() % 2 == 0)
        {
            for (int i = 1; i < lin.size(); i += 2)
            {
                period |= (lin[i - 1] << 4);
                period |= (lin[i]);
                write(_fd, &period, 1);
                period = 0;
            }
        }
        else
        {
            for (int i = 1; i < lin.size(); i += 2)
            {
                period |= (lin[i - 1] << 4);
                period |= (lin[i]);
                write(_fd, &period, 1);
                period = 0;
            }
            period |= lin.back();
            write(_fd, &period, 1);
        }
    }
}

void encodeWave(const std::string& fname)
{
    std::fstream inpstream;
    inpstream.open(fname);

    if (!inpstream.is_open())
    {
        std::cerr << "Error opening file " << fname << '\n';
        return;
    }

    skipHeader(inpstream);

    if (!std::signbit(xMin) && !std::signbit(xMax))
        encodeWave1(inpstream);
    else if (std::signbit(xMin) && std::signbit(xMax))
        encodeWave2(inpstream);
    else if (std::signbit(xMin) && !std::signbit(xMax))
        encodeWave3(inpstream);
    else if (!std::signbit(xMin) && std::signbit(xMax))
        encodeWave4(inpstream);

    inpstream.close();
}

void encodeSwell4(std::fstream& inpstream)
{
    // swell wave height values
    int add = 90;
    extractAll(inpstream);

    for (int j = yMin + add; j <= yMax + add; j += Dj)
    {
        for (int i = xMin; i <= xMax + 360; i += Di)
        {
            uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 30.0);
            write(_fd, &encoded, 1);
        }
    }

    // swell wave dir. values
    extractAll(inpstream);

    for (int j = yMin + add; j <= yMax + add; j += Dj)
    {
        for (int i = xMin; i <= xMax + 360; i += Di)
        {
            if (std::signbit(values[i][j]))
                values[i][j] = 360.0 + values[i][j];

            uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 360.0);
            write(_fd, &encoded, 1);
        }
    }

    // swell wave period values
    extractAll(inpstream);
    uint8_t c = 1;
    uint8_t period = 0;
    std::vector<uint8_t> lin;

    for (int j = yMin + add; j <= yMax + add; j += Dj)
        for (int i = xMin; i <= xMax + 360; i += Di)
            lin.push_back((uint8_t)std::round((values[i][j] * 20) / 15.0));

    if (lin.size() % 2 == 0)
    {
        for (int i = 1; i < lin.size(); i += 2)
        {
            period |= (lin[i - 1] << 4);
            period |= (lin[i]);
            write(_fd, &period, 1);
            period = 0;
        }
    }
    else
    {
        for (int i = 1; i < lin.size(); i += 2)
        {
            period |= (lin[i - 1] << 4);
            period |= (lin[i]);
            write(_fd, &period, 1);
            period = 0;
        }
        period |= lin.back();
        write(_fd, &period, 1);
    }
}

void encodeSwell3(std::fstream& inpstream)
{
    // swell wave height
    int add = 90;
    int skip = 0;
    int skipped = 0;
    extractAll(inpstream);

    for (int j = yMin + add; j <= yMax + add; j += Dj)
    {
        for (int i = xMin + 360; i <= 359; i += Di)
        {
            uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 30.0);
            write(_fd, &encoded, 1);
            if (i + Di > 359)
                skipped = 359 - i;
        }
    }

    skip = Di - 1 - skipped;

    for (int j = yMin + add; j <= yMax + add; j += Dj)
    {
        for (int i = 0 + skip; i <= xMax; i += Di)
        {
            uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 30.0);
            write(_fd, &encoded, 1);
        }
    }

    // swell wave dir.
    skip = 0;
    skipped = 0;
    extractAll(inpstream);

    for (int j = yMin + add; j <= yMax + add; j += Dj)
    {
        for (int i = xMin + 360; i <= 359; i += Di)
        {
            if (std::signbit(values[i][j]))
                values[i][j] = 360.0 + values[i][j];

            uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 360.0);
            write(_fd, &encoded, 1);
            if (i + Di > 359)
                skipped = 359 - i;
        }
    }

    skip = Di - 1 - skipped;

    for (int j = yMin + add; j <= yMax + add; j += Dj)
    {
        for (int i = 0 + skip; i <= xMax; i += Di)
        {
            uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 360.0);
            write(_fd, &encoded, 1);
        }
    }

    // swell wave period
    skip = 0;
    skipped = 0;
    uint8_t period = 0;
    std::vector<uint8_t> lin;
    extractAll(inpstream);

    for (int j = yMin + add; j <= yMax + add; j += Dj)
    {
        for (int i = xMin + 360; i <= 359; i += Di)
        {
            lin.push_back((uint8_t)std::round((values[i][j] * 20) / 15.0));
            if (i + Di > 359)
                skipped = 359 - i;
        }
    }

    skip = Di - 1 - skipped;

    for (int j = yMin + add; j <= yMax + add; j += Dj)
        for (int i = 0 + skip; i <= xMax; i += Di)
            lin.push_back((uint8_t)std::round((values[i][j] * 20) / 15.0));

    if (lin.size() % 2 == 0)
    {
        for (int i = 1; i < lin.size(); i += 2)
        {
            period |= (lin[i - 1] << 4);
            period |= (lin[i]);
            write(_fd, &period, 1);
            period = 0;
        }
    }
    else
    {
        for (int i = 1; i < lin.size(); i += 2)
        {
            period |= (lin[i - 1] << 4);
            period |= (lin[i]);
            write(_fd, &period, 1);
            period = 0;
        }
        period |= lin.back();
        write(_fd, &period, 1);
    }
}

void encodeSwell2(std::fstream& inpstream)
{
    int add = 90;

    if (xMin < xMax)
    {
        // swell wave height values
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin + 360; i <= xMax + 360; i += Di)
            {
                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 30.0);
                write(_fd, &encoded, 1);
            }
        }

        // swell wave dir. values
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin + 360; i <= xMax + 360; i += Di)
            {
                if (std::signbit(values[i][j]))
                    values[i][j] = 360.0 + values[i][j];

                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 360.0);
                write(_fd, &encoded, 1);
            }
        }

        // swell wave period values
        extractAll(inpstream);
        uint8_t c = 1;
        uint8_t period = 0;
        std::vector<uint8_t> lin;

        for (int j = yMin + add; j <= yMax + add; j += Dj)
            for (int i = xMin + 360; i <= xMax + 360; i += Di)
                lin.push_back((uint8_t)std::round((values[i][j] * 20) / 15.0));

        if (lin.size() % 2)
        {
            for (int i = 1; i < lin.size(); i += 2)
            {
                period |= (lin[i - 1] << 4);
                period |= (lin[i]);
                write(_fd, &period, 1);
                period = 0;
            }
        }
        else
        {
            for (int i = 1; i < lin.size(); i += 2)
            {
                period |= (lin[i - 1] << 4);
                period |= (lin[i]);
                write(_fd, &period, 1);
                period = 0;
            }
            period |= lin.back();
            write(_fd, &period, 1);
        }
    }
    else
    {
        // swell wave height
        int skip = 0;
        int skipped = 0;
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin + 360; i <= 359; i += Di)
            {
                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 30.0);
                write(_fd, &encoded, 1);
                if (i + Di > 359)
                    skipped = 359 - i;
            }
        }

        skip = Di - 1 - skipped;

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = 0 + skip; i <= xMax + 360; i += Di)
            {
                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 30.0);
                write(_fd, &encoded, 1);
            }
        }

        // swell wave dir.
        skip = 0;
        skipped = 0;
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin + 360; i <= 359; i += Di)
            {
                if (std::signbit(values[i][j]))
                    values[i][j] = 360.0 + values[i][j];

                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 360.0);
                write(_fd, &encoded, 1);
                if (i + Di > 359)
                    skipped = 359 - i;
            }
        }

        skip = Di - 1 - skipped;

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = 0 + skip; i <= xMax + 360; i += Di)
            {
                if (std::signbit(values[i][j]))
                    values[i][j] = 360.0 + values[i][j];

                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 360.0);
                write(_fd, &encoded, 1);
            }
        }

        // swell wave period
        skip = 0;
        skipped = 0;
        uint8_t c = 1;
        uint8_t period = 0;
        std::vector<uint8_t> lin;
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin + 360; i <= 359; i += Di)
            {
                lin.push_back((uint8_t)std::round((values[i][j] * 20) / 15.0));
                if (i + Di > 359)
                    skipped = 359 - i;
            }
        }

        skip = Di - 1 - skipped;

        for (int j = yMin + add; j <= yMax + add; j += Dj)
            for (int i = 0 + skip; i <= xMax + 360; i += Di)
                lin.push_back((uint8_t)std::round((values[i][j] * 20) / 15.0));

        if (lin.size() % 2)
        {
            for (int i = 1; i < lin.size(); i += 2)
            {
                period |= (lin[i - 1] << 4);
                period |= (lin[i]);
                write(_fd, &period, 1);
                period = 0;
            }
        }
        else
        {
            for (int i = 1; i < lin.size(); i += 2)
            {
                period |= (lin[i - 1] << 4);
                period |= (lin[i]);
                write(_fd, &period, 1);
                period = 0;
            }
            period |= lin.back();
            write(_fd, &period, 1);
        }
    }
}

void encodeSwell1(std::fstream& inpstream)
{
    int add = 90;

    if (xMin < xMax)
    {
        // swell wave height values
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin; i <= xMax; i += Di)
            {
                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 30.0);
                write(_fd, &encoded, 1);
            }
        }

        // swell wave dir. values
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin; i <= xMax; i += Di)
            {
                if (std::signbit(values[i][j]))
                    values[i][j] = 360.0 + values[i][j];

                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 360.0);
                write(_fd, &encoded, 1);
            }
        }

        // swell wave period values
        extractAll(inpstream);
        uint8_t period = 0;
        std::vector<uint8_t> lin;

        for (int j = yMin + add; j <= yMax + add; j += Dj)
            for (int i = xMin; i <= xMax; i += Di)
                lin.push_back((uint8_t)std::round((values[i][j] * 20) / 15.0));

        if (lin.size() % 2)
        {
            for (int i = 1; i < lin.size(); i += 2)
            {
                period |= (lin[i - 1] << 4);
                period |= (lin[i]);
                write(_fd, &period, 1);
                period = 0;
            }
        }
        else
        {
            for (int i = 1; i < lin.size(); i += 2)
            {
                period |= (lin[i - 1] << 4);
                period |= (lin[i]);
                write(_fd, &period, 1);
                period = 0;
            }
            period |= lin.back();
            write(_fd, &period, 1);
        }
    }
    else
    {
        // swell wave height
        int skip = 0;
        int skipped = 0;
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin; i <= 359; i += Di)
            {
                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 30.0);
                write(_fd, &encoded, 1);
                if (i + Di > 359)
                    skipped = 359 - i;
            }
        }

        skip = Di - 1 - skipped;

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = 0 + skip; i <= xMax; i += Di)
            {
                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 30.0);
                write(_fd, &encoded, 1);
            }
        }

        // swell wave dir.
        skip = 0;
        skipped = 0;
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin; i <= 359; i += Di)
            {
                if (std::signbit(values[i][j]))
                    values[i][j] = 360.0 + values[i][j];

                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 360.0);
                write(_fd, &encoded, 1);
                if (i + Di > 359)
                    skipped = 359 - i;
            }
        }

        skip = Di - 1 - skipped;

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = 0 + skip; i <= xMax; i += Di)
            {
                if (std::signbit(values[i][j]))
                    values[i][j] = 360.0 + values[i][j];

                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 360.0);
                write(_fd, &encoded, 1);
            }
        }

        // swell wave period
        skip = 0;
        skipped = 0;
        uint8_t period = 0;
        std::vector<uint8_t> lin;
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin; i <= 359; i += Di)
            {
                lin.push_back((uint8_t)std::round((values[i][j] * 20) / 15.0));
                if (i + Di > 359)
                    skipped = 359 - i;
            }
        }

        skip = Di - 1 - skipped;

        for (int j = yMin + add; j <= yMax + add; j += Dj)
            for (int i = xMin; i <= 359; i += Di)
                lin.push_back((uint8_t)std::round((values[i][j] * 20) / 15.0));

        if (lin.size() % 2)
        {
            for (int i = 1; i < lin.size(); i += 2)
            {
                period |= (lin[i - 1] << 4);
                period |= (lin[i]);
                write(_fd, &period, 1);
                period = 0;
            }
        }
        else
        {
            for (int i = 1; i < lin.size(); i += 2)
            {
                period |= (lin[i - 1] << 4);
                period |= (lin[i]);
                write(_fd, &period, 1);
                period = 0;
            }
            period |= lin.back();
            write(_fd, &period, 1);
        }
    }
}

void encodeSwell(const std::string& fname)
{
    std::fstream inpstream;
    inpstream.open(fname);

    if (!inpstream.is_open())
    {
        std::cerr << "Error opening file " << fname << '\n';
        return;
    }

    skipHeader(inpstream);

    if (!std::signbit(xMin) && !std::signbit(xMax))
        encodeSwell1(inpstream);
    else if (std::signbit(xMin) && std::signbit(xMax))
        encodeSwell2(inpstream);
    else if (std::signbit(xMin) && !std::signbit(xMax))
        encodeSwell3(inpstream);
    else if (!std::signbit(xMin) && std::signbit(xMax))
        encodeSwell4(inpstream);

    inpstream.close();
}

void encodeCurrent(const std::string& fname)
{
    std::fstream inpstream;
    inpstream.open(fname);

    if (!inpstream.is_open())
    {
        std::cerr << "Error opening file " << fname << '\n';
        return;
    }

    int add = 90;

    skipHeader(inpstream);

    if (!std::signbit(xMin) && !std::signbit(xMax)) // both >= 0
    {
        if (xMin < xMax)
        {
            // current speed
            extractAll(inpstream);

            for (int j = yMin + add; j <= yMax + add; j += Dj)
            {
                for (int i = xMin; i <= xMax; i += Di)
                {
                    uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 4.0);
                    // cout << values[i][j] << '\n';
                    write(_fd, &encoded, 1);
                }
            }

            // current dir.
            extractAll(inpstream);

            for (int j = yMin + add; j <= yMax + add; j += Dj)
            {
                for (int i = xMin; i <= xMax; i += Di)
                {
                    if (std::signbit(values[i][j]))
                        values[i][j] = 360.0 + values[i][j];

                    uint8_t encoded = (uint8_t)std::round(values[i][j] * 255 / 360.0);
                    write(_fd, &encoded, 1);
                }
            }
        }
        else
        {
            // current speed
            extractAll(inpstream);
            int skip = 0;
            int skipped = 0;

            for (int j = yMin + add; j <= yMax + add; j += Dj)
            {
                for (int i = xMin; i <= 359; i += Di)
                {
                    uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 4.0);
                    write(_fd, &encoded, 1);
                    if (i + Di > 359)
                        skipped = 359 - i;
                }
            }

            skip = Di - 1 - skipped;

            for (int j = yMin + add; j <= yMax + add; j += Dj)
            {
                for (int i = 0 + skip; i <= xMax; i += Di)
                {
                    uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 4.0);
                    write(_fd, &encoded, 1);
                }
            }


            // current dir.
            skip = 0;
            skipped = 0;
            extractAll(inpstream);

            for (int j = yMin + add; j <= yMax + add; j += Dj)
            {
                for (int i = xMin; i <= 359; i += Di)
                {
                    if (std::signbit(values[i][j]))
                        values[i][j] = 360.0 + values[i][j];

                    uint8_t encoded = (uint8_t)std::round(values[i][j] * 255 / 360.0);
                    write(_fd, &encoded, 1);
                    if (i + Di > 359)
                        skipped = 359 - i;
                }
            }

            skip = Di - 1 - skipped;

            for (int j = yMin + add; j <= yMax + add; j += Dj)
            {
                for (int i = 0 + skip; i <= xMax; i += Di)
                {
                    if (std::signbit(values[i][j]))
                        values[i][j] = 360.0 + values[i][j];

                    uint8_t encoded = (uint8_t)std::round(values[i][j] * 255 / 360.0);
                    write(_fd, &encoded, 1);
                }
            }
        }
    }
    else if (std::signbit(xMin) && std::signbit(xMax)) // both < 0
    {
        if (xMin < xMax)
        {
            // current speed
            extractAll(inpstream);

            for (int j = yMin + add; j <= yMax + add; j += Dj)
            {
                for (int i = xMin + 360; i <= xMax + 360; i += Di)
                {
                    uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 4.0);
                    write(_fd, &encoded, 1);
                }
            }

            // current dir.
            extractAll(inpstream);

            for (int j = yMin + add; j <= yMax + add; j += Dj)
            {
                for (int i = xMin + 360; i <= xMax + 360; i += Di)
                {
                    if (std::signbit(values[i][j]))
                        values[i][j] = 360.0 + values[i][j];

                    uint8_t encoded = (uint8_t)std::round(values[i][j] * 255 / 360.0);
                    write(_fd, &encoded, 1);
                }
            }
        }
        else
        {
            // current speed
            int skip = 0;
            int skipped = 0;
            extractAll(inpstream);

            for (int j = yMin + add; j <= yMax + add; j += Dj)
            {
                for (int i = xMin + 360; i <= 359; i += Di)
                {
                    uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 4.0);
                    write(_fd, &encoded, 1);
                    if (i + Di > 359)
                        skipped = 359 - i;
                }
            }

            skip = Di - 1 - skipped;

            for (int j = yMin + add; j <= yMax + add; j += Dj)
            {
                for (int i = 0 + skip; i <= xMax + 360; i += Di)
                {
                    uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 4.0);
                    write(_fd, &encoded, 1);
                }
            }

            // current dir.
            skip = 0;
            skipped = 0;
            extractAll(inpstream);

            for (int j = yMin + add; j <= yMax + add; j += Dj)
            {
                for (int i = xMin + 360; i <= 359; i += Di)
                {
                    if (std::signbit(values[i][j]))
                        values[i][j] = 360.0 + values[i][j];

                    uint8_t encoded = (uint8_t)std::round(values[i][j] * 255 / 360.0);
                    write(_fd, &encoded, 1);
                    if (i + Di > 359)
                        skipped = 359 - i;
                }
            }

            skip = Di - 1 - skipped;

            for (int j = yMin + add; j <= yMax + add; j += Dj)
            {
                for (int i = 0 + skip; i <= xMax + 360; i += Di)
                {
                    if (std::signbit(values[i][j]))
                        values[i][j] = 360.0 + values[i][j];

                    uint8_t encoded = (uint8_t)std::round(values[i][j] * 255 / 360.0);
                    write(_fd, &encoded, 1);
                }
            }
        }
    }
    else if (std::signbit(xMin) && !std::signbit(xMax)) // < 0 && >= 0
    {
        // current speed
        int skip = 0;
        int skipped = 0;
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin + 360; i <= 359; i += Di)
            {
                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 4.0);
                write(_fd, &encoded, 1);
                if (i + Di > 359)
                    skipped = 359 - i;
            }
        }

        skip = Di - 1 - skipped;

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = 0 + skip; i <= xMax; i += Di)
            {
                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 4.0);
                write(_fd, &encoded, 1);
            }
        }

        // current dir.
        skip = 0;
        skipped = 0;
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin + 360; i <= 359; i += Di)
            {
                if (std::signbit(values[i][j]))
                    values[i][j] = 360.0 + values[i][j];

                uint8_t encoded = (uint8_t)std::round(values[i][j] * 255 / 360.0);
                write(_fd, &encoded, 1);
                if (i + Di > 359)
                    skipped = 359 - i;
            }
        }

        skip = Di - 1 - skipped;

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = 0 + skip; i <= xMax; i += Di)
            {
                if (std::signbit(values[i][j]))
                    values[i][j] = 360.0 + values[i][j];

                uint8_t encoded = (uint8_t)std::round(values[i][j] * 255 / 360.0);
                write(_fd, &encoded, 1);
            }
        }
    }
    else if (!std::signbit(xMin) && std::signbit(xMax)) // >= 0 && < 0
    {
        // current speed
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin; i <= xMax + 360; i += Di)
            {
                uint8_t encoded = (uint8_t)std::round((values[i][j] * 255) / 4.0);
                write(_fd, &encoded, 1);
            }
        }

        // current dir.
        extractAll(inpstream);

        for (int j = yMin + add; j <= yMax + add; j += Dj)
        {
            for (int i = xMin; i <= xMax + 360; i += Di)
            {
                if (std::signbit(values[i][j]))
                    values[i][j] = 360.0 + values[i][j];

                uint8_t encoded = (uint8_t)std::round(values[i][j] * 255 / 360.0);
                write(_fd, &encoded, 1);
            }
        }
    }

    inpstream.close(); // close i-th current file
}

void encode()
{
    std::string fname;

    for (int i = 1; i <= nbForecasts; ++i)
    {
        _flag = WD_FLAG;
        fname = "wd_" + to_string(i) + ".txt";
        encodeWind(fname);

        // fname = "pres_" + to_string(i) + ".txt";
        // encodePressure(fname);

        _flag = WV_FLAG;
        fname = "wv_" + to_string(i) + ".txt";
        encodeWave(fname);

        // fname = "sw_" + to_string(i) + ".txt";
        // encodeSwell(fname);

        _flag = CR_FLAG;
        fname = "curr_" + to_string(i) + ".txt";
        encodeCurrent(fname);
    }
}

void setupHeaderInfo()
{
    xMinB = xMin;
    write(_fd, &xMinB, sizeof(float));
    cout << xMinB << '\n';

    yMinB = yMin;
    write(_fd, &yMinB, sizeof(float));
    cout << yMinB << '\n';

    xMaxB = xMax;
    write(_fd, &xMaxB, sizeof(float));
    cout << xMaxB << '\n';

    yMaxB = yMax;
    write(_fd, &yMaxB, sizeof(float));
    cout << yMaxB << '\n';

    write(_fd, &NiB, 1);
    cout << NiB << '\n';

    write(_fd, &NjB, 1);
    cout << NjB << '\n';

    DiB = Di;
    write(_fd, &DiB, 1);
    cout << DiB << '\n';

    DjB = Dj;
    write(_fd, &DjB, 1);
    cout << DjB << '\n';

    time_t time = refTime;
    cout << time << '\n';
    write(_fd, &refTimeB, sizeof(time_t));

    nbForecastsB = nbForecasts;
    cout << nbForecastsB << '\n';
    write(_fd, &nbForecastsB, sizeof(int));
}

int main(int argc, char** argv) // args(2) : [ resolution, number of valid forecasts ]
{
    const char* outfile = "out.krup";
    _fd = open(outfile, O_RDWR);

    if (_fd < 0)
    {
        std::cerr << "Error opening output file\n";
        return 1;
    }

    if (argc < 3)
    {
        std::cerr << "Incomplete commandline arguments (needed 2, got " << argc - 1 << ")\n";
        return 1;
    }

    /* do not mutate */
    INV += (int)13;
    /* ------------- */

    std::string s;
    s = argv[1];

    Di = Dj = std::stoi(s);
    s = argv[2];
    nbForecasts = std::stoi(s);

    std::string windDr("wind_folder");     // wind folder
    std::string waveDr("wave_folder");     // wave folder
    std::string presDr("pressure_folder"); // pressure folder
    std::string currDr("current_folder");  // current folder

    // getting bounding rectangle
    std::string csvFileName("my_data.csv");
    std::vector<float> boundingRect = getBoundingRect(csvFileName);

    xMin = boundingRect.at(0);
    yMin = boundingRect.at(1);
    xMax = boundingRect.at(2);
    yMax = boundingRect.at(3);
    Ni = xMax - xMin;
    Nj = yMax - yMin;

    countNiNj(); // to count Ni, Nj

    // calculate time_t from string refTimeB   2022-04-30T12
    tm time{ 0 };
    time.tm_year = stoi(refTimeB.substr(0, 4)) - 1900;
    time.tm_mon = stoi(refTimeB.substr(5, 2)) - 1;
    time.tm_mday = stoi(refTimeB.substr(8, 2));
    time.tm_hour = stoi(refTimeB.substr(11, 2));
    time.tm_min = 0;
    time.tm_sec = 0;
    time.tm_isdst = 0;
    refTime = mktime(&time);

    setupHeaderInfo();

    encode();

    return 0;
}