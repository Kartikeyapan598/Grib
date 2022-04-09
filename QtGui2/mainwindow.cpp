#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Utils.h"
#include <time.h>
#include <QApplication>

MainWindow* MainWindow::m_instance;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    m_instance = this;

    ui->setupUi(this);
    CustomScene* m_scene = new CustomScene();
    createDockView(m_scene);
    addDockWidget(Qt::LeftDockWidgetArea, m_pDockWidget2);

    // // //
    gribReaderTop = new GribReaderTop (this);
    assert(gribReaderTop);
    gribReaderTop->hide();
    // // //

    // Select various rendering options like temp, weather, wind etc
    m_rendertypes = new RenderTypes(this);
    qApp->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createDockView(CustomScene* scene)
{
    m_pDockWidget2 = new QDockWidget(this);
    m_pDockWidget2->setWindowTitle("dock type");
    m_pDockWidget2->setAllowedAreas(Qt::AllDockWidgetAreas);
    m_pForm = new Form(m_pDockWidget2, scene);
    m_pForm->SetGraphicsScene();
    m_pDockWidget2->setWidget(m_pForm);
    this->setCentralWidget(m_pDockWidget2);
}

void MainWindow::wheelEvent(QWheelEvent* event)
{
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionQt_triggered()
{
    QApplication::aboutQt();
}

void MainWindow::on_actionDrawSquare_triggered()
{
    // To Do Add Rendering of Squares in the viewport
    Square* item = new Square();
    sq.push_back(item);
    item->setPos(randomBetween(-600, 200), randomBetween(-300, 200));
    CustomScene* scene = m_pForm->GetGraphicsScene();
    scene->addItem(item);
}

void MainWindow::on_actionDraw_Circle_triggered()
{
    Points* point = new Points();
    m_wayPoints.push_back(point);
    point->setPos(randomBetween(-600, 200), randomBetween(-300, 200));
    CustomScene* scene = m_pForm->GetGraphicsScene();
    scene->addItem(point);
}

void MainWindow::UpdateRenders()
{
    // Update ViewPort rendering and rendering of Wind, Temp, Weather
}

void MainWindow::on_actionViewPort_triggered()
{
    // Hide or Show viewport on Trigger
    m_pDockWidget2->addAction(m_pDockWidget2->toggleViewAction());
}

void MainWindow::on_actionRenderTypes_triggered()
{
    m_rendertypes->show();
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "FileName", path.curr_dir, path.filter);
    // // //
    if (fileName != "") {
        QFileInfo finfo(fileName);
        gribFilePath = finfo.absolutePath();
        openMeteoDataFile (fileName);
    }
    // // //
    is_loaded = true;
    is_saved = true;
}


void MainWindow::on_actionClose_triggered()
{
    if(is_loaded == true)
    {
        // To do Unload the grib/grib2 file
        is_loaded = false;
    }
}


void MainWindow::on_actionSave_triggered()
{
    if(is_saved == false && is_loaded == true)
    {
        // To do Save the file
        is_saved = true;
    }
}


void MainWindow::on_actionAdd_WayPoints_triggered()
{
    if(m_wayPoints.size() <= 1)
    {
        CreateMessageBoxInfo(this, "WayPoints", "Not enough Points to make Paths");
    }
    if(m_wayPoints.size() > 1)
    {
        CustomScene* scene = m_pForm->GetGraphicsScene();
        for(int i = 0; i < m_wayPoints.size() - 1; i++)
        {
            QLineF line(m_wayPoints[i]->pos(), m_wayPoints[i + 1]->pos());
            m_line.push_back(line);
            scene->addLine(line);
        }
    }
}


void MainWindow::on_actionClear_scene_triggered()
{
    m_pForm->clearScene();
}

// // //
void MainWindow::openMeteoDataFile (const QString& fileName)
{
    FileDataType meteoFileType = DATATYPE_NONE;

    if (QFile::exists(fileName)) {
        meteoFileType = gribReaderTop->loadMeteoDataFile (fileName);

        if (meteoFileType != DATATYPE_NONE)
            Util::setSetting("gribFileName",  fileName);
    }

    if (gribReaderTop!=nullptr && gribReaderTop->isReaderOk()) {
        if (meteoFileType == DATATYPE_GRIB) {

            listGribDates.clear();
            std::set<time_t>* setDates = gribReaderTop->getListDates();

            if (setDates) for (long setDate : *setDates) {
                listGribDates.push_back(setDate);
            }

            currentDate = gribReaderTop->getCurrentDate();

            // Malformed grib file ?
            GriddedReader *reader = gribReaderTop->getReader();
            if (reader->hasAmbiguousHeader()) {
                QMessageBox::warning (this,
                tr("Warning"),
                tr("File :") + fileName
                + "\n\n"
                + tr("The header of this GRIB file do not respect standard format.")
                + "\n\n"
                + tr("Despite efforts to interpret it, output may be incorrect.")
                + "\n\n"
                + tr("Please inform the supplier of this file that the GDS section of "
                   "the file header is ambiguous, particularly about data position.")
                + "\n"
                );
            }
        }
       showGribInfo ();
    }

    else {
        if (meteoFileType != DATATYPE_CANCELLED) {
            QMessageBox::critical (this,
                tr("Error"),
                tr("File :") + fileName + "\n\n"
                    + tr("Can't open file.") + "\n\n"
                    + tr("It's not a GRIB file,") + "\n"
                    + tr("or it contains unrecognized data,") + "\n"
                    + tr("or...")
            );
        }
    }
}

void MainWindow::showGribInfo () {

    if (!gribReaderTop->isReaderOk())    {
        QMessageBox::information (this,
            ("File information"),
            ("File not loaded."));
        return;
    }
    GriddedReader *reader = gribReaderTop->getReader();
    GriddedRecord *record = reader->getFirstRecord();
    if (!record)    {
        QMessageBox::information (this,
            ("File information"),
            ("Data error."));
        return;
    }
    QString msg;

    msg += QString("File : %1\n") .arg(reader->getFileName());
    msg += QString("Size : %1 bytes\n") .arg(reader->getFileSize());
    msg += "\n";
    msg += QString("Weather center id : %1\n") .arg(record->getIdCenter());
    msg += +" - "+ QString("Model id : %1\n") .arg(record->getIdModel());
    msg += +" - "+ QString("Grid id : %1\n") .arg(record->getIdGrid());
    msg += "\n";

    //msg += tr("%1 enregistrements, ").arg(record->getTotalNumberOfGribRecords());
    msg += QString("Total no. of dates : %1\n").arg(reader->getNumberOfDates());

    std::set<time_t> sdates = reader->getListDates();
    msg += QString("dates.begin() : %1\n").arg( Util::formatDateTimeLong(*(sdates.begin())) );
    msg += QString("dates.end() : %1\n").arg( Util::formatDateTimeLong(*(sdates.rbegin())) );

    msg += "\n";
    msg += "Available data :";
    QString dt;
    std::set<DataCode> setdata = gribReaderTop->getAllDataCode ();
    int  currentype = -1;
    bool firstalt = true;
    for (auto dtc : setdata) {
            if (   dtc.dataType != GRIB_NOTDEF
            && dtc.dataType != GRB_WIND_VY
            && dtc.dataType != GRB_CUR_VY
        ) {
            DataCode code = dtc;
            if (dtc.dataType == GRB_WIND_VX)
                    code.dataType = GRB_PRV_WIND_XY2D;
            if (dtc.dataType == GRB_CUR_VX)
                    code.dataType = GRB_PRV_CUR_XY2D;
            if (currentype != dtc.dataType) {
                dt = "\n "+DataCodeStr::toString_name (code);
                msg += dt;
                currentype = dtc.dataType;
                firstalt = true;
            }
            if (!firstalt)
                msg += ", ";
            else
                firstalt = false;
            msg += " altitude : " + AltitudeStr::toStringShort (code.getAltitude());
        }
    }

    msg += "\n";
    msg += "\n";
    if (record->isRegularGrid()) {
        msg += QString("Grid : \nNi = %1, Nj = %2 Ni*Nj = %3 points")
                    .arg(record->getNi())
                    .arg(record->getNj())
                    .arg(record->getNi()*record->getNj());
        msg += QString("\n(xMin, yMin) : (%1, %2)\n(xMax, yMax) : (%3, %4)")
                .arg(record->getXmin())
                .arg(record->getYmin())
                .arg(record->getXmax())
                .arg(record->getYmax());
        msg += QString("\nDi = %1°\nDj = %2°")
                    .arg(record->getDeltaX()).arg(record->getDeltaY());
        msg += "\n";
    }
    else {
        msg += QString("Grid : %1 points")
                    .arg(record->getTotalNumberOfPoints());
        double resapprox = 1.0/sqrt(record->getAveragePointsDensity());
        msg += QString("  ≈ %1°x%2°")
                    .arg( resapprox, 0,'g',2 )
                    .arg( resapprox, 0,'g',2 );
        msg += "\n";
    }

    msg += "\n";
    msg += QString("Reference date: %1\n\n")
                    .arg(Util::formatDateTimeLong(reader->getFirstDate()));


    textBox = new box(this);
    int Ni = record->getNi();
    int Nj = record->getNj();

    GribRecord *rec = gribReaderTop->getReader()->getFirstGribRecord();

    QString dateWiseGrid = windData(); // or currentData();

    msg += dateWiseGrid;
    textBox->gribInfo->append(msg);
    textBox->gribInfo->setLineWrapMode(QTextEdit::NoWrap);
    textBox->show();

}

QString MainWindow::windData() {

    QString dateWiseGrid = "";
    QString valueIJ = "";
    std::set<time_t> sdates = gribReaderTop->getReader()->getListDates();
    std::set<time_t>::iterator date_i;

    for (date_i = sdates.begin(); date_i != sdates.end(); date_i++)
    {
        dateWiseGrid += Util::formatDateTimeLong(*date_i) + " : \n\n";
        QVector<QVector<v_arrow>>* wind = gribReaderTop->getWindGridRecord(*date_i);

        if (wind == NULL)
            continue;

        int Ni = (*wind).size();
        int Nj = (*wind)[0].size();
        for (int j = 0; j < Nj; j++)
        {
            for (int i = 0; i < Ni; i++)
            {
                valueIJ = "(" + QString::number((*wind)[i][j].first)
                            + ", " + QString::number((*wind)[i][j].second) + ")";
                dateWiseGrid += valueIJ + " ";
            }
            dateWiseGrid += "\n";
        }
        dateWiseGrid += "\n\n";
    }
    return dateWiseGrid;
}

QString MainWindow::currentData() {

    QString dateWiseGrid = "";
    QString valueIJ = "";
    std::set<time_t> sdates = gribReaderTop->getReader()->getListDates();
    std::set<time_t>::iterator date_i;

    for (date_i = sdates.begin(); date_i != sdates.end(); date_i++)
    {
        dateWiseGrid += Util::formatDateTimeLong(*date_i) + " : \n\n";
        QVector<QVector<v_arrow>>* current = gribReaderTop->getCurrentGridRecord(*date_i);

        if (current == NULL)
            continue;

        int Ni = (*current).size();
        int Nj = (*current)[0].size();
        for (int j = 0; j < Nj; j++)
        {
            for (int i = 0; i < Ni; i++)
            {
                valueIJ = "(" + QString::number((*current)[i][j].first)
                            + ", " + QString::number((*current)[i][j].second) + ")";
                dateWiseGrid += valueIJ + " ";
            }
            dateWiseGrid += "\n";
        }
        dateWiseGrid += "\n\n";
    }
    return dateWiseGrid;
}
// // //
