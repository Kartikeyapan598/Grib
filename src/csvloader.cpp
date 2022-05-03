#include "Utils.h"
#include "csvloader.h"
#include "qdebug.h"

#include <iostream>

#include <QFile>
#include <QFileDialog>
#include <QStringList>

bool readCSVRow (QTextStream &in, vector<string>* row)
{

    static const int delta[][5] = {
        //  ,    "   \n    ?  eof
        {   1,   2,  -1,   0,  -1  }, // 0: parsing (store char)
        {   1,   2,  -1,   0,  -1  }, // 1: parsing (store column)
        {   3,   4,   3,   3,  -2  }, // 2: quote entered (no-op)
        {   3,   4,   3,   3,  -2  }, // 3: parsing inside quotes (store char)
        {   1,   3,  -1,   0,  -1  }, // 4: quote exited (no-op)
        // -1: end of row, store column, success
        // -2: eof inside quotes
    };
    row->clear();
    if (in.atEnd())
    {
        return false;
    }

    int state = 0, t;
    char ch;
    string cell;

    while (state >= 0)
    {
        if (in.atEnd())
        {
            t = 4;
        }
        else
        {
            in >> ch;
            if (ch == ',') t = 0;
            else if (ch == '\"') t = 1;
            else if (ch == '\n') t = 2;
            else t = 3;
        }

        state = delta[state][t];

        switch (state) {
        case 0:
        case 3:
            cell += ch;
            break;
        case -1:
        case 1:
            //row->append(cell);
            row->push_back(cell);
            cell = "";
            break;
        }
    }
    if (state == -2)
    {
        throw runtime_error("End-of-file found while inside quotes.");
    }
    return true;
}

void OutVec(vector<string>& list)
{
    for(int i = 0; i < list.size(); i++)
    {
        cout<<list[i];
    }
    cout<<"\n";
}

void OutVec(vector<vector<string>>& list)
{
    for(int i = 0; i < list.size(); i++)
    {
        for(int j = 0; j < list[i].size(); j++)
        {
            cout<<list[i][j];
        }
        cout<<"\n";
    }
}

void CSVOut(QStandardItemModel* csv)
{
    QString fileName = QFileDialog::getOpenFileName(NULL, "FileName", QDir::currentPath(), "");
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << file.errorString();
        return;
    }

    QTextStream in(&file);
    vector<string> row;
    vector<string> header;
    vector<vector<string>> data;
    int i = 0;
    while (readCSVRow(in, &row))
    {
        //qDebug() << row;
        //header.push_back(row);
        //OutVec(row);
        if(i == 0)
        {
            header = row;
            data.push_back(row);
            i++;
        }
        else
        {
            data.push_back(row);
        }
    }
    if(header.size() != 0)
    {
        cout<<"Header --------------------\n";
        OutVec(header);
    }
    else
    {
        cout<<"HAHA Header size zero\n";
    }
    if(data.size() != 0)
    {
        cout<<"Data --------------------\n";
        OutVec(data);
    }
    else
    {
        cout<<"HAHA Data size zero\n";
    }
    file.close();
}


// ///////////////////////////////Other Solution///////////////////////////////


void ReadFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(NULL, "FileName", QDir::currentPath(), "");
    QFile bomFile(fileName);
    QTextStream bomIn(&bomFile);

    QList<QStringList> bomData;
    QStringList headers;
    int  lineNo = 0;

    while(!bomIn.atEnd())
    {
        QString line = bomIn.readLine();
        QStringList rowList = line.split(';');

        if ( lineNo > 0 )
           bomData.append(rowList);
        else
           headers = rowList;
        ++lineNo;
    }
    cout<<"Got Here\n";
    if(bomData.size() == 0)
    {
        qDebug() << "HAHA bomData Empty\n";
    }
    else
    {
        for(auto it : bomData)
        {
            qDebug() << it;
        }
    }
    if(headers.size() == 0)
    {
        qDebug() << "HAHA headers is empty\n";
    }
    else
    {
        qDebug() << "Headers File -------------------------------\n";
        for(auto it : headers)
        {
            qDebug() << it;
        }
    }
    bomFile.close();
    return;
}
