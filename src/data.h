#ifndef DATA_H
#define DATA_H

#include <time.h>
class Data
{
public:

    Data ();
    ~Data ();

    static Data &getDataInstance () { return *m_dataInstance; }

protected:

    int xMin, xMax, yMin, yMax;
    int Ni, Nj;
    float Di, Dj;
    time_t refDate;
    int nbForecasts;

private :
    static Data* m_dataInstance;
};

#endif // DATA_H
