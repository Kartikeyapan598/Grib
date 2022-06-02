#include "infopanel.h"
#include "ui_infopanel.h"
#include "mainwindow.h"

InfoPanel* InfoPanel::m_infoPanelInstance;

InfoPanel::InfoPanel(QWidget *parent) :
                        QDialog(parent),
                        ui(new Ui::InfoPanel)
{
    if(!m_infoPanelInstance) {
        m_infoPanelInstance = this;
    }
    else {
        return;
    }

    ui->setupUi(this);

    setWindowTitle("Info Panel");
    setWindowFlag(Qt::NoDropShadowWindowHint);

    ui->prevDayFcInfoBtn->setDisabled(true);
    ui->prevFcInfoBtn->setDisabled(true);

    m_legendView = new QGraphicsView (this->ui->legendGroupBox);

    m_legendScene = new QGraphicsScene (this->ui->legendGroupBox->rect());

    m_legendView->setScene(m_legendScene);
    m_legendView->show();

    drawWindColourScale();
    drawWindScaleKnts();
    windInitScaleInfo = true;
    windScaleInfo = true;
    waveInitScaleInfo = false;
    waveScaleInfo = false;

    windBarbsLegendRect = QRectF(2 * m_windColourImg->boundingRect().width() + 10, 15, 2 * m_windColourImg->boundingRect().width() + 70, 95);

    waveArrowsLegendRect = QRectF(windBarbsLegendRect.bottomLeft(), windBarbsLegendRect.bottomRight() + QPointF(0, windBarbsLegendRect.height()));

    currentArrowsLegendRect = QRectF(waveArrowsLegendRect.bottomLeft(), waveArrowsLegendRect.bottomRight() + QPointF(0, waveArrowsLegendRect.height()));

    isobarsLegendRect = QRectF(currentArrowsLegendRect.bottomLeft(), currentArrowsLegendRect.bottomRight() + QPointF(0, currentArrowsLegendRect.height()));

    drawWindBarbsLegend();
    drawWaveArrowLegend();
    drawCurrentArrowLegend();
    drawIsobarLegend();
}

InfoPanel::~InfoPanel()
{
    delete ui;
}

void InfoPanel::on_prevDayFcInfoBtn_clicked()
{
    MainWindow& mw_instance = MainWindow::GetMainWindowInstance();

    if (mw_instance.getForecastPtr() - 4 < 1)
    {
        ui->prevFcInfoBtn->setDisabled(true);
    }
    if (mw_instance.getForecastPtr() - 4 < 4)
    {
        ui->prevDayFcInfoBtn->setDisabled(true);
    }
    if (mw_instance.getForecastPtr() - 4 < mw_instance.getNbFc() - 1)
    {
        ui->nextFcInfoBtn->setDisabled(false);
    }
    if (mw_instance.getForecastPtr() - 4 < mw_instance.getNbFc() - 4)
    {
        ui->nextDayFcInfoBtn->setDisabled(false);
    }

    int prevFc = mw_instance.getForecastPtr();
    updateCurrentDate(mw_instance.getForecastPtr() - 4);
    setForecastPtr(mw_instance.getForecastPtr() - 4);
    mw_instance.updateElementsOnScene(prevFc);

}

void InfoPanel::on_prevFcInfoBtn_clicked()
{
    MainWindow& mw_instance = MainWindow::GetMainWindowInstance();

    if (mw_instance.getForecastPtr() - 1 < 1)
    {
        ui->prevFcInfoBtn->setDisabled(true);
    }
    if (mw_instance.getForecastPtr() - 1 < 4)
    {
        ui->prevDayFcInfoBtn->setDisabled(true);
    }
    if (mw_instance.getForecastPtr() - 1 < mw_instance.getNbFc() - 1)
    {
        ui->nextFcInfoBtn->setDisabled(false);
    }
    if (mw_instance.getForecastPtr() - 1 < mw_instance.getNbFc() - 4)
    {
        ui->nextDayFcInfoBtn->setDisabled(false);
    }

    int prevFc = mw_instance.getForecastPtr();
    updateCurrentDate(mw_instance.getForecastPtr() - 1);
    setForecastPtr(mw_instance.getForecastPtr() - 1);
    mw_instance.updateElementsOnScene(prevFc);
}

void InfoPanel::on_nextFcInfoBtn_clicked()
{
    MainWindow& mw_instance = MainWindow::GetMainWindowInstance();

    if (mw_instance.getForecastPtr() + 1 >= mw_instance.getNbFc() - 1)
    {
        ui->nextFcInfoBtn->setDisabled(true);
    }
    if (mw_instance.getForecastPtr() + 1 >= mw_instance.getNbFc() - 4)
    {
        ui->nextDayFcInfoBtn->setDisabled(true);
    }
    if (mw_instance.getForecastPtr() + 1 > 0)
    {
        ui->prevFcInfoBtn->setDisabled(false);
    }
    if (mw_instance.getForecastPtr() + 1 >= 4)
    {
        ui->prevDayFcInfoBtn->setDisabled(false);
    }

    int prevFc = mw_instance.getForecastPtr();
    updateCurrentDate(mw_instance.getForecastPtr() + 1);
    setForecastPtr(mw_instance.getForecastPtr() + 1);
    mw_instance.updateElementsOnScene(prevFc);
}

void InfoPanel::on_nextDayFcInfoBtn_clicked()
{
    MainWindow& mw_instance = MainWindow::GetMainWindowInstance();

    if (mw_instance.getForecastPtr() + 4 >= mw_instance.getNbFc() - 1)
    {
        ui->nextFcInfoBtn->setDisabled(true);
    }
    if (mw_instance.getForecastPtr() + 4 >= mw_instance.getNbFc() - 4)
    {
        ui->nextDayFcInfoBtn->setDisabled(true);
    }
    if (mw_instance.getForecastPtr() + 4 > 0)
    {
        ui->prevFcInfoBtn->setDisabled(false);
    }
    if (mw_instance.getForecastPtr() + 4 >= 4)
    {
        ui->prevDayFcInfoBtn->setDisabled(false);
    }

    int prevFc = mw_instance.getForecastPtr();
    updateCurrentDate(mw_instance.getForecastPtr() + 4);
    setForecastPtr(mw_instance.getForecastPtr() + 4);
    mw_instance.updateElementsOnScene(prevFc);
}

void InfoPanel::setRefDate (time_t refDate)
{
    this->refDate = QDateTime::fromSecsSinceEpoch(refDate);
    refDateStr = QLocale::system().toString(this->refDate, QLocale::ShortFormat);
    ui->refDateTextLabel->setText(refDateStr);
    ui->currDateTextLabel->setText(refDateStr);
}

void InfoPanel::setForecastPtr (int newPtr)
{
    MainWindow& mw_instance = MainWindow::GetMainWindowInstance();
    mw_instance.m_fcPtr = newPtr;
}

void InfoPanel::updateCurrentDate (int fc)
{
    QDateTime currDate = refDate.addSecs(step_secs * fc);
    QString currDateStr = QLocale::system().toString(currDate, QLocale::ShortFormat);
    ui->currDateTextLabel->setText(currDateStr);
}

void InfoPanel::drawWindColourScale ()
{
    QImage wImg(40, 380, QImage::Format_ARGB32);
    std::unordered_map<int, QColor> m_wCol;

    m_wCol[0] = QColor(40, 140, 255);
    m_wCol[3] = QColor(0, 175, 255);
    m_wCol[6] = QColor(0, 220, 225);
    m_wCol[9] = QColor(0, 247, 176);
    m_wCol[12] = QColor(0, 234, 156);
    m_wCol[15] = QColor(130, 240, 89);
    m_wCol[18] = QColor(240, 245, 3);
    m_wCol[21] = QColor(255, 237, 0);
    m_wCol[24] = QColor(255, 219, 0);
    m_wCol[27] = QColor(255, 199, 0);
    m_wCol[30] = QColor(255, 180, 0);
    m_wCol[33] = QColor(255, 152, 0);
    m_wCol[36] = QColor(255, 126, 0);
    m_wCol[39] = QColor(247, 120, 0);
    m_wCol[42] = QColor(236, 120, 20);
    m_wCol[45] = QColor(228, 113, 30);
    m_wCol[48] = QColor(224, 97, 40);
    m_wCol[51] = QColor(220, 81, 50);
    m_wCol[54] = QColor(213, 69, 60);
    m_wCol[57] = QColor(205, 58, 70);
    m_wCol[60] = QColor(190, 44, 80);
    m_wCol[63] = QColor(180, 26, 90);
    m_wCol[66] = QColor(170, 20, 100);
    m_wCol[69] = QColor(150, 40, 120);
    m_wCol[72] = QColor(140, 50, 140);
    m_wCol[75] = QColor(150, 50, 180);
    m_wCol[78] = QColor(200, 50, 255);

    std::vector<int> v ={0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48, 51, 54, 57, 60, 63, 66, 69, 72, 75, 78};

    fillColorMap(m_wCol, v);

    float stepj = wImg.height() / 27;
    int i = 0;

    for (float y = 0; y < wImg.height() && i <= 78; y += stepj / 3)
    {
        for (float yy = y; yy < y + stepj; yy++) {
            for (float xx = 0; xx < wImg.width(); xx++) {
                wImg.setPixel(xx, yy, m_wCol[i].rgb());
            }
        }
        i++;
    }
    m_windColourImg = new QGraphicsPixmapItem(QPixmap::fromImage(wImg));
    m_windColourImg->setOffset(5, 15);
//    m_windColourImg->setPos(0, 0);
    m_legendScene->addItem(m_windColourImg);
}

void InfoPanel::drawWaveColourScale ()
{
    QImage wImg(40, 380, QImage::Format_ARGB32);

    std::unordered_map<int, QColor> m_wvCol;

    m_wvCol[0] = QColor(40, 140, 255);
    m_wvCol[1] = QColor(0, 175, 255);
    m_wvCol[2] = QColor(0, 220, 255);
    m_wvCol[5] = QColor(0, 247, 176);
    m_wvCol[10] = QColor(130, 240, 89);
    m_wvCol[15] = QColor(240, 245, 3);
    m_wvCol[20] = QColor(255, 237, 0);
    m_wvCol[25] = QColor(255, 219, 0);
    m_wvCol[30] = QColor(255, 199, 0);
    m_wvCol[35] = QColor(255, 180, 0);
    m_wvCol[40] = QColor(255, 152, 0);
    m_wvCol[45] = QColor(255, 126, 0);
    m_wvCol[50] = QColor(247, 120, 0);
    m_wvCol[55] = QColor(236, 120, 20);
    m_wvCol[60] = QColor(228, 113, 30);
    m_wvCol[70] = QColor(224, 97, 40);
    m_wvCol[80] = QColor(220, 81, 50);
    m_wvCol[90] = QColor(213, 69, 60);
    m_wvCol[100] = QColor(205, 58, 70);
    m_wvCol[120] = QColor(190, 44, 80);
    m_wvCol[140] = QColor(180, 26, 90);
    m_wvCol[160] = QColor(170, 20, 100);
    m_wvCol[180] = QColor(150, 40, 120);
    m_wvCol[200] = QColor(140, 50, 140);

    std::vector<int> v ={0, 1, 2, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 70, 80, 90, 100, 120, 140, 160, 180, 200};

    fillColorMap(m_wvCol, v);
    float stepj = wImg.height() / 24;
    int i = 0;

    for (float y = 0; y < wImg.height() && i <= 2; y += stepj)
    {
        for (float yy = y; yy < y + stepj; yy++) {
            for (float xx = 0; xx < wImg.width(); xx++) {
                wImg.setPixel(xx, yy, m_wvCol[i].rgb());
            }
        }
        i++;
    }

    for (float y = 3*stepj; y < wImg.height() && i <= 5; y += stepj / 3)
    {
        for (float yy = y; yy < y + stepj; yy++) {
            for (float xx = 0; xx < wImg.width(); xx++) {
                wImg.setPixel(xx, yy, m_wvCol[i].rgb());
            }
        }
        i++;
    }

    for (float y = 4*stepj; y < wImg.height() && i <= 60; y += stepj / 5)
    {
        for (float yy = y; yy < y + stepj; yy++) {
            for (float xx = 0; xx < wImg.width(); xx++) {
                wImg.setPixel(xx, yy, m_wvCol[i].rgb());
            }
        }
        i++;
    }

    for (float y = 15*stepj; y < wImg.height() && i <= 100; y += stepj / 10)
    {
        for (float yy = y; yy < y + stepj; yy++) {
            for (float xx = 0; xx < wImg.width(); xx++) {
                wImg.setPixel(xx, yy, m_wvCol[i].rgb());
            }
        }
        i++;
    }

    for (float y = 19*stepj; y < wImg.height() && i <= 200; y += stepj / 20)
    {
        for (float yy = y; yy < y + stepj; yy++) {
            for (float xx = 0; xx < wImg.width(); xx++) {
                wImg.setPixel(xx, yy, m_wvCol[i].rgb());
            }
        }
        i++;
    }

    m_waveColourImg = new QGraphicsPixmapItem(QPixmap::fromImage(wImg));
    m_waveColourImg->setOffset(5, 15);
    m_waveColourImg->setPos(0, 0);
    m_legendScene->addItem(m_waveColourImg);
}

void InfoPanel::drawWindScaleKnts ()
{
    float stepj = m_windColourImg->boundingRect().height() / 78;
    int i = 0;

    for (float y = 0; y < m_windColourImg->boundingRect().height() && i <= 78; y += stepj)
    {
        if (i % 5 == 0)
        {
            QGraphicsTextItem* txt = new QGraphicsTextItem(QString::number(i) + " kts");

            QFont f = txt->font();
            f.setPointSize(10);
            txt->setFont(f);

            txt->setPos(m_windColourImg->boundingRect().topRight().x() - 2, y + 7);
            m_legendScene->addItem(txt);
            windScaleTxt.push_back(txt);
        }
        i++;
    }
}

void InfoPanel::drawWaveScaleMtrs ()
{
    float stepj = m_waveColourImg->boundingRect().height() / 24;
    int i = 0;
    float y = 0;
    for (; y < m_waveColourImg->boundingRect().height() && i <= 2; y += stepj)
    {
        QGraphicsTextItem* txt = new QGraphicsTextItem(QString::number(i/10.0) + " m");

        QFont f = txt->font();
        f.setPointSize(10);
        txt->setFont(f);

        txt->setPos(m_waveColourImg->boundingRect().topRight().x() - 2, y + 7);
        m_legendScene->addItem(txt);
        waveScaleTxt.push_back(txt);
        i++;
    }

    for (; y < m_waveColourImg->boundingRect().height() && i <= 5; y += stepj)
    {
        QGraphicsTextItem* txt = new QGraphicsTextItem(QString::number((i + 2) / 10.0) + " m");

        QFont f = txt->font();
        f.setPointSize(10);
        txt->setFont(f);

        txt->setPos(m_waveColourImg->boundingRect().topRight().x() - 2, y + 7);
        m_legendScene->addItem(txt);
        waveScaleTxt.push_back(txt);
        i += 3;
    }

    for (; y < m_waveColourImg->boundingRect().height() && i <= 60; y += stepj)
    {
        QGraphicsTextItem* txt = new QGraphicsTextItem(QString::number((i + 4) / 10.0) + " m");

        QFont f = txt->font();
        f.setPointSize(10);
        txt->setFont(f);

        txt->setPos(m_waveColourImg->boundingRect().topRight().x() - 2, y + 7);
        m_legendScene->addItem(txt);
        waveScaleTxt.push_back(txt);
        i += 5;
    }

    for (; y < m_waveColourImg->boundingRect().height() && i <= 100; y += stepj)
    {
        QGraphicsTextItem* txt = new QGraphicsTextItem(QString::number((i + 9) / 10.0) + " m");

        QFont f = txt->font();
        f.setPointSize(10);
        txt->setFont(f);

        txt->setPos(m_waveColourImg->boundingRect().topRight().x() - 2, y + 7);
        m_legendScene->addItem(txt);
        waveScaleTxt.push_back(txt);
        i += 10;
    }

    for (; y < m_waveColourImg->boundingRect().height() && i <= 200; y += stepj)
    {
        QGraphicsTextItem* txt = new QGraphicsTextItem(QString::number((i + 19) / 10.0) + " m");

        QFont f = txt->font();
        f.setPointSize(10);
        txt->setFont(f);

        txt->setPos(m_waveColourImg->boundingRect().topRight().x() - 2, y + 7);
        m_legendScene->addItem(txt);
        waveScaleTxt.push_back(txt);
        i += 20;
    }
}

void InfoPanel::drawWindBarbsLegend ()
{
    QGraphicsRectItem* rect = m_legendScene->addRect(windBarbsLegendRect, QPen(Qt::black));
    windLegend.push_back(rect);

    // ellipse
    float xx = windBarbsLegendRect.topLeft().x() + 16;
    float yy = windBarbsLegendRect.topLeft().y() - 5;

    QGraphicsTextItem* txt = new QGraphicsTextItem("Wind Speed");
    txt->setPos(xx + 15, yy);
    m_legendScene->addItem(txt);
    windLegend.push_back(txt);

    yy += 30;

    double rads = 4;
    QGraphicsItem* windItem = m_legendScene -> addEllipse(xx - rads, yy - rads, rads*2.0, rads*2.0, QPen(Qt::black, 2));
    windLegend.push_back(windItem);
    double rad = 8;
    windItem = m_legendScene -> addEllipse(xx - rad, yy - rad, rad*2.0, rad*2.0, QPen(Qt::black, 2));
    windLegend.push_back(windItem);

    txt = new QGraphicsTextItem("< 1 kts");
    txt->setPos(xx + 65, yy - 13);
    m_legendScene->addItem(txt);
    windLegend.push_back(txt);

    // small barb
    xx -= 10;
    yy += 20;
    double arrowSize = 40;
    double dx = arrowSize;
    double dy = 0;

    windItem = m_legendScene -> addLine(xx, yy, xx + dx, yy + dy, QPen(Qt::black, 2));
    windLegend.push_back(windItem);

    double d = arrowSize/6;
    double bx = xx;
    double by = yy;

    bx += d;
    by += 0;

    QPointF be = barb(0, arrowSize, true);

    windItem = m_legendScene -> addLine(bx, by, bx + be.x(), by + be.y(), QPen(Qt::black, 2));
    windLegend.push_back(windItem);

    txt = new QGraphicsTextItem(" 5 kts");
    txt->setPos(xx + 75, yy - 15);
    m_legendScene->addItem(txt);
    windLegend.push_back(txt);

    // big barb
    xx += 15;
    yy += 20;
    bx = xx;
    by = yy;
    windItem = m_legendScene -> addLine(xx, yy, xx + dx, yy + dy, QPen(Qt::black, 2));
    windLegend.push_back(windItem);

    be = barb(0, arrowSize);

    windItem = m_legendScene -> addLine(bx, by, bx + be.x(), by + be.y(), QPen(Qt::black, 2));
    windLegend.push_back(windItem);

    txt = new QGraphicsTextItem(" 10 kts");
    txt->setPos(xx + 60, yy - 15);
    m_legendScene->addItem(txt);
    windLegend.push_back(txt);

    // triangle
    xx -= 10;
    yy += 20;
    bx = xx;
    by = yy;
    windItem = m_legendScene -> addLine(xx, yy, xx + dx, yy + dy, QPen(Qt::black, 2));
    windLegend.push_back(windItem);

    QPair<QPointF, QPointF> p = triangle(0, arrowSize);

    windItem = m_legendScene->addLine(bx, by, bx + p.first.x(), by + p.first.y(), QPen(Qt::black, 2));
    windLegend.push_back(windItem);

    windItem = m_legendScene->addLine(bx + p.second.x(), by + p.second.y(), bx + p.first.x(), by + p.first.y(), QPen(Qt::black, 2));
    windLegend.push_back(windItem);

    txt = new QGraphicsTextItem(" 50 kts");
    txt->setPos(xx + 70, yy - 15);
    m_legendScene->addItem(txt);
    windLegend.push_back(txt);

}

void InfoPanel::drawWaveArrowLegend ()
{
    QGraphicsRectItem* rect = m_legendScene->addRect(waveArrowsLegendRect, QPen(Qt::black));
    waveLegend.push_back(rect);

    double arrowSize = 40;
    double ang = - M_PI/6;
    double dx = arrowSize * qCos(ang);         // projection of length of arrow on x - axis (in m_scene's ref)
    double dy = arrowSize * qSin(ang);         // projection of length of arrow on y - axis      ,,
    double xx = waveArrowsLegendRect.topLeft().x() + 15;                      // starting coordinates of the arrow
    double yy = waveArrowsLegendRect.topLeft().y() - 5;

    QGraphicsTextItem* txt = new QGraphicsTextItem("Wave Direction");
    txt->setPos(xx + 10, yy);
    m_legendScene->addItem(txt);
    waveLegend.push_back(txt);

    xx -= 10;
    yy += 50;

    QGraphicsItem* windWaveItem = m_legendScene->addLine(xx, yy, xx + dx, yy + dy, QPen(Qt::blue, 2));
    waveLegend.push_back(windWaveItem);

    //arrow heads
    double ax1 = xx + dx - qCos(ang - M_PI / 6) * arrowSize / 5;
    double ay1 = yy + dy - qSin(ang - M_PI / 6) * arrowSize / 5;
    double ax2 = xx + dx - qSin(M_PI / 3 - ang) * arrowSize / 5;
    double ay2 = yy + dy - qCos(M_PI / 3 - ang) * arrowSize / 5;

    windWaveItem = m_legendScene->addLine(xx + dx, yy + dy, ax1, ay1, QPen(Qt::blue, 2));
    waveLegend.push_back(windWaveItem);

    windWaveItem = m_legendScene->addLine(xx + dx, yy + dy, ax2, ay2, QPen(Qt::blue, 2));
    waveLegend.push_back(windWaveItem);

    txt = new QGraphicsTextItem("Wind Wave");
    txt->setPos(xx + 50, yy - 20);
    txt->setTextWidth(100);
    m_legendScene->addItem(txt);
    waveLegend.push_back(txt);

    yy += 35;

    windWaveItem = m_legendScene->addLine(xx, yy, xx + dx, yy + dy, QPen(Qt::magenta, 2));
    waveLegend.push_back(windWaveItem);

    //arrow heads
    ax1 = xx + dx - qCos(ang - M_PI / 6) * arrowSize / 5;
    ay1 = yy + dy - qSin(ang - M_PI / 6) * arrowSize / 5;
    ax2 = xx + dx - qSin(M_PI / 3 - ang) * arrowSize / 5;
    ay2 = yy + dy - qCos(M_PI / 3 - ang) * arrowSize / 5;

    windWaveItem = m_legendScene->addLine(xx + dx, yy + dy, ax1, ay1, QPen(Qt::magenta, 2));
    waveLegend.push_back(windWaveItem);

    windWaveItem = m_legendScene->addLine(xx + dx, yy + dy, ax2, ay2, QPen(Qt::magenta, 2));
    waveLegend.push_back(windWaveItem);

    txt = new QGraphicsTextItem("Swell Wave");
    txt->setPos(xx + 50, yy - 20);
    txt->setTextWidth(100);
    m_legendScene->addItem(txt);
    waveLegend.push_back(txt);
}

void InfoPanel::drawCurrentArrowLegend ()
{
    QGraphicsRectItem* rect = m_legendScene->addRect(currentArrowsLegendRect, QPen(Qt::black));
    currentLegend.push_back(rect);
    rect -> setBrush(QColor(40, 140, 255));

    double arrowSize = 50;
    double ang = - M_PI / 4;
    double dx = arrowSize * qCos(ang);
    double dy = arrowSize * qSin(ang);
    double xx = currentArrowsLegendRect.topLeft().x() + 15;
    double yy = currentArrowsLegendRect.topLeft().y() - 5;

    QGraphicsTextItem* txt = new QGraphicsTextItem("Current");
    txt->setPos(xx + 5, yy);
    m_legendScene->addItem(txt);
    currentLegend.push_back(txt);

    xx += 5;
    yy += 70;

    QGraphicsItem* currentItem = m_legendScene->addLine(xx, yy, xx + dx, yy + dy, QPen(Qt::white, 2));
    currentLegend.push_back(currentItem);

    //arrow heads
    double ax1 = xx + dx - qCos(ang - M_PI / 6) * arrowSize / 5;
    double ay1 = yy + dy - qSin(ang - M_PI / 6) * arrowSize / 5;
    double ax2 = xx + dx - qSin(M_PI / 3 - ang) * arrowSize / 5;
    double ay2 = yy + dy - qCos(M_PI / 3 - ang) * arrowSize / 5;

    currentItem = m_legendScene->addLine(xx + dx, yy + dy, ax1, ay1, QPen(Qt::white, 2));
    currentLegend.push_back(currentItem);

    currentItem = m_legendScene->addLine(xx + dx, yy + dy, ax2, ay2, QPen(Qt::white, 2));
    currentLegend.push_back(currentItem);

    //barbs

    double d = arrowSize / 12;
    double bx = xx + d*qCos(ang);
    double by = yy + d*qSin(ang);
    QPointF be = barb(ang, arrowSize, true);

    currentItem = m_legendScene->addLine(bx, by, bx + be.x(), by + be.y(), QPen(Qt::white, 2));
    currentLegend.push_back(currentItem);

    txt = new QGraphicsTextItem("0.5 kt");
    txt->setPos(xx + 50, yy - 30);
    m_legendScene->addItem(txt);
    currentLegend.push_back(txt);

}

void InfoPanel::drawIsobarLegend ()
{
    QGraphicsRectItem* rect = m_legendScene->addRect(isobarsLegendRect, QPen(Qt::black));
    isoBarLegend.push_back(rect);

    double xx = isobarsLegendRect.topLeft().x() + 15;
    double yy = isobarsLegendRect.topLeft().y() - 5;

    QGraphicsTextItem* txt = new QGraphicsTextItem("Isobars");
    txt->setPos(xx + 10, yy);
    m_legendScene->addItem(txt);
    isoBarLegend.push_back(txt);

    yy += 60;

    QGraphicsLineItem* line = m_legendScene->addLine(xx, yy, xx + 20, yy - 20, QPen(Qt::blue, 2));
    isoBarLegend.push_back(line);

    line = m_legendScene->addLine(xx + 20, yy - 20, xx + 40, yy - 20, QPen(Qt::blue, 2));
    isoBarLegend.push_back(line);

    line = m_legendScene->addLine(xx + 40, yy - 20, xx + 80, yy, QPen(Qt::blue, 2));
    isoBarLegend.push_back(line);

    QGraphicsTextItem* label = new QGraphicsTextItem("992");

    label->setPos(xx + 15, yy - 35);
    QFont f = label->font();
    f.setWeight(QFont::Bold);
    label->setFont(f);
    label->setDefaultTextColor(Qt::darkBlue);

    m_legendScene->addItem(label);
    isoBarLegend.push_back(label);
}

void InfoPanel::addWindColourScale ()
{
    if (windInitScaleInfo)
    {
        m_windColourImg->setOffset(5, 15);
        m_legendScene->addItem(m_windColourImg);

        for (auto it : windScaleTxt) {
            m_legendScene->addItem(it);
        }
    }
    else
    {
        drawWindColourScale();
        drawWindScaleKnts();
        windInitScaleInfo = true;
    }
}

void InfoPanel::addWaveColourScale ()
{
    if (waveInitScaleInfo)
    {
        m_waveColourImg->setOffset(5, 15);
        m_legendScene->addItem(m_waveColourImg);

        for (auto it : waveScaleTxt) {
            m_legendScene->addItem(it);
        }
    }
    else
    {
        drawWaveColourScale();
        drawWaveScaleMtrs();
        waveInitScaleInfo = true;
    }
}

void InfoPanel::removeWindColourScale ()
{
    m_legendScene->removeItem(m_windColourImg);

    for (auto it : windScaleTxt) {
        m_legendScene->removeItem(it);
    }
}

void InfoPanel::removeWaveColourScale ()
{
    m_legendScene->removeItem(m_waveColourImg);

    for (auto it : waveScaleTxt) {
        m_legendScene->removeItem(it);
    }
}
