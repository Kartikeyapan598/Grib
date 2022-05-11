#include <vector>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include "griddata.h"


GridData* GridData::m_gridDataInstance;

GridData::GridData(const char* fname)
{
    if (!m_gridDataInstance) {
        m_gridDataInstance = this;
    }
    else {
        return;
    }
    decode(fname);
}
GridData::~GridData()
{
}



void GridData::decodeWindSpeed(int fc, int* fd)
{
    uint8_t t = 0;
    int fieldsToRead = Ni * Nj;

    while (fieldsToRead)
    {
        if (read(*fd, &t, 1) != 1)
        {
            // QMsgbox read error
            // e.g. error reading wind speed data for forecast 'fc'
            return;
        }
        --fieldsToRead;

        double value = t * (100 / 255.0);
        windSpeed[fc].push_back(value);
    }
}

void GridData::decodeWindDir(int fc, int* fd)
{
    uint8_t t = 0;
    int fieldsToRead = Ni * Nj;

    while (fieldsToRead)
    {
        if (read(*fd, &t, 1) != 1)
        {
            // QMsgbox read error
            return;
        }
        --fieldsToRead;

        int value = t * (360 / 255.0);
        windDir[fc].push_back(value);
    }
}

void GridData::decodePressure(int fc, int* fd)
{
    uint16_t t = 0;
    int fieldsToRead = Ni * Nj;

    while (fieldsToRead)
    {
        if (read(*fd, &t, 2) != 2)
        {
            // QMsgbox read error
            return;
        }
        --fieldsToRead;

        //        double value = t * (15000 / 255.0);
        double value = t;
        pressure[fc].push_back(value);
    }
}

void GridData::decodeWaveSigHeight(int fc, int* fd)
{
    uint8_t t = 0;
    int fieldsToRead = Ni * Nj;

    while (fieldsToRead)
    {
        if (read(*fd, &t, 1) != 1)
        {
            // QMsgbox read error
            return;
        }
        --fieldsToRead;

        double value = t * (20 / 255.0);
        waveSigHeight[fc].push_back(value);
    }
}

void GridData::decodeWindWaveHeight(int fc, int* fd)
{
    uint8_t t = 0;
    int fieldsToRead = Ni * Nj;

    while (fieldsToRead)
    {
        if (read(*fd, &t, 1) != 1)
        {
            // QMsgbox read error
            return;
        }
        --fieldsToRead;

        double value = t * (30 / 255.0);
        windWaveHeight[fc].push_back(value);
    }
}

void GridData::decodeWindWaveDir(int fc, int* fd)
{
    uint8_t t = 0;
    int fieldsToRead = Ni * Nj;

    while (fieldsToRead)
    {
        if (read(*fd, &t, 1) != 1)
        {
            // QMsgbox read error
            return;
        }
        --fieldsToRead;

        int value = t * (360 / 255.0);
        windWaveDir[fc].push_back(value);
    }
}

void GridData::decodeWindWavePeriod(int fc, int* fd)
{
    uint8_t t = 0;
    uint8_t val = 0;
    int fieldsToRead = Ni * Nj;

    while (fieldsToRead)
    {
        if (read(*fd, &t, 1) != 1)
        {
            // QMsgbox read error
            return;
        }
        --fieldsToRead;

        for (uint8_t i = 0; i < 4; ++i)
            val |= (t & (1 << i));

        double value = val * (20 / 15.0);
        windWavePer[fc].push_back(value);
        t >>= 4;
        val = 0;

        if (fieldsToRead)
        {
            --fieldsToRead;

            for (uint8_t i = 0; i < 4; ++i)
                val |= (t & (1 << i));

            value = val * (20 / 15.0);
            windWavePer[fc].push_back(value);
            t >>= 4;
            val = 0;
        }
        else
        {
            break;
        }
    }
}

void GridData::decodeSwellWaveHeight(int fc, int* fd)
{
    uint8_t t = 0;
    int fieldsToRead = Ni * Nj;

    while (fieldsToRead)
    {
        if (read(*fd, &t, 1) != 1)
        {
            // QMsgbox read error
            return;
        }
        --fieldsToRead;

        double value = t * (30 / 255.0);
        swellWaveHeight[fc].push_back(value);
    }
}

void GridData::decodeSwellWaveDir(int fc, int* fd) // 1 byte = 1 cell's Swell Wave Dir. value
{
    uint8_t t = 0;
    int fieldsToRead = Ni * Nj;

    while (fieldsToRead)
    {
        if (read(*fd, &t, 1) != 1)
        {
            // QMsgbox read error
            return;
        }
        --fieldsToRead;

        double value = t * (360 / 255.0);
        swellWaveDir[fc].push_back(value);
    }
}

void GridData::decodeSwellWavePeriod(int fc, int* fd)
{
    uint8_t t = 0;
    uint8_t val = 0;
    int fieldsToRead = ni * nj;

    while (fieldsToRead > 0)
    {
        if (read(*fd, &t, 1) != 1)
        {
            // QMsgbox read error
            return;
        }
        --fieldsToRead;

        for (uint8_t i = 0; i < 4; ++i)
            val |= (t & (1 << i));

        double value = val * (20 / 15.0);
        swellWavePer[fc].push_back(value);
        t = (t >> 4);
        val = 0;

        if (fieldsToRead)
        {
            --fieldsToRead;

            for (uint8_t i = 0; i < 4; ++i)
                val |= (t & (1 << i));

            value = val * (20 / 15.0);
            swellWavePer[fc].push_back(value);
            t >>= 4;
            val = 0;
        }
        else
        {
            break;
        }
    }
}

void GridData::decodeCurrentSpeed(int fc, int* fd)
{
    uint8_t t = 0;
    int fieldsToRead = ni * nj;

    while (fieldsToRead)
    {
        if (read(*fd, &t, 1) != 1)
        {
            // QMsgbox read error
            // e.g. error reading wind speed data for forecast 'fc'
            std::cerr << "Error reading wind speed for forecast: " << fc << '\n';
            return;
        }
        --fieldsToRead;

        double value = t * (100 / 255.0);
        currentSpeed[fc].push_back(value);
    }
}

void GridData::decodeCurrentDir(int fc, int* fd)
{
    uint8_t t = 0;
    int fieldsToRead = ni * nj;

    while (fieldsToRead)
    {
        if (read(*fd, &t, 1) != 1)
        {
            // QMsgbox read error
            return;
        }
        --fieldsToRead;

        int value = t * (360 / 255.0);
        currentDir[fc].push_back(value);
    }
}

int GridData::decode(int* fd)
{
    for (int i = 0; i < nbForecasts; ++i)
    {
        decodeWindSpeed(i, fd);
        decodeWindDir(i, fd);

        decodePressure(i, fd);

        decodeWaveSigHeight(i, fd);
        decodeWindWaveHeight(i, fd);
        decodeWindWaveDir(i, fd);
        decodeWindWavePeriod(i, fd);

        decodeSwellWaveHeight(i, fd);
        decodeSwellWaveDir(i, fd);
        decodeSwellWavePeriod(i, fd);

        decodeCurrentSpeed(i, fd);
        decodeCurrentDir(i, fd);
    }

    return 0;
}

int GridData::decode(const char* fname)
{
    int fd = open(fname, O_RDONLY);
    if (fd < 0)
    {
        // QMessagebox error
        return -1;
    }

    uint32_t t = 0;
    float buf[4] = { 0, 0, 0, 0 };

    if (read(fd, buf, 16) != 16)
    {
        // QMessagebox error
        return -1;
    }

    xMin = buf[0];
    yMin = buf[1];
    xMax = buf[2];
    yMax = buf[3];

    if (read(fd, &t, 1) != 1)
    {
        // QMessagebox error
        return -1;
    }

    Ni = t;
    t = 0;

    if (read(fd, &t, 1) != 1)
    {
        // QMessagebox error
        return -1;
    }

    Nj = t;
    t = 0;

    if (read(fd, &t, 1) != 1)
    {
        // QMessagebox error
        return -1;
    }

    Di = t;
    t = 0;

    if (read(fd, &t, 1) != 1)
    {
        // QMessagebox error
        return -1;
    }

    Dj = t;
    t = 0;

    if (read(fd, &refDate, 8) != 8)
    {
        // QMessagebox error
        return -1;
    }

    if (read(fd, &nbForecasts, 4) != 4)
    {
        // QMessagebox error
        return -1;
    }

    int status = decode(&fd);
    close(fd);

    return status;
}
