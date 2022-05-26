#include "plotter.h"
#include <iostream>

Plotter::Plotter(Gridder* gridder)
{
    m_gridder = gridder;
    initPlotter();
}

void Plotter::initPlotter ()
{
    isInitWindSpeedDrawn = false;
    areInitWindArrowsDrawn = false;
    isInitWaveSigHtDrawn = false;
    isInitWindWaveHtDrawn = false;
    areInitWindWaveArrowsDrawn = false;
    isInitSwellWaveHtDrawn = false;
    areInitSwellWaveArrowsDrawn = false;
    areInitCurrentArrowsDrawn = false;
    areInitIsobarsDrawn = false;
}

void Plotter::setScene (CustomScene* scene)
{
    m_scene = scene;
}


void Plotter::addWindColorPlot()
{
    if (!isInitWindSpeedDrawn)
    {
        drawWindColor();
        isInitWindSpeedDrawn = true;
    }
    else
    {
        int mapXmax = m_scene->xExtent();
        int mapYmax = m_scene->yExtent();

        windColorPlot->setOffset(-mapXmax, -mapYmax);
        windColorPlot->setPos(0, 0);
        windColorPlot->setZValue(-1);
        m_scene->addItem(windColorPlot);
    }
}

void Plotter::addWindArrowPlot ()
{
    if (!areInitWindArrowsDrawn)
    {
        drawWindArrow();
        areInitWindArrowsDrawn = true;
    }
    else
    {
        for (auto it : windArrowPlot) {
            m_scene->addItem(it);
        }
    }
}

void Plotter::addWaveSigHtColorPlot()
{
    if (!isInitWaveSigHtDrawn)
    {
        drawWaveSigHtColor();
        isInitWaveSigHtDrawn = true;
    }
    else
    {
        int mapXmax = m_scene->xExtent();
        int mapYmax = m_scene->yExtent();

        waveSigHtColorPlot->setOffset(-mapXmax, -mapYmax);
        waveSigHtColorPlot->setPos(0, 0);
        waveSigHtColorPlot->setZValue(-1);
        m_scene->addItem(waveSigHtColorPlot);
    }
}

void Plotter::addWindWaveHtColorPlot()
{
    if (!isInitWindWaveHtDrawn)
    {
        drawWindWaveHtColor();
        isInitWindWaveHtDrawn = true;
    }
    else
    {
        int mapXmax = m_scene->xExtent();
        int mapYmax = m_scene->yExtent();

        windWaveHtColorPlot->setOffset(-mapXmax, -mapYmax);
        windWaveHtColorPlot->setPos(0, 0);
        windWaveHtColorPlot->setZValue(-1);
        m_scene->addItem(windWaveHtColorPlot);
    }
}
void Plotter::addWindWaveArrowPlot()
{
    if (!areInitWindWaveArrowsDrawn)
    {
        drawWindWaveArrow();
        areInitWindWaveArrowsDrawn = true;
    }
    else
    {
        for (auto it : windWaveArrowPlot) {
            m_scene->addItem(it);
        }
    }
}

void Plotter::addSwellWaveHtColorPlot()
{
    if (!isInitSwellWaveHtDrawn)
    {
        drawSwellWaveHtColor();
        isInitSwellWaveHtDrawn = true;
    }
    else
    {
        int mapXmax = m_scene->xExtent();
        int mapYmax = m_scene->yExtent();

        swellWaveHtColorPlot->setOffset(-mapXmax, -mapYmax);
        swellWaveHtColorPlot->setPos(0, 0);
        swellWaveHtColorPlot->setZValue(-1);
        m_scene->addItem(swellWaveHtColorPlot);
    }
}

void Plotter::addSwellWaveArrowPlot()
{
    if (!areInitSwellWaveArrowsDrawn)
    {
        drawSwellWaveArrow();
        areInitSwellWaveArrowsDrawn = true;
    }
    else
    {
        for (auto it : swellWaveArrowPlot) {
            m_scene->addItem(it);
        }
    }
}

void Plotter::addCurrentArrowPlot()
{
    if (!areInitCurrentArrowsDrawn)
    {
        drawCurrentArrow();
        areInitCurrentArrowsDrawn = true;
    }
    else
    {
        for (auto it : currentArrowPlot) {
            m_scene->addItem(it);
        }
    }
}


void Plotter::addIsobarPlot()
{
    if (!areInitIsobarsDrawn)
    {
        drawIsobars();
        areInitIsobarsDrawn = true;
    }
    else
    {
        for (auto it : isobarPlot) {
            m_scene->addItem(it);
        }
    }
}

/* remove funcs */
void Plotter::removeWindArrowPlot()
{
    for (auto it : windArrowPlot) {
        m_scene->removeItem(it);
    }
}

void Plotter::removeWindColorPlot()
{
    qInfo() << "wind clr remove\n";

    m_scene->removeItem(windColorPlot);
}

void Plotter::removeWaveSigHtColorPlot()
{
    m_scene->removeItem(waveSigHtColorPlot);
}

void Plotter::removeWindWaveHtColorPlot()
{
    m_scene->removeItem(windWaveHtColorPlot);
}

void Plotter::removeSwellWaveHtColorPlot()
{
    m_scene->removeItem(swellWaveHtColorPlot);
}

void Plotter::removeWindWaveArrowPlot()
{
    for (auto it : windWaveArrowPlot) {
        m_scene->removeItem(it);
    }
}

void Plotter::removeSwellWaveArrowPlot()
{
    for (auto it : swellWaveArrowPlot) {
        m_scene->removeItem(it);
    }
}

void Plotter::removeCurrentArrowPlot()
{
    for (auto it : currentArrowPlot) {
        m_scene->removeItem(it);
    }
}

void Plotter::removeIsobarPlot()
{
    for (auto it : isobarPlot) {
        m_scene->removeItem(it);
    }
}


void Plotter::drawWindArrow ()
{
    vector<vector<double>> wSpeed = m_gridder->windSpeed;
    vector<vector<int>> wDir = m_gridder->windDir;

    if (wSpeed.size() == 0 || wDir.size() == 0) {
        return;
    }

    double Di = m_gridder->Di;
    double Dj = m_gridder->Dj;
    int xMin = m_gridder->minPoint.x();
    int yMin = m_gridder->minPoint.y();
    int xMax = m_gridder->maxPoint.x();
    int yMax = m_gridder->maxPoint.y();
    int Ni = m_gridder->Ni;
    int Nj = m_gridder->Nj;

    double stepi = m_scene->stepI();
    double stepj = m_scene->stepJ();

    // arrow properties
    double arrowSize = stepj / 2;

    QPen pen = QPen(Qt::black, 0.07);
    pen.setCapStyle(Qt::RoundCap);

    if (xMin < xMax)
    {
        int j = 0;
        for (int y = yMin; y <= yMax && j < Nj; y += Dj)
        {
            int i = 0;
            for (int x = xMin; x <= xMax && i < Ni; x += Di)
            {
                double vInKnots = 1.94384 * (wSpeed[i][j]);
                double ang = (wDir[i][j]) * M_PI / 180.0;  // ang of arrow with +ve x axis in m_scene's ref
                double dx = arrowSize * qCos(ang);                // proj of arrow on x axis (in m_scene's ref)
                double dy = arrowSize * qSin(ang);                // proj of arrow on y axis (in m_scene's ref)
                double xx = x * stepi;                            // starting coordinates of the arrow
                double yy = -toMerc(y) * stepj;

                // barbs
                int vApprox = (int)(std::floor(vInKnots));

                if (vApprox == 0)
                {
                    double rads = 1;
                    QGraphicsItem* windItem = m_scene -> addEllipse(xx - rads, yy - rads, rads*2.0, rads*2.0, pen);
                    windArrowPlot.push_back(windItem);

                    double rad = 2;
                    windItem = m_scene -> addEllipse(xx - rad, yy - rad, rad*2.0, rad*2.0, pen);
                    windArrowPlot.push_back(windItem);
                }
                else if (vApprox < 50)
                {
                    QGraphicsItem* windItem = m_scene -> addLine(xx, yy, xx + dx, yy + dy, pen);
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

                        windItem = m_scene -> addLine(bx, by, bx + be.x(), by + be.y(), pen);
                        windArrowPlot.push_back(windItem);

                        bx += d*qCos(ang);
                        by += d*qSin(ang);
                    }

                    for (int i = 0; i < noOfSmallBarbs; i++)
                    {
                        QPointF be = barb(ang, arrowSize, true);

                        windItem = m_scene -> addLine(bx, by, bx + be.x(), by + be.y(), pen);
                        windArrowPlot.push_back(windItem);
                    }
                }
                else
                {
                    QGraphicsItem* windItem = m_scene -> addLine(xx, yy, xx + dx, yy + dy, pen);
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

                        windItem = m_scene->addLine(bx, by, bx + p.first.x(), by + p.first.y(), pen);
                        windArrowPlot.push_back(windItem);

                        windItem = m_scene->addLine(bx + p.second.x(), by + p.second.y(), bx + p.first.x(), by + p.first.y(), pen);
                        windArrowPlot.push_back(windItem);

                        bx = p.second.x();
                        by = p.second.y();
                    }

                    bx += d*qCos(ang);
                    by += d*qSin(ang);

                    for (int i = 0; i < noOfBigBarbs; i++)
                    {
                        QPointF be = barb(ang, arrowSize);

                        windItem = m_scene -> addLine(bx, by, bx + be.x(), by + be.y(), pen);
                        windArrowPlot.push_back(windItem);

                        bx += d*qCos(ang);
                        by += d*qSin(ang);
                    }

                    for (int i = 0; i < noOfSmallBarbs; i++)
                    {
                        QPointF be = barb(ang, arrowSize, true);

                        windItem = m_scene -> addLine(bx, by, bx + be.x(), by + be.y(), pen);
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
                double ang = (wDir[i][j]) * M_PI / 180.0;   // angle of arrow with +ve x axis in m_scene's ref
                double dx = arrowSize*qCos(ang);         // projection of length of arrow on x - axis (in m_scene's ref)
                double dy = arrowSize*qSin(ang);         // projection of length of arrow on y - axis      ,,
                double xx = x*stepi;   // starting coordinates of the arrow
                double yy = -toMerc(y)*stepj;

                // barbs
                int vApprox = (int)(std::floor(vInKnots));

                if (vApprox == 0)
                {
                    double rads = 1;
                    QGraphicsItem* windItem = m_scene -> addEllipse(xx - rads, yy - rads, rads*2.0, rads*2.0, pen);
                    windArrowPlot.push_back(windItem);

                    double rad = 2;
                    windItem = m_scene -> addEllipse(xx - rad, yy - rad, rad*2.0, rad*2.0, pen);
                    windArrowPlot.push_back(windItem);
                }
                else if (vApprox < 50)
                {
                    QGraphicsItem* windItem = m_scene -> addLine(xx, yy, xx + dx, yy + dy, pen);
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

                        windItem = m_scene -> addLine(bx, by, bx + be.x(), by + be.y(), pen);
                        windArrowPlot.push_back(windItem);

                        bx += d*qCos(ang);
                        by += d*qSin(ang);
                    }

                    for (int i = 0; i < noOfSmallBarbs; i++)
                    {
                        QPointF be = barb(ang, arrowSize, true);

                        windItem = m_scene -> addLine(bx, by, bx + be.x(), by + be.y(), pen);
                        windArrowPlot.push_back(windItem);
                    }
                }
                else
                {
                    QGraphicsItem* windItem = m_scene -> addLine(xx, yy, xx + dx, yy + dy, pen);
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

                        windItem = m_scene->addLine(bx, by, bx + p.first.x(), by + p.first.y(), pen);
                        windArrowPlot.push_back(windItem);

                        windItem = m_scene->addLine(bx + p.second.x(), by + p.second.y(), bx + p.first.x(), by + p.first.y(), pen);
                        windArrowPlot.push_back(windItem);

                        bx = p.second.x();
                        by = p.second.y();
                    }

                    bx += d*qCos(ang);
                    by += d*qSin(ang);

                    for (int i = 0; i < noOfBigBarbs; i++)
                    {
                        QPointF be = barb(ang, arrowSize);

                        windItem = m_scene -> addLine(bx, by, bx + be.x(), by + be.y(), pen);
                        windArrowPlot.push_back(windItem);

                        bx += d*qCos(ang);
                        by += d*qSin(ang);
                    }

                    for (int i = 0; i < noOfSmallBarbs; i++)
                    {
                        QPointF be = barb(ang, arrowSize, true);

                        windItem = m_scene -> addLine(bx, by, bx + be.x(), by + be.y(), pen);
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
                double ang = (wDir[i][j]) * M_PI / 180.0;  // angle of arrow with +ve x axis in m_scene's ref
                double dx = arrowSize*qCos(ang);         // projection of length of arrow on x - axis (in m_scene's ref)
                double dy = arrowSize*qSin(ang);         // projection of length of arrow on y - axis      ,,
                double xx = x*stepi;   // starting coordinates of the arrow
                double yy = -toMerc(y)*stepj;

                // barbs
                int vApprox = (int)(std::floor(vInKnots));

                if (vApprox == 0)
                {
                    double rads = 1;
                    QGraphicsItem* windItem = m_scene -> addEllipse(xx - rads, yy - rads, rads*2.0, rads*2.0, pen);
                    windArrowPlot.push_back(windItem);

                    double rad = 2;
                    windItem = m_scene -> addEllipse(xx - rad, yy - rad, rad*2.0, rad*2.0, pen);
                    windArrowPlot.push_back(windItem);
                }
                else if (vApprox < 50)
                {
                    QGraphicsItem* windItem = m_scene -> addLine(xx, yy, xx + dx, yy + dy, pen);
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

                        windItem = m_scene -> addLine(bx, by, bx + be.x(), by + be.y(), pen);
                        windArrowPlot.push_back(windItem);

                        bx += d*qCos(ang);
                        by += d*qSin(ang);
                    }

                    for (int i = 0; i < noOfSmallBarbs; i++)
                    {
                        QPointF be = barb(ang, arrowSize, true);

                        windItem = m_scene -> addLine(bx, by, bx + be.x(), by + be.y(), pen);
                        windArrowPlot.push_back(windItem);
                    }
                }
                else
                {
                    QGraphicsItem* windItem = m_scene -> addLine(xx, yy, xx + dx, yy + dy, pen);
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

                        windItem = m_scene->addLine(bx, by, bx + p.first.x(), by + p.first.y(), pen);
                        windArrowPlot.push_back(windItem);

                        windItem = m_scene->addLine(bx + p.second.x(), by + p.second.y(), bx + p.first.x(), by + p.first.y(), pen);
                        windArrowPlot.push_back(windItem);

                        bx = p.second.x();
                        by = p.second.y();
                    }

                    bx += d*qCos(ang);
                    by += d*qSin(ang);

                    for (int i = 0; i < noOfBigBarbs; i++)
                    {
                        QPointF be = barb(ang, arrowSize);

                        windItem = m_scene -> addLine(bx, by, bx + be.x(), by + be.y(), pen);
                        windArrowPlot.push_back(windItem);

                        bx += d*qCos(ang);
                        by += d*qSin(ang);
                    }

                    for (int i = 0; i < noOfSmallBarbs; i++)
                    {
                        QPointF be = barb(ang, arrowSize, true);

                        windItem = m_scene -> addLine(bx, by, bx + be.x(), by + be.y(), pen);
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
void Plotter::drawWindColor ()
{
    vector<vector<double>> wSpeed = m_gridder->windSpeed;

    if (wSpeed.size() == 0) {
        return;
    }

    double Di = m_gridder->Di;
    double Dj = m_gridder->Dj;
    int xMin = m_gridder->minPoint.x();
    int yMin = m_gridder->minPoint.y();
    int xMax = m_gridder->maxPoint.x();
    int yMax = m_gridder->maxPoint.y();
    int Ni = m_gridder->Ni;
    int Nj = m_gridder->Nj;


    double stepi = m_scene->stepI();
    double stepj = m_scene->stepJ();

    int mapXmax = m_scene->xExtent();
    int mapYmax = m_scene->yExtent();

    std::unordered_map<int, QColor> m = m_scene->GetWindColourScale();  // wind colour scale



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

    m_scene -> addItem(windColorPlot);

    isInitWindSpeedDrawn = true;
}


void Plotter::drawWaveSigHtColor ()
{
    vector<vector<double>> wvSigHt = m_gridder->waveSigHeight;

    if (wvSigHt.size() == 0) {
        return;
    }

    double Di = m_gridder->Di;
    double Dj = m_gridder->Dj;
    int xMin = m_gridder->minPoint.x();
    int yMin = m_gridder->minPoint.y();
    int xMax = m_gridder->maxPoint.x();
    int yMax = m_gridder->maxPoint.y();
    int Ni = m_gridder->Ni;
    int Nj = m_gridder->Nj;


    double stepi = m_scene->stepI();
    double stepj = m_scene->stepJ();

    m_scene->clearInitialGrid();

    int mapXmax = m_scene->xExtent();
    int mapYmax = m_scene->yExtent();

    std::unordered_map<int, QColor> m = m_scene->GetWaveColourScale();  // wave colour scale



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
    m_scene->addItem(waveSigHtColorPlot);

    isInitWaveSigHtDrawn = true;
}
void Plotter::drawWindWaveHtColor ()
{
    vector<vector<double>> wWvHt = m_gridder->windWaveHeight;

    if (wWvHt.size() == 0) {
        return;
    }

    double Di = m_gridder->Di;
    double Dj = m_gridder->Dj;
    int xMin = m_gridder->minPoint.x();
    int yMin = m_gridder->minPoint.y();
    int xMax = m_gridder->maxPoint.x();
    int yMax = m_gridder->maxPoint.y();
    int Ni = m_gridder->Ni;
    int Nj = m_gridder->Nj;


    double stepi = m_scene->stepI();
    double stepj = m_scene->stepJ();

    m_scene->clearInitialGrid();

    int mapXmax = m_scene->xExtent();
    int mapYmax = m_scene->yExtent();

    std::unordered_map<int, QColor> m = m_scene->GetWaveColourScale();  // wind colour scale



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

    m_scene->addItem(windWaveHtColorPlot);

    isInitWindWaveHtDrawn = true;
}


void Plotter::drawSwellWaveHtColor ()
{
    vector<vector<double>> sWvHt = m_gridder->swellWaveHeight;

    if (sWvHt.size() == 0) {
        return;
    }

    double Di = m_gridder->Di;
    double Dj = m_gridder->Dj;
    int xMin = m_gridder->minPoint.x();
    int yMin = m_gridder->minPoint.y();
    int xMax = m_gridder->maxPoint.x();
    int yMax = m_gridder->maxPoint.y();
    int Ni = m_gridder->Ni;
    int Nj = m_gridder->Nj;


    double stepi = m_scene->stepI();
    double stepj = m_scene->stepJ();

    int mapXmax = m_scene->xExtent();
    int mapYmax = m_scene->yExtent();

    std::unordered_map<int, QColor> m = m_scene->GetWaveColourScale();  // wind colour scale



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

    m_scene->addItem(swellWaveHtColorPlot);

    isInitSwellWaveHtDrawn = true;
}


void Plotter::drawWindWaveArrow ()
{
    vector<vector<double>> wWvHt = m_gridder->windWaveHeight;
    vector<vector<int>> wWvDir = m_gridder->windWaveDir;

    if (wWvHt.size() == 0 || wWvDir.size() == 0) {
        return;
    }

    double Di = m_gridder->Di;
    double Dj = m_gridder->Dj;
    int xMin = m_gridder->minPoint.x();
    int yMin = m_gridder->minPoint.y();
    int xMax = m_gridder->maxPoint.x();
    int yMax = m_gridder->maxPoint.y();
    int Ni = m_gridder->Ni;
    int Nj = m_gridder->Nj;




    double stepi = m_scene->stepI();
    double stepj = m_scene->stepJ();

    // arrow properties
    QPen pen = QPen(Qt::blue, 2);
    pen.setCapStyle(Qt::RoundCap);
    pen.setCosmetic(true);
    double arrowSize = stepj / 2;


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
                double ang = (90 - (wWvDir[i][j])) * M_PI / 180.0;  // angle of arrow with +ve x axis in m_scene's ref
                double dx = arrowSize * qCos(ang);         // projection of length of arrow on x - axis (in m_scene's ref)
                double dy = arrowSize * qSin(ang);         // projection of length of arrow on y - axis      ,,
                double xx = x * stepi;                      // starting coordinates of the arrow
                double yy = -toMerc(y) * stepj;

                QGraphicsItem* windWaveItem = m_scene->addLine(xx, yy, xx + dx, yy + dy, pen);
                windWaveArrowPlot.push_back(windWaveItem);

                //arrow heads
                double ax1 = xx + dx - qCos(ang - M_PI / 6) * arrowSize / 5;
                double ay1 = yy + dy - qSin(ang - M_PI / 6) * arrowSize / 5;
                double ax2 = xx + dx - qSin(M_PI / 3 - ang) * arrowSize / 5;
                double ay2 = yy + dy - qCos(M_PI / 3 - ang) * arrowSize / 5;

                windWaveItem = m_scene->addLine(xx + dx, yy + dy, ax1, ay1, pen);
                windWaveArrowPlot.push_back(windWaveItem);

                windWaveItem = m_scene->addLine(xx + dx, yy + dy, ax2, ay2, pen);
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
                double ang = (wWvDir[i][j]) * M_PI / 180.0;   // angle of arrow with +ve x axis in m_scene's ref
                double dx = arrowSize*qCos(ang);         // projection of length of arrow on x - axis (in m_scene's ref)
                double dy = arrowSize*qSin(ang);         // projection of length of arrow on y - axis      ,,
                double xx = x*stepi;   // starting coordinates of the arrow
                double yy = -toMerc(y)*stepj;

                QGraphicsItem* windWaveItem = m_scene->addLine(xx, yy, xx + dx, yy + dy, pen);
                windWaveArrowPlot.push_back(windWaveItem);

                //arrow heads
                double ax1 = xx + dx - qCos(ang - M_PI / 6) * arrowSize / 5;
                double ay1 = yy + dy - qSin(ang - M_PI / 6) * arrowSize / 5;
                double ax2 = xx + dx - qSin(M_PI / 3 - ang) * arrowSize / 5;
                double ay2 = yy + dy - qCos(M_PI / 3 - ang) * arrowSize / 5;

                windWaveItem = m_scene->addLine(xx + dx, yy + dy, ax1, ay1, pen);
                windWaveArrowPlot.push_back(windWaveItem);

                windWaveItem = m_scene->addLine(xx + dx, yy + dy, ax2, ay2, pen);
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
                double ang = (wWvDir[i][j]) * M_PI / 180.0;  // angle of arrow with +ve x axis in m_scene's ref
                double dx = arrowSize*qCos(ang);         // projection of length of arrow on x - axis (in m_scene's ref)
                double dy = arrowSize*qSin(ang);         // projection of length of arrow on y - axis      ,,
                double xx = x*stepi;   // starting coordinates of the arrow
                double yy = -toMerc(y)*stepj;

                QGraphicsItem* windWaveItem = m_scene->addLine(xx, yy, xx + dx, yy + dy, pen);
                windWaveArrowPlot.push_back(windWaveItem);

                //arrow heads
                double ax1 = xx + dx - qCos(ang - M_PI / 6) * arrowSize / 5;
                double ay1 = yy + dy - qSin(ang - M_PI / 6) * arrowSize / 5;
                double ax2 = xx + dx - qSin(M_PI / 3 - ang) * arrowSize / 5;
                double ay2 = yy + dy - qCos(M_PI / 3 - ang) * arrowSize / 5;

                windWaveItem = m_scene->addLine(xx + dx, yy + dy, ax1, ay1, pen);
                windWaveArrowPlot.push_back(windWaveItem);

                windWaveItem = m_scene->addLine(xx + dx, yy + dy, ax2, ay2, pen);
                windWaveArrowPlot.push_back(windWaveItem);

                i++;
            }
            j++;
        }
    }

    areInitWindArrowsDrawn = true;
}


void Plotter::drawSwellWaveArrow ()
{
    vector<vector<double>> sWvHt = m_gridder->swellWaveHeight;
    vector<vector<int>> sWvDir = m_gridder->swellWaveDir;

    if (sWvHt.size() == 0 || sWvDir.size() == 0) {
        return;
    }

    double Di = m_gridder->Di;
    double Dj = m_gridder->Dj;
    int xMin = m_gridder->minPoint.x();
    int yMin = m_gridder->minPoint.y();
    int xMax = m_gridder->maxPoint.x();
    int yMax = m_gridder->maxPoint.y();
    int Ni = m_gridder->Ni;
    int Nj = m_gridder->Nj;



    double stepi = m_scene->stepI();
    double stepj = m_scene->stepJ();

    // arrow properties
    QPen pen = QPen(Qt::magenta, 2);
    pen.setCapStyle(Qt::RoundCap);
    pen.setCosmetic(true);
    double arrowSize = stepj / 2;

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
                double ang = (sWvDir[i][j]) * M_PI / 180.0;  // angle of arrow with +ve x axis in m_scene's ref
                double dx = arrowSize * qCos(ang);         // projection of length of arrow on x - axis (in m_scene's ref)
                double dy = arrowSize * qSin(ang);         // projection of length of arrow on y - axis      ,,
                double xx = x * stepi;   // starting coordinates of the arrow
                double yy = -toMerc(y) * stepj;

                QGraphicsItem* swellWaveItem = m_scene->addLine(xx, yy, xx + dx, yy + dy, pen);
                swellWaveArrowPlot.push_back(swellWaveItem);

                //arrow heads
                double ax1 = xx + dx - qCos(ang - M_PI / 6) * arrowSize / 5;
                double ay1 = yy + dy - qSin(ang - M_PI / 6) * arrowSize / 5;
                double ax2 = xx + dx - qSin(M_PI / 3 - ang) * arrowSize / 5;
                double ay2 = yy + dy - qCos(M_PI / 3 - ang) * arrowSize / 5;

                swellWaveItem = m_scene->addLine(xx + dx, yy + dy, ax1, ay1, pen);
                swellWaveArrowPlot.push_back(swellWaveItem);

                swellWaveItem = m_scene->addLine(xx + dx, yy + dy, ax2, ay2, pen);
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
                double ang = (sWvDir[i][j]) * M_PI / 180.0;   // angle of arrow with +ve x axis in m_scene's ref
                double dx = arrowSize*qCos(ang);         // projection of length of arrow on x - axis (in m_scene's ref)
                double dy = arrowSize*qSin(ang);         // projection of length of arrow on y - axis      ,,
                double xx = x*stepi;   // starting coordinates of the arrow
                double yy = -toMerc(y)*stepj;

                QGraphicsItem* swellWaveItem = m_scene->addLine(xx, yy, xx + dx, yy + dy, pen);
                swellWaveArrowPlot.push_back(swellWaveItem);

                //arrow heads
                double ax1 = xx + dx - qCos(ang - M_PI / 6) * arrowSize / 5;
                double ay1 = yy + dy - qSin(ang - M_PI / 6) * arrowSize / 5;
                double ax2 = xx + dx - qSin(M_PI / 3 - ang) * arrowSize / 5;
                double ay2 = yy + dy - qCos(M_PI / 3 - ang) * arrowSize / 5;

                swellWaveItem = m_scene->addLine(xx + dx, yy + dy, ax1, ay1, pen);
                swellWaveArrowPlot.push_back(swellWaveItem);

                swellWaveItem = m_scene->addLine(xx + dx, yy + dy, ax2, ay2, pen);
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
                double ang = (sWvDir[i][j]) * M_PI / 180.0;  // angle of arrow with +ve x axis in m_scene's ref
                double dx = arrowSize*qCos(ang);         // projection of length of arrow on x - axis (in m_scene's ref)
                double dy = arrowSize*qSin(ang);         // projection of length of arrow on y - axis      ,,
                double xx = x*stepi;   // starting coordinates of the arrow
                double yy = -toMerc(y)*stepj;

                QGraphicsItem* swellWaveItem = m_scene->addLine(xx, yy, xx + dx, yy + dy, pen);
                swellWaveArrowPlot.push_back(swellWaveItem);

                //arrow heads
                double ax1 = xx + dx - qCos(ang - M_PI / 6) * arrowSize / 5;
                double ay1 = yy + dy - qSin(ang - M_PI / 6) * arrowSize / 5;
                double ax2 = xx + dx - qSin(M_PI / 3 - ang) * arrowSize / 5;
                double ay2 = yy + dy - qCos(M_PI / 3 - ang) * arrowSize / 5;

                swellWaveItem = m_scene->addLine(xx + dx, yy + dy, ax1, ay1, pen);
                swellWaveArrowPlot.push_back(swellWaveItem);

                swellWaveItem = m_scene->addLine(xx + dx, yy + dy, ax2, ay2, pen);
                swellWaveArrowPlot.push_back(swellWaveItem);

                i++;
            }
            j++;
        }
    }
    areInitSwellWaveArrowsDrawn = true;
}


void Plotter::drawCurrentArrow ()
{
    vector<vector<double>> cSpeed = m_gridder->currentSpeed;
    vector<vector<int>> cDir = m_gridder->currentDir;

    if (cSpeed.size() == 0 || cDir.size() == 0) {
        return;
    }

    double Di = m_gridder->Di;
    double Dj = m_gridder->Dj;
    int xMin = m_gridder->minPoint.x();
    int yMin = m_gridder->minPoint.y();
    int xMax = m_gridder->maxPoint.x();
    int yMax = m_gridder->maxPoint.y();
    int Ni = m_gridder->Ni;
    int Nj = m_gridder->Nj;



    double stepi = m_scene->stepI();
    double stepj = m_scene->stepJ();

    // arrow properties
    QPen pen = QPen(Qt::white, 2);
    pen.setCapStyle(Qt::RoundCap);
    pen.setCosmetic(true);
    double arrowSize = stepj / 2;

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
                double ang = (cDir[i][j]) * M_PI / 180.0;  // angle of arrow with +ve x axis in m_scene's ref
                double dx = arrowSize * qCos(ang);         // proj of arrow on x - axis (in m_scene's ref)
                double dy = arrowSize * qSin(ang);         // proj of arrow on y - axis      ,,
                double xx = x * stepi;   // starting coordinates of the arrow
                double yy = -toMerc(y) * stepj;

                QGraphicsItem* currentItem = m_scene->addLine(xx, yy, xx + dx, yy + dy, pen);
                currentArrowPlot.push_back(currentItem);

                //arrow heads
                double ax1 = xx + dx - qCos(ang - M_PI / 6) * arrowSize / 5;
                double ay1 = yy + dy - qSin(ang - M_PI / 6) * arrowSize / 5;
                double ax2 = xx + dx - qSin(M_PI / 3 - ang) * arrowSize / 5;
                double ay2 = yy + dy - qCos(M_PI / 3 - ang) * arrowSize / 5;

                currentItem = m_scene->addLine(xx + dx, yy + dy, ax1, ay1, pen);
                currentArrowPlot.push_back(currentItem);

                currentItem = m_scene->addLine(xx + dx, yy + dy, ax2, ay2, pen);
                currentArrowPlot.push_back(currentItem);


                //barbs
                int vApprox = (int)(std::floor(vInKnots*10));

                currentItem = m_scene->addLine(xx, yy, xx + dx, yy + dy, pen);
                currentArrowPlot.push_back(currentItem);

                double d = arrowSize / 12;
                double bx = xx + d*qCos(ang);
                double by = yy + d*qSin(ang);
                int noOfbarbs = vApprox/5;

                for (int i = 0; i < noOfbarbs; i++)
                {
                    QPointF be = barb(ang, arrowSize, true);

                    currentItem = m_scene->addLine(bx, by, bx + be.x(), by + be.y(), pen);
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
                double ang = (cDir[i][j]) * M_PI / 180.0;   // ang of arrow with +ve x axis in m_scene's ref
                double dx = arrowSize*qCos(ang);         // proj of arrow on x - axis (in m_scene's ref)
                double dy = arrowSize*qSin(ang);         // proj of arrow on y - axis      ,,
                double xx = x*stepi;                     // starting coordinates of the arrow
                double yy = -toMerc(y)*stepj;

                QGraphicsItem* currentItem = m_scene->addLine(xx, yy, xx + dx, yy + dy, pen);
                currentArrowPlot.push_back(currentItem);


                //arrow heads
                double ax1 = xx + dx - qCos(ang - M_PI / 6) * arrowSize / 5;
                double ay1 = yy + dy - qSin(ang - M_PI / 6) * arrowSize / 5;
                double ax2 = xx + dx - qSin(M_PI / 3 - ang) * arrowSize / 5;
                double ay2 = yy + dy - qCos(M_PI / 3 - ang) * arrowSize / 5;

                currentItem = m_scene->addLine(xx + dx, yy + dy, ax1, ay1, pen);
                currentArrowPlot.push_back(currentItem);

                currentItem = m_scene->addLine(xx + dx, yy + dy, ax2, ay2, pen);
                currentArrowPlot.push_back(currentItem);

                //barbs
                int vApprox = (int)(std::floor(vInKnots*10));

                currentItem = m_scene->addLine(xx, yy, xx + dx, yy + dy, pen);
                currentArrowPlot.push_back(currentItem);

                double d = arrowSize / 12;
                double bx = xx + d*qCos(ang);
                double by = yy + d*qSin(ang);
                int noOfbarbs = vApprox/5;

                for (int i = 0; i < noOfbarbs; i++)
                {
                    QPointF be = barb(ang, arrowSize, true);

                    currentItem = m_scene->addLine(bx, by, bx + be.x(), by + be.y(), pen);
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
                double ang = (cDir[i][j]) * M_PI / 180.0;  // angle of arrow with +ve x axis in m_scene's ref
                double dx = arrowSize*qCos(ang);         // projection of length of arrow on x - axis (in m_scene's ref)
                double dy = arrowSize*qSin(ang);         // projection of length of arrow on y - axis      ,,
                double xx = x*stepi;   // starting coordinates of the arrow
                double yy = -toMerc(y)*stepj;

                QGraphicsItem* currentItem = m_scene->addLine(xx, yy, xx + dx, yy + dy, pen);
                currentArrowPlot.push_back(currentItem);


                //arrow heads
                double ax1 = xx + dx - qCos(ang - M_PI / 6) * arrowSize / 5;
                double ay1 = yy + dy - qSin(ang - M_PI / 6) * arrowSize / 5;
                double ax2 = xx + dx - qSin(M_PI / 3 - ang) * arrowSize / 5;
                double ay2 = yy + dy - qCos(M_PI / 3 - ang) * arrowSize / 5;

                currentItem = m_scene->addLine(xx + dx, yy + dy, ax1, ay1, pen);
                currentArrowPlot.push_back(currentItem);

                currentItem = m_scene->addLine(xx + dx, yy + dy, ax2, ay2, pen);
                currentArrowPlot.push_back(currentItem);

                //barbs
                int vApprox = (int)(std::floor(vInKnots*10));

                currentItem = m_scene->addLine(xx, yy, xx + dx, yy + dy, pen);
                currentArrowPlot.push_back(currentItem);

                double d = arrowSize / 12;
                double bx = xx + d*qCos(ang);
                double by = yy + d*qSin(ang);
                int noOfbarbs = vApprox/5;

                for (int i = 0; i < noOfbarbs; i++)
                {
                    QPointF be = barb(ang, arrowSize, true);

                    currentItem = m_scene->addLine(bx, by, bx + be.x(), by + be.y(), pen);
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


void Plotter::drawIsobars ()
{
    vector<vector<double>> pres = m_gridder->pressure;

    if (pres.size() == 0)
    {
        qInfo() << "No pressure data available for plotting!";
        return;
    }

    double Di = m_gridder->Di;
    double Dj = m_gridder->Dj;
    int xMin = m_gridder->minPoint.x();
    int yMin = m_gridder->minPoint.y();
    int xMax = m_gridder->maxPoint.x();
    int yMax = m_gridder->maxPoint.y();
    int Ni = m_gridder->Ni;
    int Nj = m_gridder->Nj;

    float d = 1;
    vector<vector<double>> pres2DegHpa = getInterpolated2dArray(pres, d);

    double p_upper = 1024;
    double p_lower = 988;
    double p_step = 2;
    double isoPres = p_upper;
    double stepi = m_scene->stepI();
    double stepj = m_scene->stepJ();
    int labelDist = 25;
    int nbIsobars = (p_upper - p_lower) / p_step + 1;

    int k = 0;
    for (; isoPres >= p_lower; isoPres -= p_step)
    {
        int rows = pres2DegHpa.size();
        int cols = pres2DegHpa[0].size();

        vector<vector<bool>> presBin(rows, vector<bool>(cols, 0));

        for (int j = 0; j < cols; j++)
        {
            for (int i = 0; i < rows; i++)
            {
                double presHpa = pres2DegHpa[i][j] / 100.0;
                if (presHpa >= isoPres) {
                    presBin[i][j] = 1;
                }
            }
        }

        QColor color = QColor(Qt::darkBlue);
//        QPen ib_pen = QPen (color, 2);
//        QColor color;
//        color.setHsl(236, 100, 26 + k);
        QPen ib_pen = QPen (color, 2);
        ib_pen.setCosmetic(true);

        float unitStep_i = Di / float(d);
        float unitStep_j = Dj / float(d);

        if (xMin < xMax)
        {
            int pointCnt = 0;
            int j = 0;
            for (float y = yMin; y <= yMax && j < d * (Nj - 1); y += unitStep_j)
            {
                int i = 0;

                for (float x = xMin; x <= xMax && i < d * (Ni - 1); x += unitStep_i)
                {
                    QPointF a1 = QPointF(x, y);
                    QPointF a2 = QPointF(x + unitStep_i, y);
                    QPointF a3 = QPointF(x, y + unitStep_j);
                    QPointF a4 = QPointF(x + unitStep_i, y + unitStep_j);

                    int i_n = i * 2;
                    int j_n = j * 2;

                    bool b00 = presBin[i_n][j_n];
                    bool b20 = presBin[i_n + 2][j_n];
                    bool b02 = presBin[i_n][j_n + 2];
                    bool b22 = presBin[i_n + 2][j_n + 2];
                    bool b11 = presBin[i_n + 1][j_n + 1];

                    double pr1 = pres2DegHpa[i_n][j_n] / 100.0;
                    double pr2 = pres2DegHpa[i_n + 2][j_n] / 100.0;
                    double pr3 = pres2DegHpa[i_n][j_n + 2] / 100.0;
                    double pr4 = pres2DegHpa[i_n + 2][j_n + 2] / 100.0;

                    QVector<QLineF> lines = getIsoLines(b00, b20, b02, b22, b11, a1, a2, a3, a4, pr1, pr2, pr3, pr4, isoPres);

                    for (int c = 0; c < lines.size(); c++)
                    {
                        QLineF line = lines.at(c);

                        QPointF q1 = QPointF(line.p1().x() * stepi, -toMerc(line.p1().y()) * stepj);
                        QPointF q2 = QPointF(line.p2().x() * stepi, -toMerc(line.p2().y()) * stepj);

                        QLineF lineToPlot = QLineF(q1, q2);

                        if (q1 != q2)
                        {
                            QGraphicsLineItem* cellLine = m_scene->addLine(lineToPlot, ib_pen);
                            isobarPlot.push_back(cellLine);

                            if (pointCnt == labelDist * d)
                            {
                                QString labelText = QString::number(isoPres);
                                QGraphicsTextItem* label = new QGraphicsTextItem(labelText);

                                label->setPos(q1.x() - 4.9, q1.y() - 4.6);
                                QFont f = label->font();
                                f.setPointSizeF(1.1);
                                f.setWeight(QFont::Light);
                                label->setFont(f);
                                label->setDefaultTextColor(color);

                                m_scene->addItem(label);
                                isobarPlot.push_back(label);
                                pointCnt = 0;
                            }
                            pointCnt++;
                        }
                    }
                    i++;
                }
                j++;
            }
        }
        else
        {
            float skipped = 0;
            int pointCnt = 0;
            int j = 0;
            for (float y = yMin; y <= yMax && j < d * (Nj - 1); y += unitStep_j)
            {
                int i = 0;
                for (float x = xMin; x <= 180 && i < d * (Ni - 1); x += unitStep_i)
                {
                    QPointF a1 = QPointF(x, y);
                    QPointF a2 = QPointF(x + unitStep_i, y);
                    QPointF a3 = QPointF(x, y + unitStep_j);
                    QPointF a4 = QPointF(x + unitStep_i, y + unitStep_j);

                    int i_n = i * 2;
                    int j_n = j * 2;

                    bool b00 = presBin[i_n][j_n];
                    bool b20 = presBin[i_n + 2][j_n];
                    bool b02 = presBin[i_n][j_n + 2];
                    bool b22 = presBin[i_n + 2][j_n + 2];
                    bool b11 = presBin[i_n + 1][j_n + 1];

                    double pr1 = pres2DegHpa[i_n][j_n] / 100.0;
                    double pr2 = pres2DegHpa[i_n + 2][j_n] / 100.0;
                    double pr3 = pres2DegHpa[i_n][j_n + 2] / 100.0;
                    double pr4 = pres2DegHpa[i_n + 2][j_n + 2] / 100.0;

                    QVector<QLineF> lines = getIsoLines(b00, b20, b02, b22, b11, a1, a2, a3, a4, pr1, pr2, pr3, pr4, isoPres);

                    for (int c = 0; c < lines.size(); c++)
                    {
                        QLineF line = lines.at(c);

                        QPointF q1 = QPointF(line.p1().x() * stepi, -toMerc(line.p1().y()) * stepj);
                        QPointF q2 = QPointF(line.p2().x() * stepi, -toMerc(line.p2().y()) * stepj);

                        QLineF lineToPlot = QLineF(q1, q2);
                        if (q1 != q2)
                        {
                            QGraphicsLineItem* cellLine = m_scene->addLine(lineToPlot, ib_pen);
                            isobarPlot.push_back(cellLine);

                            if (pointCnt == labelDist * d)
                            {
                                QString labelText = QString::number(isoPres);
                                QGraphicsTextItem* label = new QGraphicsTextItem(labelText);

                                label->setPos(q1.x() - 4.7, q1.y() - 4.7);
                                QFont f = label->font();
                                f.setPointSizeF(1.2);
                                label->setFont(f);
                                label->setDefaultTextColor(color);

                                m_scene->addItem(label);
                                isobarPlot.push_back(label);
                                pointCnt = 0;
                            }
                            pointCnt++;
                        }
                    }

                    if (x + unitStep_i > 180) {
                        skipped = 180 - floor(x);
                    }
                    i++;
                }
                i += d - 1;
                float skip = Di - 1 - skipped;

                for (float x = -179 + skip; x <= xMax && i < d * (Ni - 1); x += unitStep_i)
                {
                    QPointF a1 = QPointF(x, y);
                    QPointF a2 = QPointF(x + unitStep_i, y);
                    QPointF a3 = QPointF(x, y + unitStep_j);
                    QPointF a4 = QPointF(x + unitStep_i, y + unitStep_j);

                    int i_n = i * 2;
                    int j_n = j * 2;

                    bool b00 = presBin[i_n][j_n];
                    bool b20 = presBin[i_n + 2][j_n];
                    bool b02 = presBin[i_n][j_n + 2];
                    bool b22 = presBin[i_n + 2][j_n + 2];
                    bool b11 = presBin[i_n + 1][j_n + 1];

                    double pr1 = pres2DegHpa[i_n][j_n] / 100.0;
                    double pr2 = pres2DegHpa[i_n + 2][j_n] / 100.0;
                    double pr3 = pres2DegHpa[i_n][j_n + 2] / 100.0;
                    double pr4 = pres2DegHpa[i_n + 2][j_n + 2] / 100.0;

                    QVector<QLineF> lines = getIsoLines(b00, b20, b02, b22, b11, a1, a2, a3, a4, pr1, pr2, pr3, pr4, isoPres);

                    for (int c = 0; c < lines.size(); c++)
                    {
                        QLineF line = lines.at(c);

                        QPointF q1 = QPointF(line.p1().x() * stepi, -toMerc(line.p1().y()) * stepj);
                        QPointF q2 = QPointF(line.p2().x() * stepi, -toMerc(line.p2().y()) * stepj);

                        QLineF lineToPlot = QLineF(q1, q2);
                        if (q1 != q2)
                        {
                            QGraphicsLineItem* cellLine = m_scene->addLine(lineToPlot, ib_pen);
                            isobarPlot.push_back(cellLine);

                            if (pointCnt == labelDist * d)
                            {
                                QString labelText = QString::number(isoPres);
                                QGraphicsTextItem* label = new QGraphicsTextItem(labelText);

                                label->setPos(q1.x() - 4.7, q1.y() - 4.7);
                                QFont f = label->font();
                                f.setPointSizeF(1.2);
                                label->setFont(f);
                                label->setDefaultTextColor(color);

                                m_scene->addItem(label);
                                isobarPlot.push_back(label);
                                pointCnt = 0;
                            }
                            pointCnt++;
                        }
                    }
                    i++;
                }
                j++;
            }
        }
        k++;
     }
    areInitIsobarsDrawn = true;
}
