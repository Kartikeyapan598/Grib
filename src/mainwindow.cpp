#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Utils.h"

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

    isGridDrawn = false;

    isWindSpeedOnScene = false;
    areWindArrowsOnScene = false;
    isWaveSigHtOnScene = false;
    isWindWaveHtOnScene = false;
    areWindWaveArrowsOnScene = false;
    isSwellWaveHtOnScene = false;
    areSwellWaveArrowsOnScene = false;
    areCurrentArrowsOnScene = false;
    areIsobarsOnScene = false;

    m_fcPtr = 0;

    // Select various rendering options like temp, weather, wind etc
    m_rendertypes = new RenderTypes(this);
    m_rendertypes->setDisabled(true);

    m_infoPanel = new InfoPanel(this);
    m_infoPanel->setDisabled(true);

    qApp->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createDockView(CustomScene* scene)
{
    m_pDockWidget2 = new QDockWidget(this);
    m_pDockWidget2->setWindowFlag(Qt::FramelessWindowHint);
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

void MainWindow::UpdateRenders()
{
    // Update ViewPort rendering and rendering of Wind, Temp, Weather
}

void MainWindow::on_actionRenderTypes_triggered()
{
    m_rendertypes->show();
}

void MainWindow::on_actionOpen_triggered()
{
    fileName = "";
    fileName = QFileDialog::getOpenFileName(this, "FileName", path.curr_dir, path.filter);

/* Save old gridData to local directory
--
--
--
*/
    if (fileName != "")
    {
        gridData = new GridData(fileName.toStdString().c_str());

        for (int fc = 0; fc < gridData->getNbForecasts(); ++fc)
        {
            Gridder* gridder = new Gridder(fc);
            Plotter* plotter = new Plotter(gridder);
            m_plotters.push_back(plotter);
            m_plotters.back()->setScene(m_pForm->GetGraphicsScene());
        }

        drawGrid();

        m_infoPanel->setRefDate(gridData->getRefDate());

        m_plotters.front()->addWindColorPlot();
        isWindSpeedOnScene = true;

        m_plotters.front()->addWindArrowPlot();
        areWindArrowsOnScene = true;

        m_rendertypes->setDisabled(false);
        m_infoPanel->show();
        m_infoPanel->setDisabled(false);


        is_loaded = true;
        is_saved = true;
    }
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


void MainWindow::on_actionDownLoad_File_triggered()
{
    //Crashing Why?
    //m_dataManager->append(QStringList("https://www.win-rar.com/fileadmin/winrar-versions/winrar/winrar-x64-611.exe"));
    DownloadManager* manager = new DownloadManager(NULL);
    QStringList list;
    list.append(QString("https://speed.hetzner.de/100MB.bin"));
    manager->append(list);
    return;
}


void MainWindow::on_actionUpload_CSV_triggered()
{
    // save the csv as original csv
    QString fName = QFileDialog::getOpenFileName(this, "FileName", path.curr_dir, path.filter);

    // Initialize route
    routes = new CsvRoutes(fName);

    addCsvRoutePlot();

    /* Upload to back-end
     *
     *
     *
     *
     */
}



void MainWindow::addCsvRoutePlot()
{
    CustomScene* scene = m_pForm->GetGraphicsScene();

    float stepi = scene->stepI();
    float stepj = scene->stepJ();

    std::vector<std::pair<float, float>> route = routes->getOrigCsvRoute();

    QPen wp_border = QPen(Qt::red, 3);
    wp_border.setCosmetic(true);

    QPen wp_line = QPen(Qt::darkRed, 3);
    wp_line.setCosmetic(true);
    wp_line.setCapStyle(Qt::RoundCap);

    QPointF prev = QPointF(route.at(0).first, route.at(0).second);

    for (size_t i = 0; i < route.size(); i++)
    {
        float x = route.at(i).first * stepi;
        float y = -toMerc(route.at(i).second) * stepj;

        // waypoint
        double rad = 0.3;
        QGraphicsEllipseItem* el = scene->addEllipse(x - rad, y - rad, rad*2.0, rad*2.0, wp_border, QBrush(Qt::SolidPattern));
        el->setZValue(std::numeric_limits<qreal>::max() - 2);                       // below label


        // line

        if (i > 0)
        {
            float prev_x = prev.x() * stepi;
            float prev_y = -toMerc(prev.y()) * stepj;

            QGraphicsLineItem* l = scene->addLine(prev_x, prev_y, x, y, wp_line);
            l->setZValue(std::numeric_limits<qreal>::max() - 1);

            prev = QPointF(route.at(i).first, route.at(i).second);
        }


        // waypoint number

        QGraphicsTextItem* label = new QGraphicsTextItem(QString::number(i + 1));
        label->setDefaultTextColor(Qt::white);

        QFont f = label->font();
        f.setPointSizeF(0.4);
        label->setFont(f);

        QRectF textRect = label->boundingRect();

        if (i % 2 == 0) {
            label->setPos(x - 5 * textRect.width() / 8, y - textRect.height() / 2);
        }
        else {
            label->setPos(x - 3 * textRect.width() / 8, y - textRect.height() / 2);
        }

        label->setZValue(std::numeric_limits<qreal>::max());                    // above waypoint
        scene->addItem(label);
    }
}


void MainWindow::drawGrid()
{
    if (!isGridDrawn)
    {
        double Di = gridData->getD();
        double Dj = gridData->getD();
        int xMin = gridData->getMinPoint().x();
        int yMin = gridData->getMinPoint().y();
        int xMax = gridData->getMaxPoint().x();
        int yMax = gridData->getMaxPoint().y();

        CustomScene* scene = m_pForm->GetGraphicsScene();
        double stepi = scene->stepI();
        double stepj = scene->stepJ();

        scene->clearInitialGrid();

        if (Di >= 1)
        {
            float yMn_merc = toMerc(yMin - Dj);
            float yMx_merc = toMerc(yMax + Dj);

            QPen gridPen = QPen(QColor(30, 30, 30), 0.5);
            gridPen.setCosmetic(true);

            // Vertical Lines (longitudes)

            if (xMin < xMax)
            {
                for (int x = xMin - Di; x <= xMax; x += Di) {
                    scene->addLine(x*stepi, -yMx_merc*stepj, x*stepi, -yMn_merc*stepj, gridPen);
                }
                for (int y = yMin - Dj; y <= yMax + Dj; y += Dj) {
                    float y_merc = toMerc(y);
                    scene->addLine((xMin - Di)*stepi, -y_merc*stepj, (xMax + Dj)*stepi, -y_merc*stepj, gridPen);
                }
            }
            else
            {
                int skipped = 0;
                for (int x = xMin - Di; x <= 180; x += Di) {
                    scene->addLine(x*stepi, -yMx_merc*stepj, x*stepi, -yMn_merc*stepj, gridPen);
                    if (x + Di > 180) {
                        skipped = 180 - x;
                    }
                }
                int skip = skipped;
                if (skipped) {
                    skip = Di - 1 - skipped;
                }
                for (int x = -179 + skip; x <= xMax + Di; x += Di) {
                    scene->addLine(x*stepi, -yMx_merc*stepj, x*stepi, -yMn_merc*stepj, gridPen);
                }

                for (int y = yMin - Dj; y <= yMax + Dj; y += Dj) {
                    float y_merc = toMerc(y);
                    scene->addLine((xMin - Di)*stepi, -y_merc*stepj, (180)*stepi, -y_merc*stepj, gridPen);
                    scene->addLine((-179 + skip)*stepi, -y_merc*stepj, (xMax + Di)*stepi, -y_merc*stepj, gridPen);
                }
            }
        }
        else
        {
            qInfo() << "Application not configured for resolution < 1";
            return;
        }
        isGridDrawn = true;
    }
}

void MainWindow::updateElementsOnScene(int prevFc) // for updated m_fcPtr
{
    if (isWindSpeedOnScene)
    {
        m_plotters.at(prevFc)->removeWindColorPlot();
        m_plotters.at(m_fcPtr)->addWindColorPlot();
    }

    if (areWindArrowsOnScene)
    {
        m_plotters.at(prevFc)->removeWindArrowPlot();
        m_plotters.at(m_fcPtr)->addWindArrowPlot();
    }

    if (isWaveSigHtOnScene)
    {
        m_plotters.at(prevFc)->removeWaveSigHtColorPlot();
        m_plotters.at(m_fcPtr)->addWaveSigHtColorPlot();
    }

    if (areWindWaveArrowsOnScene)
    {
        m_plotters.at(prevFc)->removeWindWaveArrowPlot();
        m_plotters.at(m_fcPtr)->addWindWaveArrowPlot();
    }

    if (isWindWaveHtOnScene)
    {
        m_plotters.at(prevFc)->removeWindWaveHtColorPlot();
        m_plotters.at(m_fcPtr)->addWindWaveHtColorPlot();
    }

    if (isSwellWaveHtOnScene)
    {
        m_plotters.at(prevFc)->removeSwellWaveHtColorPlot();
        m_plotters.at(m_fcPtr)->addSwellWaveHtColorPlot();
    }

    if (areSwellWaveArrowsOnScene)
    {
        m_plotters.at(prevFc)->removeSwellWaveArrowPlot();
        m_plotters.at(m_fcPtr)->addSwellWaveArrowPlot();
    }

    if (areCurrentArrowsOnScene)
    {
        m_plotters.at(prevFc)->removeCurrentArrowPlot();
        m_plotters.at(m_fcPtr)->addCurrentArrowPlot();
    }

    if (areIsobarsOnScene)
    {
        m_plotters.at(prevFc)->removeIsobarPlot();
        m_plotters.at(m_fcPtr)->addIsobarPlot();
    }
}

void MainWindow::on_actionInfo_Panel_triggered()
{
    m_infoPanel->show();
}

