#include "griddata.h"
#include "qdebug.h"


GridData* GridData::m_gridDataInstance;

GridData::GridData (const char* fname)
{
    if(!m_gridDataInstance) {
        m_gridDataInstance = this;
    }
    else {
        return;
    }
    decode(fname);
}

GridData::~GridData ()
{
}



void GridData::decodeWindSpeed(int fc, int* fd)
{
    uint8_t t = 0;

    windSpeed[fc] = vector<vector<double>>(Ni, vector<double>(Nj, 0.0));
    for (int j = 0; j < Nj; j++)
    {
        for (int i = 0; i < Ni; i++)
        {
            if (read(*fd, &t, 1) != 1)
            {
                // QMsgbox read error
                // e.g. error reading wind speed data for forecast 'fc'
                return;
            }

            double value = (double)(t * 80) / 255.0;
            windSpeed[fc][i][j] = value;
        }
    }
}

void GridData::decodeWindDir(int fc, int* fd)
{
    uint8_t t = 0;

    windDir[fc] = vector<vector<int>>(Ni, vector<int>(Nj, 0.0));

    for (int j = 0; j < Nj; j++)
    {
        for (int i = 0; i < Ni; i++)
        {
            if (read(*fd, &t, 1) != 1)
            {
                // QMsgbox read error
                return;
            }

            int value = t * (360 / 255.0);
            windDir[fc][i][j] = value;
        }
    }
}

void GridData::decodePressure(int fc, int* fd)
{
    uint16_t t = 0;

    pressure[fc] = vector<vector<double>>(Ni, vector<double>(Nj, 0.0));

    for (int j = 0; j < Nj; j++)
    {
        for (int i = 0; i < Ni; i++)
        {
            if (read(*fd, &t, 2) != 2)
            {
                // QMsgbox read error
                return;
            }

    //        double value = t * (15000 / 255.0);
            double value = t;
            pressure[fc][i][j] = value * 10;
        }
    }
}

void GridData::decodeWaveSigHeight(int fc, int* fd)
{
    uint8_t t = 0;
    int fieldsToRead = Ni*Nj;

    waveSigHeight[fc] = vector<vector<double>>(Ni, vector<double>(Nj, 0.0));

    vector<double> waveSigHeightTemp;

    while (fieldsToRead)
    {
        if (read(*fd, &t, 1) != 1)
        {
            // QMsgbox read error
            return;
        }
        fieldsToRead--;
        double value = t * (20 / 255.0);
        waveSigHeightTemp.push_back(value);
    }

    for (int j = 0; j < Nj; j++)
    {
        for (int i = 0; i < Ni; i++)
        {
            waveSigHeight[fc][i][j] = waveSigHeightTemp[i + j*Ni];
        }
    }
}

void GridData::decodeWindWaveHeight(int fc, int* fd)
{
    uint8_t t = 0;
    windWaveHeight[fc] = vector<vector<double>>(Ni, vector<double>(Nj, 0.0));

    for (int j = 0; j < Nj; j++)
    {
        for (int i = 0; i < Ni; i++)
        {
            if (read(*fd, &t, 1) != 1)
            {
                // QMsgbox read error
                return;
            }

            double value = t * (30 / 255.0);
            windWaveHeight[fc][i][j] = value;
        }
    }
}

void GridData::decodeWindWaveDir(int fc, int* fd)
{
    uint8_t t = 0;
    windWaveDir[fc] = vector<vector<int>>(Ni, vector<int>(Nj, 0.0));

    for (int j = 0; j < Nj; j++)
    {
        for (int i = 0; i < Ni; i++)
        {
            if (read(*fd, &t, 1) != 1)
            {
                // QMsgbox read error
                return;
            }

            int value = t * (360 / 255.0);
            windWaveDir[fc][i][j] = value;
        }
    }
}

void GridData::decodeWindWavePeriod(int fc, int* fd)
{
    uint8_t t = 0;
    uint8_t val = 0;
    int fieldsToRead = Ni * Nj;

    windWavePer[fc] = vector<vector<int>>(Ni, vector<int>(Nj, 0.0));

    vector<int> windWavePerTemp;

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
        windWavePerTemp.push_back(value);
        t >>= 4;
        val = 0;

        if (fieldsToRead)
        {
            --fieldsToRead;

            for (uint8_t i = 0; i < 4; ++i)
                val |= (t & (1 << i));

            value = val * (20 / 15.0);
            windWavePerTemp.push_back(value);
            t = 0;
            val = 0;
        }
        else
        {
            break;
        }
    }

    for (int j = 0; j < Nj; j++){
        for (int i = 0; i < Ni; i++) {
            windWavePer[fc][i][j] = windWavePerTemp[i + j*Ni];
        }
    }
}

void GridData::decodeSwellWaveHeight(int fc, int* fd)
{
    uint8_t t = 0;
    swellWaveHeight[fc] = vector<vector<double>>(Ni, vector<double>(Nj, 0.0));

    for (int j = 0; j < Nj; j++)
    {
        for (int i = 0; i < Ni; i++)
        {
            if (read(*fd, &t, 1) != 1)
            {
                // QMsgbox read error
                return;
            }

            double value = t * (30 / 255.0);
            swellWaveHeight[fc][i][j] = value;
        }
    }
}

void GridData::decodeSwellWaveDir(int fc, int* fd) // 1 byte = 1 cell's Swell Wave Dir. value
{
    uint8_t t = 0;
    swellWaveDir[fc] = vector<vector<int>>(Ni, vector<int>(Nj, 0.0));

    for (int j = 0; j < Nj; j++)
    {
        for (int i = 0; i < Ni; i++)
        {
            if (read(*fd, &t, 1) != 1)
            {
                // QMsgbox read error
                return;
            }

            double value = t * (360 / 255.0);
            swellWaveDir[fc][i][j] = value;
        }
    }
}

void GridData::decodeSwellWavePeriod(int fc, int* fd)
{
    uint8_t t = 0;
    uint8_t val = 0;
    int fieldsToRead = Ni * Nj;

    swellWavePer[fc] = vector<vector<int>>(Ni, vector<int>(Nj, 0.0));

    vector<int> swellWavePerTemp;

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
        swellWavePerTemp.push_back(value);
        t >>= 4;
        val = 0;

        if (fieldsToRead)
        {
            --fieldsToRead;

            for (uint8_t i = 0; i < 4; ++i)
                val |= (t & (1 << i));

            value = val * (20 / 15.0);
            swellWavePerTemp.push_back(value);
            t = 0;
            val = 0;
        }
        else
        {
            break;
        }
    }

    for (int j = 0; j < Nj; j++){
        for (int i = 0; i < Ni; i++) {
            swellWavePer[fc][i][j] = swellWavePerTemp[i + j*Ni];
        }
    }
}

void GridData::decodeCurrentSpeed(int fc, int* fd)
{
    uint8_t t = 0;
    currentSpeed[fc] = vector<vector<double>>(Ni, vector<double>(Nj, 0.0));

    for (int j = 0; j < Nj; j++)
    {
        for (int i = 0; i < Ni; i++)
        {
            if (read(*fd, &t, 1) != 1)
            {
                // QMsgbox read error
                // e.g. error reading current speed data for forecast 'fc'
                qInfo() << "Error reading current speed for forecast: " << fc << '\n';
                return;
            }

            double value = t * (4 / 255.0);
            currentSpeed[fc][i][j] = value;
        }
    }
    for (int i = fc + 1; i < nbForecasts; ++i) {
        currentSpeed[i] = currentSpeed[fc];
    }
}

void GridData::decodeCurrentDir(int fc, int* fd)
{
    uint8_t t = 0;
    currentDir[fc] = vector<vector<int>>(Ni, vector<int>(Nj, 0.0));

    for (int j = 0; j < Nj; j++)
    {
        for (int i = 0; i < Ni; i++)
        {
            if (read(*fd, &t, 1) != 1)
            {
                // QMsgbox read error
                return;
            }

            int value = t * (360 / 255.0);
            currentDir[fc][i][j] = value;
        }
    }
    for (int i = fc + 1; i < nbForecasts; ++i) {
        currentDir[i] = currentDir[fc];
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
//        decodeWindWavePeriod(i, fd);

        decodeSwellWaveHeight(i, fd);
        decodeSwellWaveDir(i, fd);
//        decodeSwellWavePeriod(i, fd);
    }
//    for (int i = 0; i < nbForecasts; i += 20)
//    {
//        decodeCurrentSpeed(i, fd);
//        decodeCurrentDir(i, fd);
//    }
        decodeCurrentSpeed(0, fd);
        decodeCurrentDir(0, fd);

    return 0;
}

int GridData::decode (const char* fname)
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

    qInfo() << xMin;
    qInfo() << yMin;
    qInfo() << xMax;
    qInfo() << yMax;

    if (read(fd, &t, 4) != 4)
        {
            // QMessagebox error
            return -1;
        }

        Ni = t;
        t = 0;
        qInfo() << Ni;

        if (read(fd, &t, 4) != 4)
        {
            // QMessagebox error
            return -1;
        }

        Nj = t;
        t = 0;
        qInfo() << Nj;

        if (read(fd, &t, 1) != 1)
        {
            // QMessagebox error
            return -1;
        }

        Di = t;
        t = 0;
        qInfo() << Di;

        if (read(fd, &t, 1) != 1)
        {
            // QMessagebox error
            return -1;
        }

        Dj = t;
        t = 0;
        qInfo() << Dj;

    if (read(fd, &refDate, sizeof(time_t)) != sizeof(time_t))
    {
        // QMessagebox error
        return -1;
    }

    qInfo() << refDate;

    if (read(fd, &nbForecasts, 4) != 4)
    {
        // QMessagebox error
        return -1;
    }

    qInfo() << nbForecasts;

    int status = decode(&fd);
    close(fd);

    return status;
}
