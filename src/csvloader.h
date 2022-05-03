#pragma once

#include <QTextStream>
#include <QStandardItemModel>

#include <string>
#include <vector>
#include <istream>
#include <fstream>
#include <sstream>
#include <iterator>

using namespace std;

void CSVOut(QStandardItemModel*);
bool readCSVRow (QTextStream &in, QStringList *row);

void ReadFromFile();
