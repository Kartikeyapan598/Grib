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

    isWindSpeedDrawn = false;
    areWindArrowsDrawn = false;
    isWaveSigHtDrawn = false;
    isWindWaveHtDrawn = false;
    areWindWaveArrowsDrawn = false;
    isSwellWaveHtDrawn = false;
    areSwellWaveArrowsDrawn = false;
    areCurrentArrowsDrawn = false;

    isInitWindSpeedDrawn = false;
    areInitWindArrowsDrawn = false;
    isInitWaveSigHtDrawn = false;
    isInitWindWaveHtDrawn = false;
    areInitWindWaveArrowsDrawn = false;
    isInitSwellWaveHtDrawn = false;
    areInitSwellWaveArrowsDrawn = false;
    areInitCurrentArrowsDrawn = false;

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
    QGraphicsScene* scene = m_pForm->GetGraphicsScene();
    scene->addItem(item);
}

void MainWindow::on_actionDraw_Circle_triggered()
{
    Points* point = new Points();
    m_wayPoints.push_back(point);
    point->setPos(randomBetween(-600, 200), randomBetween(-300, 200));
    QGraphicsScene* scene = m_pForm->GetGraphicsScene();
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
    fileName = QFileDialog::getOpenFileName(this, "FileName", path.curr_dir, path.filter);

/* Save old gridData to local directory
--
--
--
*/
    gridData = new GridData(fileName.toStdString().c_str());
    drawGrid();
//    addWindArrowPlot(0);
//    addWindColorPlot(0);
//    addWaveSigHtColorPlot(0);
    addCurrentArrowPlot(0);

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

//void MainWindow::on_actionDownLoad_File_triggered()
//{
//    //Crashing Why?
//    //m_dataManager->append(QStringList("https://www.win-rar.com/fileadmin/winrar-versions/winrar/winrar-x64-611.exe"));
//    DownloadManager* manager = new DownloadManager(NULL);
//    QStringList list;
//    list.append(QString("https://speed.hetzner.de/100MB.bin"));
//    manager->append(list);
//    return;
//}


void MainWindow::on_actionUpload_CSV_triggered()
{
    // save the csv as original csv
    QString fName = QFileDialog::getOpenFileName(this, "FileName", path.curr_dir, path.filter);

    // Initialize route


    /* Upload to back-end
     *
     *
     *
     *
     */
}

void MainWindow::addWindArrowPlot(int fc)
{
    CustomScene* scene = m_pForm->GetGraphicsScene();

    if (!areInitWindArrowsDrawn)
    {
        drawWindArrow(fc);
        areWindArrowsDrawn = true;
    }
    else if (!areWindArrowsDrawn)
    {
        for (auto it : windArrowPlot) {
            scene->addItem(it);
        }
        areWindArrowsDrawn = true;
    }

}

void MainWindow::addWindColorPlot(int fc)
{
    CustomScene* scene = m_pForm->GetGraphicsScene();

    if (!isInitWindSpeedDrawn)
    {
        drawWindColor(fc);
        isWindSpeedDrawn = true;
    }
    else if (!isWindSpeedDrawn)
    {
        int mapXmax = scene->xExtent();
        int mapYmax = scene->yExtent();

        windColorPlot->setOffset(-mapXmax, -mapYmax);
        windColorPlot->setPos(0, 0);
        windColorPlot->setZValue(-1);
        scene -> addItem(windColorPlot);
        isWindSpeedDrawn = true;
    }

}

void MainWindow::addWaveSigHtColorPlot(int fc)
{
    CustomScene* scene = m_pForm->GetGraphicsScene();

    if (!isInitWaveSigHtDrawn)
    {
        drawWaveSigHtColor(fc);
        isWaveSigHtDrawn = true;
    }
    else if (!isWaveSigHtDrawn)
    {
        int mapXmax = scene->xExtent();
        int mapYmax = scene->yExtent();

        waveSigHtColorPlot->setOffset(-mapXmax, -mapYmax);
        waveSigHtColorPlot->setPos(0, 0);
        waveSigHtColorPlot->setZValue(-1);
        scene -> addItem(waveSigHtColorPlot);
        isWaveSigHtDrawn = true;
    }

}

void MainWindow::addWindWaveHtColorPlot(int fc)
{
    CustomScene* scene = m_pForm->GetGraphicsScene();

    if (!isInitWindWaveHtDrawn)
    {
        drawWindWaveHtColor(fc);
        isWindWaveHtDrawn = true;
    }
    else if (!isWindWaveHtDrawn)
    {
        int mapXmax = scene->xExtent();
        int mapYmax = scene->yExtent();

        windWaveHtColorPlot->setOffset(-mapXmax, -mapYmax);
        windWaveHtColorPlot->setPos(0, 0);
        windWaveHtColorPlot->setZValue(-1);
        scene -> addItem(windWaveHtColorPlot);
        isWindWaveHtDrawn = true;
    }

}
void MainWindow::addWindWaveArrowPlot(int fc)
{
    CustomScene* scene = m_pForm->GetGraphicsScene();

    if (!areInitWindWaveArrowsDrawn)
    {
        drawWindWaveArrow(fc);
        areWindWaveArrowsDrawn = true;
    }
    else if (!areWindWaveArrowsDrawn)
    {
        for (auto it : windWaveArrowPlot) {
            scene->addItem(it);
        }
        areWindWaveArrowsDrawn = true;
    }

}

void MainWindow::addSwellWaveHtColorPlot(int fc)
{
    CustomScene* scene = m_pForm->GetGraphicsScene();

    if (!isInitSwellWaveHtDrawn)
    {
        drawSwellWaveHtColor(fc);
        isSwellWaveHtDrawn = true;
    }
    else if (!isSwellWaveHtDrawn)
    {
        int mapXmax = scene->xExtent();
        int mapYmax = scene->yExtent();

        swellWaveHtColorPlot->setOffset(-mapXmax, -mapYmax);
        swellWaveHtColorPlot->setPos(0, 0);
        swellWaveHtColorPlot->setZValue(-1);
        scene -> addItem(swellWaveHtColorPlot);
        isSwellWaveHtDrawn = true;
    }

}
void MainWindow::addSwellWaveArrowPlot(int fc)
{
    CustomScene* scene = m_pForm->GetGraphicsScene();

    if (!areInitSwellWaveArrowsDrawn)
    {
        drawSwellWaveArrow(fc);
        areSwellWaveArrowsDrawn = true;
    }
    else if (!areSwellWaveArrowsDrawn)
    {
        for (auto it : swellWaveArrowPlot) {
            scene->addItem(it);
        }
        areSwellWaveArrowsDrawn = true;
    }

}

void MainWindow::addCurrentArrowPlot(int fc)
{
    CustomScene* scene = m_pForm->GetGraphicsScene();

    if (!areInitCurrentArrowsDrawn)
    {
        drawCurrentArrow(fc);
        areCurrentArrowsDrawn = true;
    }
    else if (!areCurrentArrowsDrawn)
    {
        for (auto it : currentArrowPlot) {
            scene->addItem(it);
        }
        areCurrentArrowsDrawn = true;
    }
}


void MainWindow::removeWindArrowPlot(int fc)
{
    CustomScene* scene = m_pForm->GetGraphicsScene();

    if (areWindArrowsDrawn)
    {
        for (auto it : windArrowPlot) {
            scene->removeItem(it);
        }
        areWindArrowsDrawn = false;
    }

}

void MainWindow::removeWindColorPlot(int fc)
{
    CustomScene* scene = m_pForm->GetGraphicsScene();

    if (isWindSpeedDrawn)
    {
        scene->removeItem(windColorPlot);
        isWindSpeedDrawn = false;
    }

}

void MainWindow::removeWaveSigHtColorPlot(int fc)
{
    CustomScene* scene = m_pForm->GetGraphicsScene();

    if (isWaveSigHtDrawn)
    {
        scene->removeItem(waveSigHtColorPlot);
        isWaveSigHtDrawn = false;
    }

}

void MainWindow::removeWindWaveHtColorPlot(int fc)
{
    CustomScene* scene = m_pForm->GetGraphicsScene();

    if (isWindWaveHtDrawn)
    {
        scene->removeItem(windWaveHtColorPlot);
        isWindWaveHtDrawn = false;
    }

}

void MainWindow::removeSwellWaveHtColorPlot(int fc)
{
    CustomScene* scene = m_pForm->GetGraphicsScene();

    if (isSwellWaveHtDrawn)
    {
        scene->removeItem(swellWaveHtColorPlot);
        isSwellWaveHtDrawn = false;
    }

}

void MainWindow::removeWindWaveArrowPlot(int fc)
{
    CustomScene* scene = m_pForm->GetGraphicsScene();

    if (areWindWaveArrowsDrawn)
    {
        for (auto it : windWaveArrowPlot) {
            scene->removeItem(it);
        }
        areWindWaveArrowsDrawn = false;
    }

}

void MainWindow::removeSwellWaveArrowPlot(int fc)
{
    CustomScene* scene = m_pForm->GetGraphicsScene();

    if (areSwellWaveArrowsDrawn)
    {
        for (auto it : swellWaveArrowPlot) {
            scene->removeItem(it);
        }
        areSwellWaveArrowsDrawn = false;
    }

}

void MainWindow::removeCurrentArrowPlot(int fc)
{
    CustomScene* scene = m_pForm->GetGraphicsScene();

    if (areCurrentArrowsDrawn)
    {
        for (auto it : currentArrowPlot) {
            scene->removeItem(it);
        }
        areCurrentArrowsDrawn = false;
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
        isGridDrawn = true;
    }
}

void MainWindow::drawWindArrow(int fc)
{
    CustomScene* scene = m_pForm->GetGraphicsScene();
    vector<vector<double>> wSpeed = gridData->getWindSpeed(fc);
    vector<vector<int>> wDir = gridData->getWindDir(fc);

    if (wSpeed.size() == 0 || wDir.size() == 0) {
        return;
    }

    double Di = gridData->getD();
    double Dj = gridData->getD();
    int xMin = gridData->getMinPoint().x();
    int yMin = gridData->getMinPoint().y();
    int xMax = gridData->getMaxPoint().x();
    int yMax = gridData->getMaxPoint().y();
    int Ni = gridData->getNi();
    int Nj = gridData->getNj();

    double stepi = scene->stepI();
    double stepj = scene->stepJ();

    // arrow properties
    double arrowSize = stepj;

    QPen pen = QPen(Qt::black, 0.2);

    if (xMin < xMax)
    {
        int j = 0;
        for (int y = yMin; y <= yMax && j < Nj; y += Dj)
        {
            int i = 0;
            for (int x = xMin; x <= xMax && i < Ni; x += Di)
            {
                double vInKnots = 1.94384 * (wSpeed[i][j]);
                double ang = (wDir[i][j]) * M_PI / 180.0;  // ang of arrow with +ve x axis in scene's ref
                double dx = arrowSize * qCos(ang);                // proj of arrow on x axis (in scene's ref)
                double dy = arrowSize * qSin(ang);                // proj of arrow on y axis (in scene's ref)
                double xx = x * stepi;                            // starting coordinates of the arrow
                double yy = -toMerc(y) * stepj;

                // barbs
                int vApprox = (int)(std::floor(vInKnots));

                if (vApprox == 0)
                {
                    double rads = 1;
                    QGraphicsItem* windItem = scene -> addEllipse(xx - rads, yy - rads, rads*2.0, rads*2.0, pen);
                    windArrowPlot.push_back(windItem);

                    double rad = 2;
                    windItem = scene -> addEllipse(xx - rad, yy - rad, rad*2.0, rad*2.0, pen);
                    windArrowPlot.push_back(windItem);
                }
                else if (vApprox < 50)
                {
                    QGraphicsItem* windItem = scene -> addLine(xx, yy, xx + dx, yy + dy, pen);
                    windArrowPlot.push_back(windItem);

                    double d = arrowSize/6;
                    double bx = xx;
                    double by = yy;
                    int noOfSmallBarbs = (vApprox < 5) ? 1 : (vApprox/5) % 2;
                    int noOfBigBarbs = vApprox/10;

                    if (noOfBigBarbs == 0)
                    {
                        bx += d*qCos(ang);
                        by += d*qSin(ang);
                    }

                    for (int i = 0; i < noOfBigBarbs; i++)
                    {
                        QPointF be = barb(ang, arrowSize);

                        windItem = scene -> addLine(bx, by, bx + be.x(), by + be.y(), pen);
                        windArrowPlot.push_back(windItem);

                        bx += d*qCos(ang);
                        by += d*qSin(ang);
                    }

                    for (int i = 0; i < noOfSmallBarbs; i++)
                    {
                        QPointF be = barb(ang, arrowSize, true);

                        windItem = scene -> addLine(bx, by, bx + be.x(), by + be.y(), pen);
                        windArrowPlot.push_back(windItem);
                    }
                }
                else
                {
                    QGraphicsItem* windItem = scene -> addLine(xx, yy, xx + dx, yy + dy, pen);
                    windArrowPlot.push_back(windItem);

                    double d = arrowSize/6;
                    double bx = xx;
                    double by = yy;
                    int noOfSmallBarbs = (vApprox/5) % 2;
                    int noOfBigBarbs = (vApprox/10) % 5;
                    int noOfTriangles = vApprox/50;

                    for (int i = 0; i < noOfTriangles; i++)
                    {
                        QPair<QPointF, QPointF> p = triangle(ang, arrowSize);

                        windItem = scene->addLine(bx, by, bx + p.first.x(), by + p.first.y(), pen);
                        windArrowPlot.push_back(windItem);

                        windItem = scene->addLine(bx + p.second.x(), by + p.second.y(), bx + p.first.x(), by + p.first.y(), pen);
                        windArrowPlot.push_back(windItem);

                        bx = p.second.x();
                        by = p.second.y();
                    }

                    bx += d*qCos(ang);
                    by += d*qSin(ang);

                    for (int i = 0; i < noOfBigBarbs; i++)
                    {
                        QPointF be = barb(ang, arrowSize);

                        windItem = scene -> addLine(bx, by, bx + be.x(), by + be.y(), pen);
                        windArrowPlot.push_back(windItem);

                        bx += d*qCos(ang);
                        by += d*qSin(ang);
                    }

                    for (int i = 0; i < noOfSmallBarbs; i++)
                    {
                        QPointF be = barb(ang, arrowSize, true);

                        windItem = scene -> addLine(bx, by, bx + be.x(), by + be.y(), pen);
                        windArrowPlot.push_back(windItem);
                    }
                }
                i++;
            }
            j++;
        }
    }

    else
    {
        int j = 0;
        for (int y = yMin; y <= yMax && j < Nj; y+=Dj)
        {
            int i = 0;
            int skipped = 0;
            for (int x = xMin; x <= 180; x+=Di)
            {
                double vInKnots = 1.94384 * (wSpeed[i][j]);
                double ang = (wDir[i][j]) * M_PI / 180.0;   // angle of arrow with +ve x axis in scene's ref
                double dx = arrowSize*qCos(ang);         // projection of length of arrow on x - axis (in scene's ref)
                double dy = arrowSize*qSin(ang);         // projection of length of arrow on y - axis      ,,
                double xx = x*stepi;   // starting coordinates of the arrow
                double yy = -toMerc(y)*stepj;

                // barbs
                int vApprox = (int)(std::floor(vInKnots));

                if (vApprox == 0)
                {
                    double rads = 1;
                    QGraphicsItem* windItem = scene -> addEllipse(xx - rads, yy - rads, rads*2.0, rads*2.0, pen);
                    windArrowPlot.push_back(windItem);

                    double rad = 2;
                    windItem = scene -> addEllipse(xx - rad, yy - rad, rad*2.0, rad*2.0, pen);
                    windArrowPlot.push_back(windItem);
                }
                else if (vApprox < 50)
                {
                    QGraphicsItem* windItem = scene -> addLine(xx, yy, xx + dx, yy + dy, pen);
                    windArrowPlot.push_back(windItem);

                    double d = arrowSize/6;
                    double bx = xx;
                    double by = yy;
                    int noOfSmallBarbs = (vApprox < 5) ? 1 : (vApprox/5) % 2;
                    int noOfBigBarbs = vApprox/10;

                    if (noOfBigBarbs == 0)
                    {
                        bx += d*qCos(ang);
                        by += d*qSin(ang);
                    }

                    for (int i = 0; i < noOfBigBarbs; i++)
                    {
                        QPointF be = barb(ang, arrowSize);

                        windItem = scene -> addLine(bx, by, bx + be.x(), by + be.y(), pen);
                        windArrowPlot.push_back(windItem);

                        bx += d*qCos(ang);
                        by += d*qSin(ang);
                    }

                    for (int i = 0; i < noOfSmallBarbs; i++)
                    {
                        QPointF be = barb(ang, arrowSize, true);

                        windItem = scene -> addLine(bx, by, bx + be.x(), by + be.y(), pen);
                        windArrowPlot.push_back(windItem);
                    }
                }
                else
                {
                    QGraphicsItem* windItem = scene -> addLine(xx, yy, xx + dx, yy + dy, pen);
                    windArrowPlot.push_back(windItem);

                    double d = arrowSize/6;
                    double bx = xx;
                    double by = yy;
                    int noOfSmallBarbs = (vApprox/5) % 2;
                    int noOfBigBarbs = (vApprox/10) % 5;
                    int noOfTriangles = vApprox/50;

                    for (int i = 0; i < noOfTriangles; i++)
                    {
                        QPair<QPointF, QPointF> p = triangle(ang, arrowSize);

                        windItem = scene->addLine(bx, by, bx + p.first.x(), by + p.first.y(), pen);
                        windArrowPlot.push_back(windItem);

                        windItem = scene->addLine(bx + p.second.x(), by + p.second.y(), bx + p.first.x(), by + p.first.y(), pen);
                        windArrowPlot.push_back(windItem);

                        bx = p.second.x();
                        by = p.second.y();
                    }

                    bx += d*qCos(ang);
                    by += d*qSin(ang);

                    for (int i = 0; i < noOfBigBarbs; i++)
                    {
                        QPointF be = barb(ang, arrowSize);

                        windItem = scene -> addLine(bx, by, bx + be.x(), by + be.y(), pen);
                        windArrowPlot.push_back(windItem);

                        bx += d*qCos(ang);
                        by += d*qSin(ang);
                    }

                    for (int i = 0; i < noOfSmallBarbs; i++)
                    {
                        QPointF be = barb(ang, arrowSize, true);

                        windItem = scene -> addLine(bx, by, bx + be.x(), by + be.y(), pen);
                        windArrowPlot.push_back(windItem);
                    }
                }
                i++;
                if (x + Di > 180) {
                    skipped = 180 - x;
                }
            }

            int skip = Di - 1 - skipped;

            for (int x = -179 + skip; x <= xMax && i < Ni; x += Di)
            {
                double vInKnots = 1.94384 * (wSpeed[i][j]);
                double ang = (wDir[i][j]) * M_PI / 180.0;  // angle of arrow with +ve x axis in scene's ref
                double dx = arrowSize*qCos(ang);         // projection of length of arrow on x - axis (in scene's ref)
                double dy = arrowSize*qSin(ang);         // projection of length of arrow on y - axis      ,,
                double xx = x*stepi;   // starting coordinates of the arrow
                double yy = -toMerc(y)*stepj;

                // barbs
                int vApprox = (int)(std::floor(vInKnots));

                if (vApprox == 0)
                {
                    double rads = 1;
                    QGraphicsItem* windItem = scene -> addEllipse(xx - rads, yy - rads, rads*2.0, rads*2.0, pen);
                    windArrowPlot.push_back(windItem);

                    double rad = 2;
                    windItem = scene -> addEllipse(xx - rad, yy - rad, rad*2.0, rad*2.0, pen);
                    windArrowPlot.push_back(windItem);
                }
                else if (vApprox < 50)
                {
                    QGraphicsItem* windItem = scene -> addLine(xx, yy, xx + dx, yy + dy, pen);
                    windArrowPlot.push_back(windItem);

                    double d = arrowSize/6;
                    double bx = xx;
                    double by = yy;
                    int noOfSmallBarbs = (vApprox < 5) ? 1 : (vApprox/5) % 2;
                    int noOfBigBarbs = vApprox/10;

                    if (noOfBigBarbs == 0)
                    {
                        bx += d*qCos(ang);
                        by += d*qSin(ang);
                    }

                    for (int i = 0; i < noOfBigBarbs; i++)
                    {
                        QPointF be = barb(ang, arrowSize);

                        windItem = scene -> addLine(bx, by, bx + be.x(), by + be.y(), pen);
                        windArrowPlot.push_back(windItem);

                        bx += d*qCos(ang);
                        by += d*qSin(ang);
                    }

                    for (int i = 0; i < noOfSmallBarbs; i++)
                    {
                        QPointF be = barb(ang, arrowSize, true);

                        windItem = scene -> addLine(bx, by, bx + be.x(), by + be.y(), pen);
                        windArrowPlot.push_back(windItem);
                    }
                }
                else
                {
                    QGraphicsItem* windItem = scene -> addLine(xx, yy, xx + dx, yy + dy, pen);
                    windArrowPlot.push_back(windItem);

                    double d = arrowSize/6;
                    double bx = xx;
                    double by = yy;
                    int noOfSmallBarbs = (vApprox/5) % 2;
                    int noOfBigBarbs = (vApprox/10) % 5;
                    int noOfTriangles = vApprox/50;

                    for (int i = 0; i < noOfTriangles; i++)
                    {
                        QPair<QPointF, QPointF> p = triangle(ang, arrowSize);

                        windItem = scene->addLine(bx, by, bx + p.first.x(), by + p.first.y(), pen);
                        windArrowPlot.push_back(windItem);

                        windItem = scene->addLine(bx + p.second.x(), by + p.second.y(), bx + p.first.x(), by + p.first.y(), pen);
                        windArrowPlot.push_back(windItem);

                        bx = p.second.x();
                        by = p.second.y();
                    }

                    bx += d*qCos(ang);
                    by += d*qSin(ang);

                    for (int i = 0; i < noOfBigBarbs; i++)
                    {
                        QPointF be = barb(ang, arrowSize);

                        windItem = scene -> addLine(bx, by, bx + be.x(), by + be.y(), pen);
                        windArrowPlot.push_back(windItem);

                        bx += d*qCos(ang);
                        by += d*qSin(ang);
                    }

                    for (int i = 0; i < noOfSmallBarbs; i++)
                    {
                        QPointF be = barb(ang, arrowSize, true);

                        windItem = scene -> addLine(bx, by, bx + be.x(), by + be.y(), pen);
                        windArrowPlot.push_back(windItem);
                    }
                }
                i++;
            }
            j++;
        }
    }
    areInitWindArrowsDrawn = true;
}


void MainWindow::drawWindColor (int fc)
{
    vector<vector<double>> wSpeed = gridData->getWindSpeed(fc);

    if (wSpeed.size() == 0) {
        return;
    }

    double Di = gridData->getD();
    double Dj = gridData->getD();
    int xMin = gridData->getMinPoint().x();
    int yMin = gridData->getMinPoint().y();
    int xMax = gridData->getMaxPoint().x();
    int yMax = gridData->getMaxPoint().y();
    int Ni = gridData->getNi();
    int Nj = gridData->getNj();

    CustomScene* scene = m_pForm->GetGraphicsScene();
    double stepi = scene->stepI();
    double stepj = scene->stepJ();

    int mapXmax = scene->xExtent();
    int mapYmax = scene->yExtent();

    std::unordered_map<int, QColor> m = scene->GetWindColourScale();  // wind colour scale



    QVector<QVector<QColor>> cXY(Ni, QVector<QColor>(Nj, QColor(0,0,0))); // colour at every grid point

    for (int j = 0; j < Nj; j++)
    {
        for (int i = 0; i < Ni; i++)
        {
            double vInKnots = 1.94384 * (wSpeed[i][j]);
            int vc = (int)(qRound(vInKnots));
            cXY[i][j] = m[vc];
        }
    }

    QImage *colourLayer = new QImage (2*mapXmax, 2*mapYmax, QImage::Format_ARGB32);

    if (xMin < xMax)
    {
        int j = 0;
        for (int y = yMin; y <= yMax && j < Nj - 1; y += Dj)
        {
            int i = 0;
            float y_merc1 = toMerc(y)*stepj;
            float y_merc2 = toMerc(y + Di)*stepj;

            for (int x = xMin; x <= xMax && i < Ni - 1; x += Di)
            {
                // Grid -
                // p3   p4
                //
                // p1   p2

                double x1 = x*stepi, y1 = -y_merc1;
                double x2 = (x + Di)*stepi, y2 = -y_merc2;
                QPoint p1(x1, y1);
                QPoint p2(x2, y1);
                QPoint p3(x1, y2);
                QPoint p4(x2, y2);

                // respective colour at p1, p2, p3, p4
                int r1 = cXY[i][j].red(),      g1 = cXY[i][j].green(),         b1 = cXY[i][j].blue(),
                r2 = cXY[i + 1][j].red(),      g2 = cXY[i + 1][j].green(),     b2 = cXY[i + 1][j].blue(),
                r3 = cXY[i][j + 1].red(),      g3 = cXY[i][j + 1].green(),     b3 = cXY[i][j + 1].blue(),
                r4 = cXY[i + 1][j + 1].red(),  g4 = cXY[i + 1][j + 1].green(), b4 = cXY[i + 1][j + 1].blue();

                // applying bilinear filtering in the given grid
                for (int yy = p1.y(); yy > p3.y(); yy--) {
                    for (int xx = p1.x(); xx < p2.x(); xx++) {
                        int r_F = (r1 + ((r2 - r1)/(p2.x()-p1.x()))*(xx-p1.x()));
                        int g_F = (g1 + ((g2 - g1)/(p2.x()-p1.x()))*(xx-p1.x()));
                        int b_F = (b1 + ((b2 - b1)/(p2.x()-p1.x()))*(xx-p1.x()));
                        int r_H = (r3 + ((r4 - r3)/(p2.x()-p1.x()))*(xx-p1.x()));
                        int g_H = (g3 + ((g4 - g3)/(p2.x()-p1.x()))*(xx-p1.x()));
                        int b_H = (b3 + ((b4 - b3)/(p2.x()-p1.x()))*(xx-p1.x()));

                        // colour at general (x, y) in grid
                        int r_G = (r_F + ((r_H - r_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                        int g_G = (g_F + ((g_H - g_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                        int b_G = (b_F + ((b_H - b_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                        QPoint imgCen = QPoint(mapXmax, mapYmax);
                        QColor col = QColor(r_G, g_G, b_G);
                        colourLayer->setPixel(imgCen.x() + xx, imgCen.y() + yy, col.rgb());
                    }
                }
                i++;
            }
            j++;
        }
    }

    else
    {
        int j = 0;
        for (int y = yMin; y <= yMax && j < Nj - 1; y += Dj)
        {
            int i = 0;
            float y_merc1 = toMerc(y)*stepj;
            float y_merc2 = toMerc(y + Di)*stepj;

            int skipped = 0;

            for (int x = xMin; x <= 180 && i < Ni - 1; x += Di)
            {
                // Grid -
                // p3   p4
                //
                // p1   p2

                double x1 = x*stepi, y1 = -y_merc1;
                double x2 = (x + Di)*stepi, y2 = -y_merc2;
                QPoint p1(x1, y1);
                QPoint p2(x2, y1);
                QPoint p3(x1, y2);
                QPoint p4(x2, y2);

                // respective colour at p1, p2, p3, p4
                int r1 = cXY[i][j].red(),      g1 = cXY[i][j].green(),         b1 = cXY[i][j].blue(),
                r2 = cXY[i + 1][j].red(),      g2 = cXY[i + 1][j].green(),     b2 = cXY[i + 1][j].blue(),
                r3 = cXY[i][j + 1].red(),      g3 = cXY[i][j + 1].green(),     b3 = cXY[i][j + 1].blue(),
                r4 = cXY[i + 1][j + 1].red(),  g4 = cXY[i + 1][j + 1].green(), b4 = cXY[i + 1][j + 1].blue();

                // applying bilinear filtering in the given grid
                for (int yy = p1.y(); yy > p3.y(); yy--) {
                    for (int xx = p1.x(); xx < p2.x(); xx++) {
                        int r_F = (r1 + ((r2 - r1)/(p2.x()-p1.x()))*(xx-p1.x()));
                        int g_F = (g1 + ((g2 - g1)/(p2.x()-p1.x()))*(xx-p1.x()));
                        int b_F = (b1 + ((b2 - b1)/(p2.x()-p1.x()))*(xx-p1.x()));
                        int r_H = (r3 + ((r4 - r3)/(p2.x()-p1.x()))*(xx-p1.x()));
                        int g_H = (g3 + ((g4 - g3)/(p2.x()-p1.x()))*(xx-p1.x()));
                        int b_H = (b3 + ((b4 - b3)/(p2.x()-p1.x()))*(xx-p1.x()));

                        // colour at general (x, y) in grid
                        int r_G = (r_F + ((r_H - r_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                        int g_G = (g_F + ((g_H - g_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                        int b_G = (b_F + ((b_H - b_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                        QPoint imgCen = QPoint(mapXmax, mapYmax);
                        QColor col = QColor(r_G, g_G, b_G);
                        colourLayer->setPixel(imgCen.x() + xx, imgCen.y() + yy, col.rgb());
                    }
                }
                i++;
                if (x + Di > 180) {
                    skipped = 180 - x;
                }
            }

            int skip = skipped;

            for (int x = -179 + skip; x <= xMax && i < Ni - 1; x += Di)
            {
                // Grid -
                // p3   p4
                //
                // p1   p2

                double x1 = x*stepi, y1 = -y_merc1;
                double x2 = (x + Di)*stepi, y2 = -y_merc2;
                QPoint p1(x1, y1);
                QPoint p2(x2, y1);
                QPoint p3(x1, y2);
                QPoint p4(x2, y2);

                // respective colour at p1, p2, p3, p4
                int r1 = cXY[i][j].red(),      g1 = cXY[i][j].green(),         b1 = cXY[i][j].blue(),
                r2 = cXY[i + 1][j].red(),      g2 = cXY[i + 1][j].green(),     b2 = cXY[i + 1][j].blue(),
                r3 = cXY[i][j + 1].red(),      g3 = cXY[i][j + 1].green(),     b3 = cXY[i][j + 1].blue(),
                r4 = cXY[i + 1][j + 1].red(),  g4 = cXY[i + 1][j + 1].green(), b4 = cXY[i + 1][j + 1].blue();

                // applying bilinear filtering in the given grid
                for (int yy = p1.y(); yy > p3.y(); yy--) {
                    for (int xx = p1.x(); xx < p2.x(); xx++) {
                        int r_F = (r1 + ((r2 - r1)/(p2.x()-p1.x()))*(xx-p1.x()));
                        int g_F = (g1 + ((g2 - g1)/(p2.x()-p1.x()))*(xx-p1.x()));
                        int b_F = (b1 + ((b2 - b1)/(p2.x()-p1.x()))*(xx-p1.x()));
                        int r_H = (r3 + ((r4 - r3)/(p2.x()-p1.x()))*(xx-p1.x()));
                        int g_H = (g3 + ((g4 - g3)/(p2.x()-p1.x()))*(xx-p1.x()));
                        int b_H = (b3 + ((b4 - b3)/(p2.x()-p1.x()))*(xx-p1.x()));

                        // colour at general (x, y) in grid
                        int r_G = (r_F + ((r_H - r_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                        int g_G = (g_F + ((g_H - g_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                        int b_G = (b_F + ((b_H - b_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                        QPoint imgCen = QPoint(mapXmax, mapYmax);
                        QColor col = QColor(r_G, g_G, b_G);
                        colourLayer->setPixel(imgCen.x() + xx, imgCen.y() + yy, col.rgb());
                    }
                }
                i++;
            }
            j++;
        }
    }

    windColorPlot  = new QGraphicsPixmapItem(QPixmap::fromImage(*colourLayer));
    windColorPlot->setOffset(-mapXmax, -mapYmax);
    windColorPlot->setPos(0, 0);
    windColorPlot->setZValue(-1);

    scene -> addItem(windColorPlot);

    isInitWindSpeedDrawn = true;
}


void MainWindow::drawWaveSigHtColor (int fc)
{
    vector<vector<double>> wvSigHt = gridData->getWaveSigHeight(fc);

    if (wvSigHt.size() == 0) {
        return;
    }

    double Di = gridData->getD();
    double Dj = gridData->getD();
    int xMin = gridData->getMinPoint().x();
    int yMin = gridData->getMinPoint().y();
    int xMax = gridData->getMaxPoint().x();
    int yMax = gridData->getMaxPoint().y();
    int Ni = gridData->getNi();
    int Nj = gridData->getNj();

    CustomScene* scene = m_pForm->GetGraphicsScene();
    double stepi = scene->stepI();
    double stepj = scene->stepJ();

    scene->clearInitialGrid();

    int mapXmax = scene->xExtent();
    int mapYmax = scene->yExtent();

    std::unordered_map<int, QColor> m = scene->GetWaveColourScale();  // wave colour scale



    QVector<QVector<QColor>> cXY(Ni, QVector<QColor>(Nj, QColor(0,0,0))); // colour at every grid point

    for (int j = 0; j < Nj; j++)
    {
        for (int i = 0; i < Ni; i++)
        {
            double htInHm = (wvSigHt[i][j]) * 10;

            int h = (int)(qRound(htInHm));
            qInfo() << h;
            if (h == 0) {
                cXY[i][j] = m[-1];
            }
            else {
                cXY[i][j] = m[h];
            }
        }
    }

    QImage *colourLayer = new QImage (2*mapXmax, 2*mapYmax, QImage::Format_ARGB32);
    QPoint imgCen = QPoint(mapXmax, mapYmax);


    if (xMin < xMax)
    {
        int j = 0;
        for (int y = yMin; y <= yMax && j < Nj - 1; y += Dj)
        {
            int i = 0;
            float y_merc1 = toMerc(y)*stepj;
            float y_merc2 = toMerc(y + Di)*stepj;

            for (int x = xMin; x <= xMax && i < Ni - 1; x += Di)
            {
                // Grid -
                // p3   p4
                //
                // p1   p2

                double x1 = x*stepi, y1 = -y_merc1;
                double x2 = (x + Di)*stepi, y2 = -y_merc2;
                QPoint p1(x1, y1);
                QPoint p2(x2, y1);
                QPoint p3(x1, y2);
                QPoint p4(x2, y2);

                // respective colour at p1, p2, p3, p4
                if (cXY[i][j] != m[-1]) {
                    int r1 = cXY[i][j].red(),      g1 = cXY[i][j].green(),         b1 = cXY[i][j].blue(),
                    r2 = cXY[i + 1][j].red(),      g2 = cXY[i + 1][j].green(),     b2 = cXY[i + 1][j].blue(),
                    r3 = cXY[i][j + 1].red(),      g3 = cXY[i][j + 1].green(),     b3 = cXY[i][j + 1].blue(),
                    r4 = cXY[i + 1][j + 1].red(),  g4 = cXY[i + 1][j + 1].green(), b4 = cXY[i + 1][j + 1].blue();

                    // applying bilinear filtering in the given grid
                    for (int yy = p1.y(); yy > p3.y(); yy--) {
                        for (int xx = p1.x(); xx < p2.x(); xx++) {
                            int r_F = (r1 + ((r2 - r1)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int g_F = (g1 + ((g2 - g1)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int b_F = (b1 + ((b2 - b1)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int r_H = (r3 + ((r4 - r3)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int g_H = (g3 + ((g4 - g3)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int b_H = (b3 + ((b4 - b3)/(p2.x()-p1.x()))*(xx-p1.x()));

                            // colour at general (x, y) in grid
                            int r_G = (r_F + ((r_H - r_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                            int g_G = (g_F + ((g_H - g_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                            int b_G = (b_F + ((b_H - b_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                            QColor col = QColor(r_G, g_G, b_G);
                            colourLayer->setPixel(imgCen.x() + xx, imgCen.y() + yy, col.rgb());
                        }
                    }
                }
                i++;
            }
            j++;
        }
    }

    else
    {
        int j = 0;
        for (int y = yMin; y <= yMax && j < Nj - 1; y+=Dj)
        {
            int i = 0;
            float y_merc1 = toMerc(y)*stepj;
            float y_merc2 = toMerc(y + Di)*stepj;

            int skipped = 0;

            for (int x = xMin; x <= 180 && i < Ni - 1; x += Di)
            {
                // Grid -
                // p3   p4
                //
                // p1   p2

                double x1 = x*stepi, y1 = -y_merc1;
                double x2 = (x + Di)*stepi, y2 = -y_merc2;
                QPoint p1(x1, y1);
                QPoint p2(x2, y1);
                QPoint p3(x1, y2);
                QPoint p4(x2, y2);

                // respective colour at p1, p2, p3, p4
                if (cXY[i][j] != m[-1]) {
                    int r1 = cXY[i][j].red(),      g1 = cXY[i][j].green(),         b1 = cXY[i][j].blue(),
                    r2 = cXY[i + 1][j].red(),      g2 = cXY[i + 1][j].green(),     b2 = cXY[i + 1][j].blue(),
                    r3 = cXY[i][j + 1].red(),      g3 = cXY[i][j + 1].green(),     b3 = cXY[i][j + 1].blue(),
                    r4 = cXY[i + 1][j + 1].red(),  g4 = cXY[i + 1][j + 1].green(), b4 = cXY[i + 1][j + 1].blue();

                    // applying bilinear filtering in the given grid
                    for (int yy = p1.y(); yy > p3.y(); yy--) {
                        for (int xx = p1.x(); xx < p2.x(); xx++) {
                            int r_F = (r1 + ((r2 - r1)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int g_F = (g1 + ((g2 - g1)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int b_F = (b1 + ((b2 - b1)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int r_H = (r3 + ((r4 - r3)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int g_H = (g3 + ((g4 - g3)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int b_H = (b3 + ((b4 - b3)/(p2.x()-p1.x()))*(xx-p1.x()));

                            // colour at general (x, y) in grid
                            int r_G = (r_F + ((r_H - r_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                            int g_G = (g_F + ((g_H - g_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                            int b_G = (b_F + ((b_H - b_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                            QColor col = QColor(r_G, g_G, b_G);
                            colourLayer->setPixel(imgCen.x() + xx, imgCen.y() + yy, col.rgb());
                        }
                    }
                }
                i++;
                if (x + Di > 180) {
                    skipped = 180 - x;
                }
            }

            int skip = skipped;

            for (int x = -179 + skip; x <= xMax && i < Ni - 1; x += Di)
            {
                // Grid -
                // p3   p4
                //
                // p1   p2

                double x1 = x*stepi, y1 = -y_merc1;
                double x2 = (x + Di)*stepi, y2 = -y_merc2;
                QPoint p1(x1, y1);
                QPoint p2(x2, y1);
                QPoint p3(x1, y2);
                QPoint p4(x2, y2);

                // respective colour at p1, p2, p3, p4
                if (cXY[i][j] != m[-1]) {
                    int r1 = cXY[i][j].red(),      g1 = cXY[i][j].green(),         b1 = cXY[i][j].blue(),
                    r2 = cXY[i + 1][j].red(),      g2 = cXY[i + 1][j].green(),     b2 = cXY[i + 1][j].blue(),
                    r3 = cXY[i][j + 1].red(),      g3 = cXY[i][j + 1].green(),     b3 = cXY[i][j + 1].blue(),
                    r4 = cXY[i + 1][j + 1].red(),  g4 = cXY[i + 1][j + 1].green(), b4 = cXY[i + 1][j + 1].blue();

                    // applying bilinear filtering in the given grid
                    for (int yy = p1.y(); yy > p3.y(); yy--) {
                        for (int xx = p1.x(); xx < p2.x(); xx++) {
                            int r_F = (r1 + ((r2 - r1)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int g_F = (g1 + ((g2 - g1)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int b_F = (b1 + ((b2 - b1)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int r_H = (r3 + ((r4 - r3)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int g_H = (g3 + ((g4 - g3)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int b_H = (b3 + ((b4 - b3)/(p2.x()-p1.x()))*(xx-p1.x()));

                            // colour at general (x, y) in grid
                            int r_G = (r_F + ((r_H - r_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                            int g_G = (g_F + ((g_H - g_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                            int b_G = (b_F + ((b_H - b_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                            QColor col = QColor(r_G, g_G, b_G);
                            colourLayer->setPixel(imgCen.x() + xx, imgCen.y() + yy, col.rgb());
                        }
                    }
                }
                i++;
            }
            j++;
        }
    }

    waveSigHtColorPlot  = new QGraphicsPixmapItem(QPixmap::fromImage(*colourLayer));
    waveSigHtColorPlot->setOffset(-mapXmax, -mapYmax);
    waveSigHtColorPlot->setPos(0, 0);
    waveSigHtColorPlot->setZValue(-1);
    scene->addItem(waveSigHtColorPlot);

    isInitWaveSigHtDrawn = true;
}


void MainWindow::drawWindWaveArrow(int fc) {

    vector<vector<double>> wWvHt = gridData->getWindWaveHeight(fc);
    vector<vector<int>> wWvDir = gridData->getWindWaveDir(fc);

    if (wWvHt.size() == 0 || wWvDir.size() == 0) {
        return;
    }

    double Di = gridData->getD();
    double Dj = gridData->getD();
    int xMin = gridData->getMinPoint().x();
    int yMin = gridData->getMinPoint().y();
    int xMax = gridData->getMaxPoint().x();
    int yMax = gridData->getMaxPoint().y();
    int Ni = gridData->getNi();
    int Nj = gridData->getNj();


    CustomScene* scene = m_pForm->GetGraphicsScene();

    double stepi = scene->stepI();
    double stepj = scene->stepJ();

    // arrow properties
    QPen pen = QPen(Qt::black, 0.8);
    pen.setCosmetic(true);
    double arrowSize = stepi;


    if (xMin < xMax)
    {
        int j = 0;
        for (int y = yMin; y <= yMax && j < Nj; y += Dj)
        {
            int i = 0;
            for (int x = xMin; x <= xMax && i < Ni; x += Di)
            {
                double h = wWvHt[i][j];
                if (h == 0) {
                    i++;
                    continue;
                }
                double ang = (90 - (wWvDir[i][j])) * M_PI / 180.0;  // angle of arrow with +ve x axis in scene's ref
                double dx = arrowSize * qCos(ang);         // projection of length of arrow on x - axis (in scene's ref)
                double dy = arrowSize * qSin(ang);         // projection of length of arrow on y - axis      ,,
                double xx = x * stepi;                      // starting coordinates of the arrow
                double yy = -toMerc(y) * stepj;

                QGraphicsItem* windWaveItem = scene->addLine(xx, yy, xx + dx, yy + dy, pen);
                windWaveArrowPlot.push_back(windWaveItem);

                //arrow heads
                double ax1 = xx + dx - qCos(ang - M_PI / 6) * arrowSize / 5;
                double ay1 = yy + dy - qSin(ang - M_PI / 6) * arrowSize / 5;
                double ax2 = xx + dx - qSin(M_PI / 3 - ang) * arrowSize / 5;
                double ay2 = yy + dy - qCos(M_PI / 3 - ang) * arrowSize / 5;

                windWaveItem = scene->addLine(xx + dx, yy + dy, ax1, ay1, pen);
                windWaveArrowPlot.push_back(windWaveItem);

                windWaveItem = scene->addLine(xx + dx, yy + dy, ax2, ay2, pen);
                windWaveArrowPlot.push_back(windWaveItem);

                i++;
            }
            j++;
        }
    }

    else
    {
        int j = 0;
        for (int y = yMin; y <= yMax && j < Nj; y+=Dj)
        {
            int i = 0;
            int skipped = 0;
            for (int x = xMin; x <= 180; x+=Di)
            {
                double h = wWvHt[i][j];
                if (h == 0) {
                    i++;
                    continue;
                }
                double ang = (wWvDir[i][j]) * M_PI / 180.0;   // angle of arrow with +ve x axis in scene's ref
                double dx = arrowSize*qCos(ang);         // projection of length of arrow on x - axis (in scene's ref)
                double dy = arrowSize*qSin(ang);         // projection of length of arrow on y - axis      ,,
                double xx = x*stepi;   // starting coordinates of the arrow
                double yy = -toMerc(y)*stepj;

                QGraphicsItem* windWaveItem = scene->addLine(xx, yy, xx + dx, yy + dy, pen);
                windWaveArrowPlot.push_back(windWaveItem);

                //arrow heads
                double ax1 = xx + dx - qCos(ang - M_PI / 6) * arrowSize / 5;
                double ay1 = yy + dy - qSin(ang - M_PI / 6) * arrowSize / 5;
                double ax2 = xx + dx - qSin(M_PI / 3 - ang) * arrowSize / 5;
                double ay2 = yy + dy - qCos(M_PI / 3 - ang) * arrowSize / 5;

                windWaveItem = scene->addLine(xx + dx, yy + dy, ax1, ay1, pen);
                windWaveArrowPlot.push_back(windWaveItem);

                windWaveItem = scene->addLine(xx + dx, yy + dy, ax2, ay2, pen);
                windWaveArrowPlot.push_back(windWaveItem);

                i++;
                if (x + Di > 180) {
                    skipped = 180 - x;
                }
            }

            int skip = Di - 1 - skipped;

            for (int x = -179 + skip; x <= xMax && i < Ni; x += Di)
            {
                double h = wWvHt[i][j];
                if (h == 0) {
                    i++;
                    continue;
                }
                double ang = (wWvDir[i][j]) * M_PI / 180.0;  // angle of arrow with +ve x axis in scene's ref
                double dx = arrowSize*qCos(ang);         // projection of length of arrow on x - axis (in scene's ref)
                double dy = arrowSize*qSin(ang);         // projection of length of arrow on y - axis      ,,
                double xx = x*stepi;   // starting coordinates of the arrow
                double yy = -toMerc(y)*stepj;

                QGraphicsItem* windWaveItem = scene->addLine(xx, yy, xx + dx, yy + dy, pen);
                windWaveArrowPlot.push_back(windWaveItem);

                //arrow heads
                double ax1 = xx + dx - qCos(ang - M_PI / 6) * arrowSize / 5;
                double ay1 = yy + dy - qSin(ang - M_PI / 6) * arrowSize / 5;
                double ax2 = xx + dx - qSin(M_PI / 3 - ang) * arrowSize / 5;
                double ay2 = yy + dy - qCos(M_PI / 3 - ang) * arrowSize / 5;

                windWaveItem = scene->addLine(xx + dx, yy + dy, ax1, ay1, pen);
                windWaveArrowPlot.push_back(windWaveItem);

                windWaveItem = scene->addLine(xx + dx, yy + dy, ax2, ay2, pen);
                windWaveArrowPlot.push_back(windWaveItem);

                i++;
            }
            j++;
        }
    }

    areInitWindArrowsDrawn = true;
}

void MainWindow::drawWindWaveHtColor (int fc)
{
    vector<vector<double>> wWvHt = gridData->getWindWaveHeight(fc);

    if (wWvHt.size() == 0) {
        return;
    }

    double Di = gridData->getD();
    double Dj = gridData->getD();
    int xMin = gridData->getMinPoint().x();
    int yMin = gridData->getMinPoint().y();
    int xMax = gridData->getMaxPoint().x();
    int yMax = gridData->getMaxPoint().y();
    int Ni = gridData->getNi();
    int Nj = gridData->getNj();

    CustomScene* scene = m_pForm->GetGraphicsScene();
    double stepi = scene->stepI();
    double stepj = scene->stepJ();

    scene->clearInitialGrid();

    int mapXmax = scene->xExtent();
    int mapYmax = scene->yExtent();

    std::unordered_map<int, QColor> m = scene->GetWaveColourScale();  // wind colour scale



    QVector<QVector<QColor>> cXY(Ni, QVector<QColor>(Nj, QColor(0,0,0))); // colour at every grid point

    for (int j = 0; j < Nj; j++)
    {
        for (int i = 0; i < Ni; i++)
        {
            double htInHm = (wWvHt[i][j]) * 10;

            int h = (int)(qRound(htInHm));
            if (h == 0) {
                cXY[i][j] = m[-1];
            }
            else {
                cXY[i][j] = m[h];
            }
        }
    }

    QImage *colourLayer = new QImage (2*mapXmax, 2*mapYmax, QImage::Format_ARGB32);
    QPoint imgCen = QPoint(mapXmax, mapYmax);

    if (xMin < xMax)
    {
        int j = 0;
        for (int y = yMin; y <= yMax && j < Nj - 1; y += Dj)
        {
            int i = 0;
            float y_merc1 = toMerc(y)*stepj;
            float y_merc2 = toMerc(y + Di)*stepj;

            for (int x = xMin; x <= xMax && i < Ni - 1; x += Di)
            {
                // Grid -
                // p3   p4
                //
                // p1   p2

                double x1 = x*stepi, y1 = -y_merc1;
                double x2 = (x + Di)*stepi, y2 = -y_merc2;
                QPoint p1(x1, y1);
                QPoint p2(x2, y1);
                QPoint p3(x1, y2);
                QPoint p4(x2, y2);

                // respective colour at p1, p2, p3, p4
                if (cXY[i][j] != m[-1]) {
                    int r1 = cXY[i][j].red(),      g1 = cXY[i][j].green(),         b1 = cXY[i][j].blue(),
                    r2 = cXY[i + 1][j].red(),      g2 = cXY[i + 1][j].green(),     b2 = cXY[i + 1][j].blue(),
                    r3 = cXY[i][j + 1].red(),      g3 = cXY[i][j + 1].green(),     b3 = cXY[i][j + 1].blue(),
                    r4 = cXY[i + 1][j + 1].red(),  g4 = cXY[i + 1][j + 1].green(), b4 = cXY[i + 1][j + 1].blue();

                    // applying bilinear filtering in the given grid
                    for (int yy = p1.y(); yy > p3.y(); yy--) {
                        for (int xx = p1.x(); xx < p2.x(); xx++) {
                            int r_F = (r1 + ((r2 - r1)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int g_F = (g1 + ((g2 - g1)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int b_F = (b1 + ((b2 - b1)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int r_H = (r3 + ((r4 - r3)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int g_H = (g3 + ((g4 - g3)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int b_H = (b3 + ((b4 - b3)/(p2.x()-p1.x()))*(xx-p1.x()));

                            // colour at general (x, y) in grid
                            int r_G = (r_F + ((r_H - r_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                            int g_G = (g_F + ((g_H - g_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                            int b_G = (b_F + ((b_H - b_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                            QColor col = QColor(r_G, g_G, b_G);
                            colourLayer->setPixel(imgCen.x() + xx, imgCen.y() + yy, col.rgb());
                        }
                    }
                }
                i++;
            }
            j++;
        }
    }

    else
    {
        int j = 0;
        for (int y = yMin; y <= yMax && j < Nj - 1; y += Dj)
        {
            int i = 0;
            float y_merc1 = toMerc(y)*stepj;
            float y_merc2 = toMerc(y + Di)*stepj;

            int skipped = 0;

            for (int x = xMin; x <= 180 && i < Ni - 1; x += Di)
            {
                // Grid -
                // p3   p4
                //
                // p1   p2

                double x1 = x*stepi, y1 = -y_merc1;
                double x2 = (x + Di)*stepi, y2 = -y_merc2;
                QPoint p1(x1, y1);
                QPoint p2(x2, y1);
                QPoint p3(x1, y2);
                QPoint p4(x2, y2);

                // respective colour at p1, p2, p3, p4
                if (cXY[i][j] != m[-1]) {
                    int r1 = cXY[i][j].red(),      g1 = cXY[i][j].green(),         b1 = cXY[i][j].blue(),
                    r2 = cXY[i + 1][j].red(),      g2 = cXY[i + 1][j].green(),     b2 = cXY[i + 1][j].blue(),
                    r3 = cXY[i][j + 1].red(),      g3 = cXY[i][j + 1].green(),     b3 = cXY[i][j + 1].blue(),
                    r4 = cXY[i + 1][j + 1].red(),  g4 = cXY[i + 1][j + 1].green(), b4 = cXY[i + 1][j + 1].blue();

                    // applying bilinear filtering in the given grid
                    for (int yy = p1.y(); yy > p3.y(); yy--) {
                        for (int xx = p1.x(); xx < p2.x(); xx++) {
                            int r_F = (r1 + ((r2 - r1)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int g_F = (g1 + ((g2 - g1)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int b_F = (b1 + ((b2 - b1)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int r_H = (r3 + ((r4 - r3)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int g_H = (g3 + ((g4 - g3)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int b_H = (b3 + ((b4 - b3)/(p2.x()-p1.x()))*(xx-p1.x()));

                            // colour at general (x, y) in grid
                            int r_G = (r_F + ((r_H - r_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                            int g_G = (g_F + ((g_H - g_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                            int b_G = (b_F + ((b_H - b_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                            QColor col = QColor(r_G, g_G, b_G);
                            colourLayer->setPixel(imgCen.x() + xx, imgCen.y() + yy, col.rgb());
                        }
                    }
                }
                i++;
                if (x + Di > 180) {
                    skipped = 180 - x;
                }
            }

            int skip = skipped;

            for (int x = -179 + skip; x <= xMax && i < Ni - 1; x += Di)
            {
                // Grid -
                // p3   p4
                //
                // p1   p2

                double x1 = x*stepi, y1 = -y_merc1;
                double x2 = (x + Di)*stepi, y2 = -y_merc2;
                QPoint p1(x1, y1);
                QPoint p2(x2, y1);
                QPoint p3(x1, y2);
                QPoint p4(x2, y2);

                // respective colour at p1, p2, p3, p4
                if (cXY[i][j] != m[-1]) {
                    int r1 = cXY[i][j].red(),      g1 = cXY[i][j].green(),         b1 = cXY[i][j].blue(),
                    r2 = cXY[i + 1][j].red(),      g2 = cXY[i + 1][j].green(),     b2 = cXY[i + 1][j].blue(),
                    r3 = cXY[i][j + 1].red(),      g3 = cXY[i][j + 1].green(),     b3 = cXY[i][j + 1].blue(),
                    r4 = cXY[i + 1][j + 1].red(),  g4 = cXY[i + 1][j + 1].green(), b4 = cXY[i + 1][j + 1].blue();

                    // applying bilinear filtering in the given grid
                    for (int yy = p1.y(); yy > p3.y(); yy--) {
                        for (int xx = p1.x(); xx < p2.x(); xx++) {
                            int r_F = (r1 + ((r2 - r1)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int g_F = (g1 + ((g2 - g1)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int b_F = (b1 + ((b2 - b1)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int r_H = (r3 + ((r4 - r3)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int g_H = (g3 + ((g4 - g3)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int b_H = (b3 + ((b4 - b3)/(p2.x()-p1.x()))*(xx-p1.x()));

                            // colour at general (x, y) in grid
                            int r_G = (r_F + ((r_H - r_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                            int g_G = (g_F + ((g_H - g_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                            int b_G = (b_F + ((b_H - b_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                            QColor col = QColor(r_G, g_G, b_G);
                            colourLayer->setPixel(imgCen.x() + xx, imgCen.y() + yy, col.rgb());
                        }
                    }
                }
                i++;
            }
            j++;
        }
    }

    windWaveHtColorPlot  = new QGraphicsPixmapItem(QPixmap::fromImage(*colourLayer));
    windWaveHtColorPlot->setOffset(-mapXmax, -mapYmax);
    windWaveHtColorPlot->setPos(0, 0);
    windWaveHtColorPlot->setZValue(-1);

    scene->addItem(windWaveHtColorPlot);

    isInitWindWaveHtDrawn = true;
}


void MainWindow::drawSwellWaveHtColor (int fc)
{
    vector<vector<double>> sWvHt = gridData->getSwellWaveHeight(fc);

    if (sWvHt.size() == 0) {
        return;
    }

    double Di = gridData->getD();
    double Dj = gridData->getD();
    int xMin = gridData->getMinPoint().x();
    int yMin = gridData->getMinPoint().y();
    int xMax = gridData->getMaxPoint().x();
    int yMax = gridData->getMaxPoint().y();
    int Ni = gridData->getNi();
    int Nj = gridData->getNj();

    CustomScene* scene = m_pForm->GetGraphicsScene();
    double stepi = scene->stepI();
    double stepj = scene->stepJ();

    int mapXmax = scene->xExtent();
    int mapYmax = scene->yExtent();

    std::unordered_map<int, QColor> m = scene->GetWaveColourScale();  // wind colour scale



    QVector<QVector<QColor>> cXY(Ni, QVector<QColor>(Nj, QColor(0,0,0))); // colour at every grid point

    for (int j = 0; j < Nj; j++)
    {
        for (int i = 0; i < Ni; i++)
        {
            double htInHm = (sWvHt[i][j]) * 10;

            int h = (int)(qRound(htInHm));
            if (h == 0) {
                cXY[i][j] = m[-1];
            }
            else {
                cXY[i][j] = m[h];
            }
        }
    }

    QImage *colourLayer = new QImage (2*mapXmax, 2*mapYmax, QImage::Format_ARGB32);
    QPoint imgCen = QPoint(mapXmax, mapYmax);

    if (xMin < xMax)
    {
        int j = 0;
        for (int y = yMin; y <= yMax && j < Nj - 1; y += Dj)
        {
            int i = 0;
            float y_merc1 = toMerc(y)*stepj;
            float y_merc2 = toMerc(y + Di)*stepj;

            for (int x = xMin; x <= xMax && i < Ni - 1; x += Di)
            {
                // Grid -
                // p3   p4
                //
                // p1   p2

                double x1 = x*stepi, y1 = -y_merc1;
                double x2 = (x + Di)*stepi, y2 = -y_merc2;
                QPoint p1(x1, y1);
                QPoint p2(x2, y1);
                QPoint p3(x1, y2);
                QPoint p4(x2, y2);

                // respective colour at p1, p2, p3, p4
                if (cXY[i][j] != m[-1]) {
                    int r1 = cXY[i][j].red(),      g1 = cXY[i][j].green(),         b1 = cXY[i][j].blue(),
                    r2 = cXY[i + 1][j].red(),      g2 = cXY[i + 1][j].green(),     b2 = cXY[i + 1][j].blue(),
                    r3 = cXY[i][j + 1].red(),      g3 = cXY[i][j + 1].green(),     b3 = cXY[i][j + 1].blue(),
                    r4 = cXY[i + 1][j + 1].red(),  g4 = cXY[i + 1][j + 1].green(), b4 = cXY[i + 1][j + 1].blue();

                    // applying bilinear filtering in the given grid
                    for (int yy = p1.y(); yy > p3.y(); yy--) {
                        for (int xx = p1.x(); xx < p2.x(); xx++) {
                            int r_F = (r1 + ((r2 - r1)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int g_F = (g1 + ((g2 - g1)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int b_F = (b1 + ((b2 - b1)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int r_H = (r3 + ((r4 - r3)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int g_H = (g3 + ((g4 - g3)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int b_H = (b3 + ((b4 - b3)/(p2.x()-p1.x()))*(xx-p1.x()));

                            // colour at general (x, y) in grid
                            int r_G = (r_F + ((r_H - r_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                            int g_G = (g_F + ((g_H - g_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                            int b_G = (b_F + ((b_H - b_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                            QColor col = QColor(r_G, g_G, b_G);
                            colourLayer->setPixel(imgCen.x() + xx, imgCen.y() + yy, col.rgb());
                        }
                    }
                }
                i++;
            }
            j++;
        }
    }

    else
    {
        int j = 0;
        for (int y = yMin; y <= yMax && j < Nj - 1; y += Dj)
        {
            int i = 0;
            float y_merc1 = toMerc(y)*stepj;
            float y_merc2 = toMerc(y + Di)*stepj;

            int skipped = 0;

            for (int x = xMin; x <= 180 && i < Ni - 1; x += Di)
            {
                // Grid -
                // p3   p4
                //
                // p1   p2

                double x1 = x*stepi, y1 = -y_merc1;
                double x2 = (x + Di)*stepi, y2 = -y_merc2;
                QPoint p1(x1, y1);
                QPoint p2(x2, y1);
                QPoint p3(x1, y2);
                QPoint p4(x2, y2);

                // respective colour at p1, p2, p3, p4
                if (cXY[i][j] != m[-1]) {
                    int r1 = cXY[i][j].red(),      g1 = cXY[i][j].green(),         b1 = cXY[i][j].blue(),
                    r2 = cXY[i + 1][j].red(),      g2 = cXY[i + 1][j].green(),     b2 = cXY[i + 1][j].blue(),
                    r3 = cXY[i][j + 1].red(),      g3 = cXY[i][j + 1].green(),     b3 = cXY[i][j + 1].blue(),
                    r4 = cXY[i + 1][j + 1].red(),  g4 = cXY[i + 1][j + 1].green(), b4 = cXY[i + 1][j + 1].blue();

                    // applying bilinear filtering in the given grid
                    for (int yy = p1.y(); yy > p3.y(); yy--) {
                        for (int xx = p1.x(); xx < p2.x(); xx++) {
                            int r_F = (r1 + ((r2 - r1)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int g_F = (g1 + ((g2 - g1)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int b_F = (b1 + ((b2 - b1)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int r_H = (r3 + ((r4 - r3)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int g_H = (g3 + ((g4 - g3)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int b_H = (b3 + ((b4 - b3)/(p2.x()-p1.x()))*(xx-p1.x()));

                            // colour at general (x, y) in grid
                            int r_G = (r_F + ((r_H - r_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                            int g_G = (g_F + ((g_H - g_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                            int b_G = (b_F + ((b_H - b_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                            QColor col = QColor(r_G, g_G, b_G);
                            colourLayer->setPixel(imgCen.x() + xx, imgCen.y() + yy, col.rgb());
                        }
                    }
                }
                i++;
                if (x + Di > 180) {
                    skipped = 180 - x;
                }
            }

            int skip = skipped;

            for (int x = -179 + skip; x <= xMax && i < Ni - 1; x += Di)
            {
                // Grid -
                // p3   p4
                //
                // p1   p2

                double x1 = x*stepi, y1 = -y_merc1;
                double x2 = (x + Di)*stepi, y2 = -y_merc2;
                QPoint p1(x1, y1);
                QPoint p2(x2, y1);
                QPoint p3(x1, y2);
                QPoint p4(x2, y2);

                // respective colour at p1, p2, p3, p4
                if (cXY[i][j] != m[-1]) {
                    int r1 = cXY[i][j].red(),      g1 = cXY[i][j].green(),         b1 = cXY[i][j].blue(),
                    r2 = cXY[i + 1][j].red(),      g2 = cXY[i + 1][j].green(),     b2 = cXY[i + 1][j].blue(),
                    r3 = cXY[i][j + 1].red(),      g3 = cXY[i][j + 1].green(),     b3 = cXY[i][j + 1].blue(),
                    r4 = cXY[i + 1][j + 1].red(),  g4 = cXY[i + 1][j + 1].green(), b4 = cXY[i + 1][j + 1].blue();

                    // applying bilinear filtering in the given grid
                    for (int yy = p1.y(); yy > p3.y(); yy--) {
                        for (int xx = p1.x(); xx < p2.x(); xx++) {
                            int r_F = (r1 + ((r2 - r1)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int g_F = (g1 + ((g2 - g1)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int b_F = (b1 + ((b2 - b1)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int r_H = (r3 + ((r4 - r3)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int g_H = (g3 + ((g4 - g3)/(p2.x()-p1.x()))*(xx-p1.x()));
                            int b_H = (b3 + ((b4 - b3)/(p2.x()-p1.x()))*(xx-p1.x()));

                            // colour at general (x, y) in grid
                            int r_G = (r_F + ((r_H - r_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                            int g_G = (g_F + ((g_H - g_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                            int b_G = (b_F + ((b_H - b_F)/(p1.y()-p3.y()))*(p1.y() - yy)) + 0.5;
                            QColor col = QColor(r_G, g_G, b_G);
                            colourLayer->setPixel(imgCen.x() + xx, imgCen.y() + yy, col.rgb());
                        }
                    }
                }
                i++;
            }
            j++;
        }
    }

    swellWaveHtColorPlot  = new QGraphicsPixmapItem(QPixmap::fromImage(*colourLayer));
    swellWaveHtColorPlot->setOffset(-mapXmax, -mapYmax);
    swellWaveHtColorPlot->setPos(0, 0);
    swellWaveHtColorPlot->setZValue(-1);

    scene->addItem(swellWaveHtColorPlot);

    isInitSwellWaveHtDrawn = true;
}

void MainWindow::drawSwellWaveArrow(int fc) {

    vector<vector<double>> sWvHt = gridData->getCurrentSpeed(fc);
    vector<vector<int>> sWvDir = gridData->getCurrentDir(fc);

    if (sWvHt.size() == 0 || sWvDir.size() == 0) {
        return;
    }

    double Di = gridData->getD();
    double Dj = gridData->getD();
    int xMin = gridData->getMinPoint().x();
    int yMin = gridData->getMinPoint().y();
    int xMax = gridData->getMaxPoint().x();
    int yMax = gridData->getMaxPoint().y();
    int Ni = gridData->getNi();
    int Nj = gridData->getNj();

    CustomScene* scene = m_pForm->GetGraphicsScene();

    double stepi = scene->stepI();
    double stepj = scene->stepJ();

    // arrow properties
    QPen pen = QPen(Qt::black, 0.8);
    pen.setCosmetic(true);
    double arrowSize = stepi;

    if (xMin < xMax)
    {
        int j = 0;
        for (int y = yMin; y <= yMax && j < Nj; y += Dj)
        {
            int i = 0;
            for (int x = xMin; x <= xMax && i < Ni; x += Di)
            {
                double h = sWvHt[i][j];
                if (h == 0) {
                    i++;
                    continue;
                }
                double ang = (sWvDir[i][j]) * M_PI / 180.0;  // angle of arrow with +ve x axis in scene's ref
                double dx = arrowSize * qCos(ang);         // projection of length of arrow on x - axis (in scene's ref)
                double dy = arrowSize * qSin(ang);         // projection of length of arrow on y - axis      ,,
                double xx = x * stepi;   // starting coordinates of the arrow
                double yy = -toMerc(y) * stepj;

                QGraphicsItem* swellWaveItem = scene->addLine(xx, yy, xx + dx, yy + dy, pen);
                swellWaveArrowPlot.push_back(swellWaveItem);

                //arrow heads
                double ax1 = xx + dx - qCos(ang - M_PI / 6) * arrowSize / 5;
                double ay1 = yy + dy - qSin(ang - M_PI / 6) * arrowSize / 5;
                double ax2 = xx + dx - qSin(M_PI / 3 - ang) * arrowSize / 5;
                double ay2 = yy + dy - qCos(M_PI / 3 - ang) * arrowSize / 5;

                swellWaveItem = scene->addLine(xx + dx, yy + dy, ax1, ay1, pen);
                swellWaveArrowPlot.push_back(swellWaveItem);

                swellWaveItem = scene->addLine(xx + dx, yy + dy, ax2, ay2, pen);
                swellWaveArrowPlot.push_back(swellWaveItem);

                 i++;
            }
            j++;
        }
    }

    else
    {
        int j = 0;
        for (int y = yMin; y <= yMax && j < Nj; y+=Dj)
        {
            int i = 0;
            int skipped = 0;
            for (int x = xMin; x <= 180; x+=Di)
            {
                double h = sWvHt[i][j];
                if (h == 0) {
                    i++;
                    continue;
                }
                double ang = (sWvDir[i][j]) * M_PI / 180.0;   // angle of arrow with +ve x axis in scene's ref
                double dx = arrowSize*qCos(ang);         // projection of length of arrow on x - axis (in scene's ref)
                double dy = arrowSize*qSin(ang);         // projection of length of arrow on y - axis      ,,
                double xx = x*stepi;   // starting coordinates of the arrow
                double yy = -toMerc(y)*stepj;

                QGraphicsItem* swellWaveItem = scene->addLine(xx, yy, xx + dx, yy + dy, pen);
                swellWaveArrowPlot.push_back(swellWaveItem);

                //arrow heads
                double ax1 = xx + dx - qCos(ang - M_PI / 6) * arrowSize / 5;
                double ay1 = yy + dy - qSin(ang - M_PI / 6) * arrowSize / 5;
                double ax2 = xx + dx - qSin(M_PI / 3 - ang) * arrowSize / 5;
                double ay2 = yy + dy - qCos(M_PI / 3 - ang) * arrowSize / 5;

                swellWaveItem = scene->addLine(xx + dx, yy + dy, ax1, ay1, pen);
                swellWaveArrowPlot.push_back(swellWaveItem);

                swellWaveItem = scene->addLine(xx + dx, yy + dy, ax2, ay2, pen);
                swellWaveArrowPlot.push_back(swellWaveItem);

                i++;
                if (x + Di > 180) {
                    skipped = 180 - x;
                }
            }

            int skip = Di - 1 - skipped;

            for (int x = -179 + skip; x <= xMax && i < Ni; x+=Di)
            {
                double h = sWvHt[i][j];
                if (h == 0) {
                    i++;
                    continue;
                }
                double ang = (sWvDir[i][j]) * M_PI / 180.0;  // angle of arrow with +ve x axis in scene's ref
                double dx = arrowSize*qCos(ang);         // projection of length of arrow on x - axis (in scene's ref)
                double dy = arrowSize*qSin(ang);         // projection of length of arrow on y - axis      ,,
                double xx = x*stepi;   // starting coordinates of the arrow
                double yy = -toMerc(y)*stepj;

                QGraphicsItem* swellWaveItem = scene->addLine(xx, yy, xx + dx, yy + dy, pen);
                swellWaveArrowPlot.push_back(swellWaveItem);

                //arrow heads
                double ax1 = xx + dx - qCos(ang - M_PI / 6) * arrowSize / 5;
                double ay1 = yy + dy - qSin(ang - M_PI / 6) * arrowSize / 5;
                double ax2 = xx + dx - qSin(M_PI / 3 - ang) * arrowSize / 5;
                double ay2 = yy + dy - qCos(M_PI / 3 - ang) * arrowSize / 5;

                swellWaveItem = scene->addLine(xx + dx, yy + dy, ax1, ay1, pen);
                swellWaveArrowPlot.push_back(swellWaveItem);

                swellWaveItem = scene->addLine(xx + dx, yy + dy, ax2, ay2, pen);
                swellWaveArrowPlot.push_back(swellWaveItem);

                i++;
            }
            j++;
        }
    }
    areInitSwellWaveArrowsDrawn = true;
}

void MainWindow::drawCurrentArrow(int fc) {

    vector<vector<double>> cSpeed = gridData->getCurrentSpeed(fc);
    vector<vector<int>> cDir = gridData->getCurrentDir(fc);

    if (cSpeed.size() == 0 || cDir.size() == 0) {
        return;
    }

    double Di = gridData->getD();
    double Dj = gridData->getD();
    int xMin = gridData->getMinPoint().x();
    int yMin = gridData->getMinPoint().y();
    int xMax = gridData->getMaxPoint().x();
    int yMax = gridData->getMaxPoint().y();
    int Ni = gridData->getNi();
    int Nj = gridData->getNj();

    CustomScene* scene = m_pForm->GetGraphicsScene();

    double stepi = scene->stepI();
    double stepj = scene->stepJ();

    // arrow properties
    QPen pen = QPen(Qt::white, 1);
    pen.setCosmetic(true);
    double arrowSize = stepi;

    if (xMin < xMax)
    {
        int j = 0;
        for (int y = yMin; y <= yMax && j < Nj; y += Dj)
        {
            int i = 0;
            for (int x = xMin; x <= xMax && i < Ni; x += Di)
            {
                double vInKnots = 1.94384 * cSpeed[i][j];
                if (vInKnots == 0) {
                    i++;
                    continue;
                }
                double ang = (cDir[i][j]) * M_PI / 180.0;  // angle of arrow with +ve x axis in scene's ref
                double dx = arrowSize * qCos(ang);         // proj of arrow on x - axis (in scene's ref)
                double dy = arrowSize * qSin(ang);         // proj of arrow on y - axis      ,,
                double xx = x * stepi;   // starting coordinates of the arrow
                double yy = -toMerc(y) * stepj;

                QGraphicsItem* currentItem = scene->addLine(xx, yy, xx + dx, yy + dy, pen);
                currentArrowPlot.push_back(currentItem);

                //arrow heads
                double ax1 = xx + dx - qCos(ang - M_PI / 6) * arrowSize / 5;
                double ay1 = yy + dy - qSin(ang - M_PI / 6) * arrowSize / 5;
                double ax2 = xx + dx - qSin(M_PI / 3 - ang) * arrowSize / 5;
                double ay2 = yy + dy - qCos(M_PI / 3 - ang) * arrowSize / 5;

                currentItem = scene->addLine(xx + dx, yy + dy, ax1, ay1, pen);
                currentArrowPlot.push_back(currentItem);

                currentItem = scene->addLine(xx + dx, yy + dy, ax2, ay2, pen);
                currentArrowPlot.push_back(currentItem);


                //barbs
                int vApprox = (int)(std::floor(vInKnots*10));

                currentItem = scene->addLine(xx, yy, xx + dx, yy + dy, pen);
                currentArrowPlot.push_back(currentItem);

                double d = arrowSize / 10;
                double bx = xx + d*qCos(ang);
                double by = yy + d*qSin(ang);
                int noOfbarbs = vApprox/5;

                for (int i = 0; i < noOfbarbs; i++)
                {
                    QPointF be = barb(ang, arrowSize, true);

                    currentItem = scene->addLine(bx, by, bx + be.x(), by + be.y(), pen);
                    currentArrowPlot.push_back(currentItem);

                    bx += d*qCos(ang);
                    by += d*qSin(ang);
                }
                i++;
            }
            j++;
        }
    }

    else
    {
        int j = 0;
        for (int y = yMin; y <= yMax && j < Nj; y += Dj)
        {
            int i = 0;
            int skipped = 0;
            for (int x = xMin; x <= 180 && i < Ni; x += Di)
            {
                double vInKnots = 1.94384 * (cSpeed[i][j]);
                if (vInKnots == 0) {
                    i++;
                    continue;
                }
                double ang = (cDir[i][j]) * M_PI / 180.0;   // ang of arrow with +ve x axis in scene's ref
                double dx = arrowSize*qCos(ang);         // proj of arrow on x - axis (in scene's ref)
                double dy = arrowSize*qSin(ang);         // proj of arrow on y - axis      ,,
                double xx = x*stepi;                     // starting coordinates of the arrow
                double yy = -toMerc(y)*stepj;

                QGraphicsItem* currentItem = scene->addLine(xx, yy, xx + dx, yy + dy, pen);
                currentArrowPlot.push_back(currentItem);


                //arrow heads
                double ax1 = xx + dx - qCos(ang - M_PI / 6) * arrowSize / 5;
                double ay1 = yy + dy - qSin(ang - M_PI / 6) * arrowSize / 5;
                double ax2 = xx + dx - qSin(M_PI / 3 - ang) * arrowSize / 5;
                double ay2 = yy + dy - qCos(M_PI / 3 - ang) * arrowSize / 5;

                currentItem = scene->addLine(xx + dx, yy + dy, ax1, ay1, pen);
                currentArrowPlot.push_back(currentItem);

                currentItem = scene->addLine(xx + dx, yy + dy, ax2, ay2, pen);
                currentArrowPlot.push_back(currentItem);

                //barbs
                int vApprox = (int)(std::floor(vInKnots*10));

                currentItem = scene->addLine(xx, yy, xx + dx, yy + dy, pen);
                currentArrowPlot.push_back(currentItem);

                double d = arrowSize / 10;
                double bx = xx + d*qCos(ang);
                double by = yy + d*qSin(ang);
                int noOfbarbs = vApprox/5;

                for (int i = 0; i < noOfbarbs; i++)
                {
                    QPointF be = barb(ang, arrowSize, true);

                    currentItem = scene->addLine(bx, by, bx + be.x(), by + be.y(), pen);
                    currentArrowPlot.push_back(currentItem);

                    bx += d*qCos(ang);
                    by += d*qSin(ang);
                }

                i++;
                if (x + Di > 180) {
                    skipped = 180 - x;
                }
            }

            int skip = Di - 1 - skipped;

            for (int x = -179 + skip; x <= xMax && i < Ni; x += Di)
            {
                double vInKnots = 1.94384 * (cSpeed[i][j]);
                if (vInKnots == 0) {
                    i++;
                    continue;
                }
                double ang = (cDir[i][j]) * M_PI / 180.0;  // angle of arrow with +ve x axis in scene's ref
                double dx = arrowSize*qCos(ang);         // projection of length of arrow on x - axis (in scene's ref)
                double dy = arrowSize*qSin(ang);         // projection of length of arrow on y - axis      ,,
                double xx = x*stepi;   // starting coordinates of the arrow
                double yy = -toMerc(y)*stepj;

                QGraphicsItem* currentItem = scene->addLine(xx, yy, xx + dx, yy + dy, pen);
                currentArrowPlot.push_back(currentItem);


                //arrow heads
                double ax1 = xx + dx - qCos(ang - M_PI / 6) * arrowSize / 5;
                double ay1 = yy + dy - qSin(ang - M_PI / 6) * arrowSize / 5;
                double ax2 = xx + dx - qSin(M_PI / 3 - ang) * arrowSize / 5;
                double ay2 = yy + dy - qCos(M_PI / 3 - ang) * arrowSize / 5;

                currentItem = scene->addLine(xx + dx, yy + dy, ax1, ay1, pen);
                currentArrowPlot.push_back(currentItem);

                currentItem = scene->addLine(xx + dx, yy + dy, ax2, ay2, pen);
                currentArrowPlot.push_back(currentItem);

                //barbs
                int vApprox = (int)(std::floor(vInKnots*10));

                currentItem = scene->addLine(xx, yy, xx + dx, yy + dy, pen);
                currentArrowPlot.push_back(currentItem);

                double d = arrowSize / 10;
                double bx = xx + d*qCos(ang);
                double by = yy + d*qSin(ang);
                int noOfbarbs = vApprox/5;

                for (int i = 0; i < noOfbarbs; i++)
                {
                    QPointF be = barb(ang, arrowSize, true);

                    currentItem = scene->addLine(bx, by, bx + be.x(), by + be.y(), pen);
                    currentArrowPlot.push_back(currentItem);

                    bx += d*qCos(ang);
                    by += d*qSin(ang);
                }

                i++;
            }
            j++;
        }
    }
    areInitCurrentArrowsDrawn = true;
}



