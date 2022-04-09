#include "GribReaderTop.h"
#include "DataQString.h"
#include <QProgressDialog>

GribReaderTop::GribReaderTop(QWidget *parent) : QWidget(parent)
{
    initNewGribReaderTop();
    currentFileType = DATATYPE_NONE;
    gribRead = nullptr;
    taskProgress = nullptr;
}

GribReaderTop::~GribReaderTop() {
    listDates.clear();
    delete gribReader;
}

FileDataType GribReaderTop::loadMeteoDataFile (const QString& fileName)
{
    currentFileType = DATATYPE_NONE;
    bool ok = false;

    taskProgress = new LongTaskProgress (this);
    assert (taskProgress);
    taskProgress->continueDownload = true;

    if (gribRead != nullptr) {
        delete gribRead;
        gribRead = nullptr;
    }
    taskProgress->setMessage();
    taskProgress->setValue (0);

    int nbrecs;
    {
        ZUFILE *file = zu_open (qPrintable(fileName), "rb", ZU_COMPRESS_AUTO);
        if (file == nullptr) {
            erreur("Can't open file: %s", qPrintable(fileName));
            taskProgress->setVisible (false);
            delete taskProgress;
            taskProgress = nullptr;
            return DATATYPE_NONE;
        }
        GribReader r;
        if(taskProgress == nullptr)
            taskProgress->continueDownload = false;
        nbrecs = r.countGribRecords (file);
        zu_close(file);
    }

    //----------------------------------------------
    gribReader = nullptr;
    if (nbrecs>0 && !ok) {	// try to load a GRIB file
        //DBGQS("try to load a GRIB file: "+fileName);

        //taskProgress->setWindowTitle (tr("Open file")+" GRIB");
        taskProgress->setVisible (true);
        taskProgress->setValue (0);

        gribReader = new GribReader ();
        assert(gribReader);

        loadFile (fileName, taskProgress, nbrecs);    // GRIB file ?

        if (isReaderOkT()) {
            currentFileType = DATATYPE_GRIB;
            ok = true;
        }
        else {
            delete gribReader;
            gribReader = nullptr;
        }
    }
    taskProgress->setVisible (false);

    gribRead = gribReader;
    if (ok) {
        switch (currentFileType) {
            case DATATYPE_GRIB :
                setCurrentDateClosestFromNow ();
                setInterpolateValues (mustInterpolateValues);
                interpolateMissingRecords (mustInterpolateMissingRecords);
                duplicateFirstCumulativeRecord (mustDuplicateFirstCumulativeRecord);
                duplicateMissingWaveRecords (mustDuplicateMissingWaveRecords);
                break;

             default :
                break;
        }
    }
    update();

    bool cancelled = ! taskProgress->continueDownload;
    delete taskProgress;
    taskProgress = nullptr;

    if (cancelled)
        return DATATYPE_CANCELLED;

    return currentFileType;
}

void GribReaderTop::initNewGribReaderTop()
{
    gribReader = nullptr;

}

void GribReaderTop::loadGrib (LongTaskProgress* taskProgress, int nbrecs)
{
    listDates.clear();

    if (taskProgress != nullptr)
    {
        QObject::connect(gribReader, &LongTaskMessage::valueChanged,
                taskProgress, &LongTaskProgress::setValue);

        QObject::connect(gribReader, &LongTaskMessage::newMessage,
                taskProgress, &LongTaskProgress::setMessage);

        QObject::connect(taskProgress,   &LongTaskProgress::canceled,
                gribReader, &LongTaskMessage::cancel);
    }
    gribReader->openFile (fileName, nbrecs);
    if (gribReader->isOk())
    {
        listDates = gribReader->getListDates();
        setCurrentDate ( !listDates.empty() ? *(listDates.begin()) : 0);
    }
}
//----------------------------------------------------
void GribReaderTop::loadFile (const QString &fileName, LongTaskProgress* taskProgress, int nbrecs)
{
    this->fileName = fileName;
    delete gribReader;
    gribReader = new GribReader ();
    loadGrib(taskProgress, nbrecs);
    if (isReaderOkT())
        return;
}

//----------------------------------------------------
void GribReaderTop::duplicateFirstCumulativeRecord ( bool mustDuplicate )
{
    mustDuplicateFirstCumulativeRecord = mustDuplicate;
    if (isReaderOkT())
    {
        if (mustDuplicate) {
            gribReader->copyFirstCumulativeRecord ();
        }
        else {
            gribReader->removeFirstCumulativeRecord ();
        }
    }
}

//----------------------------------------------------
void GribReaderTop::duplicateMissingWaveRecords ( bool mustDuplicate )
{
    mustDuplicateMissingWaveRecords = mustDuplicate;
    if (isReaderOkT())
    {
        if (mustDuplicate) {
            gribReader->copyMissingWaveRecords ();
        }
        else {
            gribReader->removeMissingWaveRecords ();
        }
    }
}

//----------------------------------------------------
void GribReaderTop::interpolateMissingRecords ( bool mustInterpolate )
{
    mustInterpolateMissingRecords = mustInterpolate;
    if (isReaderOkT())
    {
        if (mustInterpolate) {
            gribReader->interpolateMissingRecords ();
        }
        else {
            gribReader->removeInterpolateRecords ();
        }
    }
}

//----------------------------------------------------
void GribReaderTop::setCurrentDate (time_t t)
{
    currentDate = t;
}
void GribReaderTop::setCurrentDateClosestFromNow ()
{
    if (!isReaderOkT())
        return;

    time_t date = gribReader->getClosestDateFromNow ();
    if (date > 0) {
        setCurrentDate (date);
    }
}
//------------------------------------------------------------
std::set<Altitude> GribReaderTop::getAllAltitudes (int dataType) const
{
    std::set<Altitude> empty;
    if (!isReaderOk())
        return empty;

    return gribRead->getAllAltitudes (dataType);
}
//------------------------------------------------------------
std::set<DataCode> GribReaderTop::getAllDataCode () const
{
    std::set<DataCode> empty;
    if (!isReaderOk())
        return empty;

    return gribRead->getAllDataCode ();
}



// Get current on date - dataDate

GribRecord* GribReaderTop::getCurrentXRecord(time_t dataDate) {

    Altitude currentAltitude;

    if (gribRead->hasData (DataCode(GRB_PRV_CUR_XY2D, LV_GND_SURF,0))) {
        currentAltitude = Altitude (LV_GND_SURF,0);
    } else if (gribRead->hasData (DataCode(GRB_PRV_CUR_XY2D, LV_BLW_SURF,1))) {
        currentAltitude = Altitude (LV_BLW_SURF,1);
    } else if (gribRead->hasData (DataCode(GRB_PRV_CUR_XY2D, LV_BLW_SURF,2))) {
        currentAltitude = Altitude (LV_BLW_SURF,2);
    } else if (gribRead->hasData (DataCode(GRB_PRV_CUR_XY2D, LV_BLW_SURF,3))) {
        currentAltitude = Altitude (LV_BLW_SURF,3);
    }
    else if (gribRead->hasData (DataCode(GRB_CUR_SPEED, LV_GND_SURF,0))
     && gribRead->hasData (DataCode(GRB_CUR_DIR, LV_GND_SURF,0)) )
    {
        currentAltitude = Altitude (LV_GND_SURF,0);
    }

    GribRecord *recx = gribRead->getRecord
                                (DataCode(GRB_CUR_VX,currentAltitude),dataDate);
    GribRecord *recy = gribRead->getRecord
                                (DataCode(GRB_CUR_VY,currentAltitude),dataDate);

    if (recx == NULL || recy == NULL)
        return NULL;

    return recx;
}

GribRecord* GribReaderTop::getCurrentSRecord(time_t dataDate) {

    bool flag = false;

    Altitude currentAltitude;

    if (gribRead->hasData (DataCode(GRB_PRV_CUR_XY2D, LV_GND_SURF,0))) {
        currentAltitude = Altitude (LV_GND_SURF,0);
    } else if (gribRead->hasData (DataCode(GRB_PRV_CUR_XY2D, LV_BLW_SURF,1))) {
        currentAltitude = Altitude (LV_BLW_SURF,1);
    } else if (gribRead->hasData (DataCode(GRB_PRV_CUR_XY2D, LV_BLW_SURF,2))) {
        currentAltitude = Altitude (LV_BLW_SURF,2);
    } else if (gribRead->hasData (DataCode(GRB_PRV_CUR_XY2D, LV_BLW_SURF,3))) {
        currentAltitude = Altitude (LV_BLW_SURF,3);
    }
    else if (gribRead->hasData (DataCode(GRB_CUR_SPEED, LV_GND_SURF,0))
     && gribRead->hasData (DataCode(GRB_CUR_DIR, LV_GND_SURF,0)) )
    {
        currentAltitude = Altitude (LV_GND_SURF,0);
    }

    GribRecord *recx = gribRead->getRecord
                                (DataCode(GRB_CUR_VX,currentAltitude),dataDate);
    GribRecord *recy = gribRead->getRecord
                                (DataCode(GRB_CUR_VY,currentAltitude),dataDate);

    if (recx == NULL || recy == NULL) {

        flag = true;
        recx = gribRead->getRecord
                           (DataCode(GRB_CUR_SPEED,currentAltitude),dataDate);
    }

    if (recx == NULL || recy == NULL)
        return NULL;

    return flag ? recx : NULL;
}

GribRecord* GribReaderTop::getCurrentYRecord (time_t dataDate) {

    Altitude currentAltitude;

    if (gribRead->hasData (DataCode(GRB_PRV_CUR_XY2D, LV_GND_SURF,0))) {
        currentAltitude = Altitude (LV_GND_SURF,0);
    } else if (gribRead->hasData (DataCode(GRB_PRV_CUR_XY2D, LV_BLW_SURF,1))) {
        currentAltitude = Altitude (LV_BLW_SURF,1);
    } else if (gribRead->hasData (DataCode(GRB_PRV_CUR_XY2D, LV_BLW_SURF,2))) {
        currentAltitude = Altitude (LV_BLW_SURF,2);
    } else if (gribRead->hasData (DataCode(GRB_PRV_CUR_XY2D, LV_BLW_SURF,3))) {
        currentAltitude = Altitude (LV_BLW_SURF,3);
    }
    else if (gribRead->hasData (DataCode(GRB_CUR_SPEED, LV_GND_SURF,0))
     && gribRead->hasData (DataCode(GRB_CUR_DIR, LV_GND_SURF,0)) )
    {
        currentAltitude = Altitude (LV_GND_SURF,0);
    }

    GribRecord *recx = gribRead->getRecord
                                (DataCode(GRB_CUR_VX,currentAltitude),dataDate);
    GribRecord *recy = gribRead->getRecord
                                (DataCode(GRB_CUR_VY,currentAltitude),dataDate);

    if (recx == NULL || recy == NULL)
        return NULL;
    return recy;
}

GribRecord* GribReaderTop::getCurrentDRecord (time_t dataDate) {
    bool flag = false;

    Altitude currentAltitude;

    if (gribRead->hasData (DataCode(GRB_PRV_CUR_XY2D, LV_GND_SURF,0))) {
        currentAltitude = Altitude (LV_GND_SURF,0);
    } else if (gribRead->hasData (DataCode(GRB_PRV_CUR_XY2D, LV_BLW_SURF,1))) {
        currentAltitude = Altitude (LV_BLW_SURF,1);
    } else if (gribRead->hasData (DataCode(GRB_PRV_CUR_XY2D, LV_BLW_SURF,2))) {
        currentAltitude = Altitude (LV_BLW_SURF,2);
    } else if (gribRead->hasData (DataCode(GRB_PRV_CUR_XY2D, LV_BLW_SURF,3))) {
        currentAltitude = Altitude (LV_BLW_SURF,3);
    }
    else if (gribRead->hasData (DataCode(GRB_CUR_SPEED, LV_GND_SURF,0))
     && gribRead->hasData (DataCode(GRB_CUR_DIR, LV_GND_SURF,0)) )
    {
        currentAltitude = Altitude (LV_GND_SURF,0);
    }

    GribRecord *recx = gribRead->getRecord
                                (DataCode(GRB_CUR_VX,currentAltitude),dataDate);
    GribRecord *recy = gribRead->getRecord
                                (DataCode(GRB_CUR_VY,currentAltitude),dataDate);

    if (recx == NULL || recy == NULL) {

        flag = true;
        recx = gribRead->getRecord
                           (DataCode(GRB_CUR_SPEED,currentAltitude),dataDate);
        recy = gribRead->getRecord
                                (DataCode(GRB_CUR_DIR,currentAltitude),dataDate);
    }

    if (recx == NULL || recy == NULL)
        return NULL;
    return flag ? recy : NULL;
}

QVector<QVector<v_arrow>>* GribReaderTop::getCurrentGridRecord (time_t dataDate) {

    bool flag = true;
    GribRecord* recx = getCurrentXRecord(dataDate);
    GribRecord* recy = getCurrentYRecord(dataDate);

    if (recx == NULL || recy == NULL)
    {
        bool flag = false;
        recx = getCurrentSRecord(dataDate);
        recy = getCurrentDRecord(dataDate);
    }

    if (recx == NULL || recy == NULL)
        return NULL;

    v_arrow zeroPair;
    zeroPair.first = 0.0;
    zeroPair.second = 0.0;
    QVector<QVector<v_arrow>>* currentVdirAtXY = new QVector<QVector<v_arrow>>(recx->getNi(), QVector<v_arrow>(recx->getNj(), zeroPair));

    if (flag)
    {
        int Nj = recx->getNj();
        for (int j = 0; j < Nj; j++)
        {
            int Ni = recx->getNi();
            for (int i = 0; i < Ni; i++)
            {
                data_t valueAtX = recx->getValueOnRegularGrid(i,j);

                if(valueAtX != GRIB_NOTDEF)
                    (*currentVdirAtXY)[i][j].first = valueAtX;
                else {
                    double ii = i;
                    double jj = j;
                    valueAtX = recx->getInterpolatedValue(ii, jj);
                    (*currentVdirAtXY)[i][j].first = valueAtX;
                }

                data_t valueAtY = recy->getValueOnRegularGrid(i,j);

                if(valueAtY != GRIB_NOTDEF)
                    (*currentVdirAtXY)[i][j].second = valueAtY;
                else {
                    double ii = i;
                    double jj = j;
                    valueAtY = recy->getInterpolatedValue(ii, jj);
                    (*currentVdirAtXY)[i][j].second = valueAtY;
                }
            }
        }
    }
    else
    {
        int Nj = recy->getNj();
        for (int j = 0; j < Nj; j++)
        {
            int Ni = recx->getNi();
            for (int i = 0; i < Ni; i++)
            {
                data_t valueAtY = recy->getValueOnRegularGrid(i,j);
                data_t valueAtX = recx->getValueOnRegularGrid(i,j);

                if(valueAtY != GRIB_NOTDEF || valueAtX != GRIB_NOTDEF)
                {
                    double ang = valueAtY*180.0/M_PI;
                    (*currentVdirAtXY)[i][j].second = valueAtX*sin(ang);
                    (*currentVdirAtXY)[i][j].first = valueAtX*cos(ang);
                }
                else {
                    double ii = i;
                    double jj = j;
                    valueAtX = recx->getInterpolatedValue(ii, jj);
                    valueAtY = recy->getInterpolatedValue(ii, jj);
                    double ang = valueAtY*180.0/M_PI;
                    (*currentVdirAtXY)[i][j].second = valueAtX*sin(ang);
                    (*currentVdirAtXY)[i][j].first = valueAtX*cos(ang);
                }
            }
        }
    }

    return currentVdirAtXY;
}




// Get wind on date - dataDate

GribRecord* GribReaderTop::getWindXRecord(time_t dataDate) {

    Altitude windAltitude;

    if (gribRead->hasData (DataCode(GRB_PRV_WIND_XY2D,LV_ABOV_GND,10))) {
        windAltitude = Altitude (LV_ABOV_GND,10);
    }
    else if (gribRead->hasData (DataCode(GRB_PRV_WIND_XY2D,LV_ABOV_GND,3))) {
        windAltitude = Altitude (LV_ABOV_GND,3);
    }
    else if (gribRead->hasData (DataCode(GRB_PRV_WIND_XY2D,LV_ABOV_GND,2))) {
        windAltitude = Altitude (LV_ABOV_GND,2);
    }
    else if (gribRead->hasData (DataCode(GRB_PRV_WIND_XY2D,LV_ABOV_GND,1))) {
        windAltitude = Altitude (LV_ABOV_GND,1);
    }
    else if (gribRead->hasData (DataCode(GRB_PRV_WIND_XY2D,LV_GND_SURF,0))) {
        windAltitude = Altitude (LV_GND_SURF,0);
    }
    else if (gribRead->hasData (DataCode(GRB_WIND_SPEED,LV_ABOV_GND,10))
     && gribRead->hasData (DataCode(GRB_WIND_DIR,LV_ABOV_GND,10)) )
    {
        windAltitude = Altitude (LV_ABOV_GND,10);
    }

    GribRecord *recx = gribRead->getRecord
                                (DataCode(GRB_WIND_VX,windAltitude),dataDate);
    GribRecord *recy = gribRead->getRecord
                                (DataCode(GRB_WIND_VY,windAltitude),dataDate);

    if (recx == NULL || recy == NULL)
        return NULL;

    return recx;
}

GribRecord* GribReaderTop::getWindSRecord(time_t dataDate) {

    bool flag = false;

    Altitude windAltitude;

    if (gribRead->hasData (DataCode(GRB_PRV_WIND_XY2D,LV_ABOV_GND,10))) {
        windAltitude = Altitude (LV_ABOV_GND,10);
    }
    else if (gribRead->hasData (DataCode(GRB_PRV_WIND_XY2D,LV_ABOV_GND,3))) {
        windAltitude = Altitude (LV_ABOV_GND,3);
    }
    else if (gribRead->hasData (DataCode(GRB_PRV_WIND_XY2D,LV_ABOV_GND,2))) {
        windAltitude = Altitude (LV_ABOV_GND,2);
    }
    else if (gribRead->hasData (DataCode(GRB_PRV_WIND_XY2D,LV_ABOV_GND,1))) {
        windAltitude = Altitude (LV_ABOV_GND,1);
    }
    else if (gribRead->hasData (DataCode(GRB_PRV_WIND_XY2D,LV_GND_SURF,0))) {
        windAltitude = Altitude (LV_GND_SURF,0);
    }
    else if (gribRead->hasData (DataCode(GRB_WIND_SPEED,LV_ABOV_GND,10))
     && gribRead->hasData (DataCode(GRB_WIND_DIR,LV_ABOV_GND,10)) )
    {
        windAltitude = Altitude (LV_ABOV_GND,10);
    }

    GribRecord *recx = gribRead->getRecord
                                (DataCode(GRB_WIND_VX,windAltitude),dataDate);
    GribRecord *recy = gribRead->getRecord
                                (DataCode(GRB_WIND_VY,windAltitude),dataDate);

    if (recx == NULL || recy == NULL) {

        flag = true;
        recx = gribRead->getRecord
                           (DataCode(GRB_WIND_SPEED,windAltitude),dataDate);
    }

    if (recx == NULL || recy == NULL)
        return NULL;

    return flag ? recx : NULL;
}

GribRecord* GribReaderTop::getWindYRecord (time_t dataDate) {

    Altitude windAltitude;

    if (gribRead->hasData (DataCode(GRB_PRV_WIND_XY2D,LV_ABOV_GND,10))) {
        windAltitude = Altitude (LV_ABOV_GND,10);
    }
    else if (gribRead->hasData (DataCode(GRB_PRV_WIND_XY2D,LV_ABOV_GND,3))) {
        windAltitude = Altitude (LV_ABOV_GND,3);
    }
    else if (gribRead->hasData (DataCode(GRB_PRV_WIND_XY2D,LV_ABOV_GND,2))) {
        windAltitude = Altitude (LV_ABOV_GND,2);
    }
    else if (gribRead->hasData (DataCode(GRB_PRV_WIND_XY2D,LV_ABOV_GND,1))) {
        windAltitude = Altitude (LV_ABOV_GND,1);
    }
    else if (gribRead->hasData (DataCode(GRB_PRV_WIND_XY2D,LV_GND_SURF,0))) {
        windAltitude = Altitude (LV_GND_SURF,0);
    }
    else if (gribRead->hasData (DataCode(GRB_WIND_SPEED,LV_ABOV_GND,10))
     && gribRead->hasData (DataCode(GRB_WIND_DIR,LV_ABOV_GND,10)) )
    {
        windAltitude = Altitude (LV_ABOV_GND,10);
    }

    GribRecord *recx = gribRead->getRecord
                                (DataCode(GRB_WIND_VX,windAltitude),dataDate);
    GribRecord *recy = gribRead->getRecord
                                (DataCode(GRB_WIND_VY,windAltitude),dataDate);

    if (recx == NULL || recy == NULL)
        return NULL;
    return recy;
}

GribRecord* GribReaderTop::getWindDRecord (time_t dataDate) {
    bool flag = false;

    Altitude windAltitude;

    if (gribRead->hasData (DataCode(GRB_PRV_WIND_XY2D,LV_ABOV_GND,10))) {
        windAltitude = Altitude (LV_ABOV_GND,10);
    }
    else if (gribRead->hasData (DataCode(GRB_PRV_WIND_XY2D,LV_ABOV_GND,3))) {
        windAltitude = Altitude (LV_ABOV_GND,3);
    }
    else if (gribRead->hasData (DataCode(GRB_PRV_WIND_XY2D,LV_ABOV_GND,2))) {
        windAltitude = Altitude (LV_ABOV_GND,2);
    }
    else if (gribRead->hasData (DataCode(GRB_PRV_WIND_XY2D,LV_ABOV_GND,1))) {
        windAltitude = Altitude (LV_ABOV_GND,1);
    }
    else if (gribRead->hasData (DataCode(GRB_PRV_WIND_XY2D,LV_GND_SURF,0))) {
        windAltitude = Altitude (LV_GND_SURF,0);
    }
    else if (gribRead->hasData (DataCode(GRB_WIND_SPEED,LV_ABOV_GND,10))
     && gribRead->hasData (DataCode(GRB_WIND_DIR,LV_ABOV_GND,10)) )
    {
        windAltitude = Altitude (LV_ABOV_GND,10);
    }

    GribRecord *recx = gribRead->getRecord
                                (DataCode(GRB_WIND_VX,windAltitude),dataDate);
    GribRecord *recy = gribRead->getRecord
                                (DataCode(GRB_WIND_VY,windAltitude),dataDate);

    if (recx == NULL || recy == NULL) {

        flag = true;
        recx = gribRead->getRecord
                           (DataCode(GRB_WIND_SPEED,windAltitude),dataDate);
        recy = gribRead->getRecord
                                (DataCode(GRB_WIND_DIR,windAltitude),dataDate);
    }

    if (recx == NULL || recy == NULL)
        return NULL;
    return flag ? recy : NULL;
}

QVector<QVector<v_arrow>>* GribReaderTop::getWindGridRecord (time_t dataDate) {

    bool flag = true;
    GribRecord* recx = getWindXRecord(dataDate);
    GribRecord* recy = getWindYRecord(dataDate);

    if (recx == NULL || recy == NULL)
    {
        bool flag = false;
        recx = getWindSRecord(dataDate);
        recy = getWindDRecord(dataDate);
    }

    if (recx == NULL || recy == NULL)
        return NULL;

    v_arrow zeroPair;
    zeroPair.first = 0.0;
    zeroPair.second = 0.0;
    QVector<QVector<v_arrow>>* windVdirAtXY = new QVector<QVector<v_arrow>>(recx->getNi(), QVector<v_arrow>(recx->getNj(), zeroPair));

    if (flag)
    {
        int Nj = recx->getNj();
        for (int j = 0; j < Nj; j++)
        {
            int Ni = recx->getNi();
            for (int i = 0; i < Ni; i++)
            {
                data_t valueAtX = recx->getValueOnRegularGrid(i,j);

                if(valueAtX != GRIB_NOTDEF)
                    (*windVdirAtXY)[i][j].first = -valueAtX;
                else {
                    double ii = i;
                    double jj = j;
                    valueAtX = recx->getInterpolatedValue(ii, jj);
                    (*windVdirAtXY)[i][j].first = -valueAtX;
                }

                data_t valueAtY = recy->getValueOnRegularGrid(i,j);

                if(valueAtY != GRIB_NOTDEF)
                    (*windVdirAtXY)[i][j].second = valueAtY;
                else {
                    double ii = i;
                    double jj = j;
                    valueAtY = recy->getInterpolatedValue(ii, jj);
                    (*windVdirAtXY)[i][j].second = valueAtY;
                }
            }
        }
    }
    else
    {
        int Nj = recy->getNj();
        for (int j = 0; j < Nj; j++)
        {
            int Ni = recx->getNi();
            for (int i = 0; i < Ni; i++)
            {
                data_t valueAtY = recy->getValueOnRegularGrid(i,j);
                data_t valueAtX = recx->getValueOnRegularGrid(i,j);

                if(valueAtY != GRIB_NOTDEF || valueAtX != GRIB_NOTDEF)
                {
                    double ang = valueAtY*180.0/M_PI;
                    (*windVdirAtXY)[i][j].second = valueAtX*sin(ang);
                    (*windVdirAtXY)[i][j].first = valueAtX*cos(ang);
                }
                else {
                    double ii = i;
                    double jj = j;
                    valueAtX = recx->getInterpolatedValue(ii, jj);
                    valueAtY = recy->getInterpolatedValue(ii, jj);
                    double ang = valueAtY*180.0/M_PI;
                    (*windVdirAtXY)[i][j].second = valueAtX*sin(ang);
                    (*windVdirAtXY)[i][j].first = valueAtX*cos(ang);
                }
            }
        }
    }

    return windVdirAtXY;
}
