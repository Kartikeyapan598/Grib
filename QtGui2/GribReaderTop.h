#ifndef GRIBREADERTOP_H
#define GRIBREADERTOP_H
#include <memory>

#include <QWidget>
#include <QToolBar>
#include <GribReader.h>
#include "LongTaskProgress.h"
#include <vector>

using v_arrow = QPair<double, double>;
class GribReaderTop : public QWidget
{
    Q_OBJECT

public:
    GribReaderTop (QWidget *parent);
    ~GribReaderTop();
    void    setCurrentDate (time_t t);
    time_t  getCurrentDate () const {return currentDate;};


    FileDataType  loadMeteoDataFile (const QString& fileName);
    FileDataType  getMeteoFileType()  {return currentFileType;}

    virtual void  loadFile (const QString &fileName, LongTaskProgress* taskProgress=NULL,
                            int nbrecs=0);

    GribReader *getReader()  const  {return gribRead != nullptr && gribRead->isOk()? gribRead: nullptr;}
    GribReader *getReaderT()  const  {return gribReader != nullptr && gribReader->isOk()? gribReader: nullptr;}


    GribRecord *getWindXRecord(time_t dataDate);
    GribRecord *getWindSRecord(time_t dataDate);
    GribRecord *getWindYRecord(time_t dataDate);
    GribRecord *getWindDRecord(time_t dataDate);
    QVector<QVector<v_arrow>> *getWindGridRecord (time_t dataDate);

    virtual bool  isReaderOk() const
                    {return gribRead!=nullptr && gribRead->isOk();}
    virtual bool  isReaderOkT() const
                    {return gribReader!=nullptr && gribReader->isOk();}

    virtual void setInterpolateValues (bool b)
                        {mustInterpolateValues = b;}
    virtual void interpolateMissingRecords (bool b);
    virtual void duplicateFirstCumulativeRecord (bool b);
    virtual void duplicateMissingWaveRecords (bool b);


    virtual void     setCurrentDateClosestFromNow ();

    virtual time_t   getClosestDateFromNow  () const
                        { return isReaderOk() ?
                            getReader()->getClosestDateFromNow():0;}

    virtual std::set<time_t> * getListDates ()  {return &listDates;}
    virtual int getNumberOfDates ()  {return listDates.size();}

    virtual std::set<Altitude> getAllAltitudes (int dataType) const;
    virtual std::set<DataCode> getAllDataCode ()  const;


private:
    FileDataType  currentFileType;
    GribReader  *gribRead;
    LongTaskProgress *taskProgress;
protected:
    void  loadGrib (LongTaskProgress* taskProgress, int nbrecs);
    void  initNewGribReaderTop ();
    GribReader 	*gribReader;
    QString 	fileName;
    time_t  	currentDate;
    std::set<time_t>    listDates;     // list of GribRecord dates

    bool    mustInterpolateValues;
    bool	mustDuplicateFirstCumulativeRecord;
    bool    mustInterpolateMissingRecords;
    bool    mustDuplicateMissingWaveRecords;
};

#endif // GRIBREADERTOP_H
