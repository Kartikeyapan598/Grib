#ifndef CSVROUTES_H
#define CSVROUTES_H
#include <vector>
#include <utility>
#include <string>
#include <fstream>
#include <sstream>
#include <QString>

class CsvRoutes
{
public:

    CsvRoutes(QString csvFileName);

    const std::vector<std::pair<float, float>>& getOrigCsvRoute()
                                                               { return orig_CsvRoute; }
    const std::vector<std::pair<float, float>>& getOptCsvRoute(int n)
                                                               { return opt_CsvRoutes[n - 1]; }

private :

    float degMinSecToDeg(char dir, std::string latLonString, std::string latLonMinString);
    std::vector<std::pair<float, float>> orig_CsvRoute;
    std::vector<std::vector<std::pair<float, float>>> opt_CsvRoutes;
};

#endif // CSVROUTES_H
