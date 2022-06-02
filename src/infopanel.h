#ifndef INFOPANEL_H
#define INFOPANEL_H

#include "qdatetime.h"
#include <QDialog>
#include "customscene.h"

namespace Ui {
class InfoPanel;
}

class InfoPanel : public QDialog
{
    Q_OBJECT

public:

    InfoPanel () = delete;

    explicit InfoPanel (QWidget *parent = nullptr);

    ~InfoPanel ();

    static InfoPanel &getInfoPanelInstance () { return *m_infoPanelInstance; }

    void setForecastPtr (int newPtr);
    void setRefDate (time_t refDate);
    bool getWindScaleInfo () { return windScaleInfo; }
    bool getWaveScaleInfo () { return waveScaleInfo; }
    void setWindScaleInfo (bool windScaleShown) { windScaleInfo = windScaleShown; }
    void setWaveScaleInfo (bool waveScaleShown) { waveScaleInfo = waveScaleShown; }

    void addWindColourScale ();
    void addWaveColourScale ();
    void removeWindColourScale ();
    void removeWaveColourScale ();

private slots:
    void on_prevDayFcInfoBtn_clicked();

    void on_prevFcInfoBtn_clicked();

    void on_nextFcInfoBtn_clicked();

    void on_nextDayFcInfoBtn_clicked();


private:
    static InfoPanel* m_infoPanelInstance;
    void friend CustomScene::mouseMoveEvent(QGraphicsSceneMouseEvent* e);

    void updateCurrentDate (int fc);
    void drawWindColourScale ();
    void drawWindScaleKnts ();
    void drawWaveColourScale ();
    void drawWaveScaleMtrs ();
    void drawWindBarbsLegend ();
    void drawWaveArrowLegend ();
    void drawCurrentArrowLegend ();
    void drawIsobarLegend ();

    Ui::InfoPanel *ui;
    QGraphicsView* m_legendView;
    QGraphicsScene* m_legendScene;

    QGraphicsPixmapItem* m_waveColourImg;
    QGraphicsPixmapItem* m_windColourImg;

    QRectF windBarbsLegendRect;
    QRectF waveArrowsLegendRect;
    QRectF currentArrowsLegendRect;
    QRectF isobarsLegendRect;

    QVector<QGraphicsItem*> windLegend;
    QVector<QGraphicsItem*> waveLegend;
    QVector<QGraphicsItem*> currentLegend;
    QVector<QGraphicsItem*> isoBarLegend;

    QVector<QGraphicsItem*> windScaleTxt;
    QVector<QGraphicsItem*> waveScaleTxt;

    bool windInitScaleInfo;
    bool windScaleInfo;
    bool waveInitScaleInfo;
    bool waveScaleInfo;

    QDateTime refDate;
    QString refDateStr;
    const qint64 step_secs = 21600;

};

#endif // INFOPANEL_H
