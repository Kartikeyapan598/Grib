#ifndef PLOTTER_H
#define PLOTTER_H

#include "customscene.h"
#include "gridder.h"
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>

class Plotter {

public:

    Plotter (Gridder* gridder);

    void setScene(CustomScene* scene);

    void addWindArrowPlot();
    void addWindColorPlot();
    void addWaveSigHtColorPlot();
    void addWindWaveHtColorPlot();
    void addWindWaveArrowPlot();
    void addSwellWaveHtColorPlot();
    void addSwellWaveArrowPlot();
    void addCurrentArrowPlot();
    void addIsobarPlot();

    void removeWindArrowPlot();
    void removeWindColorPlot();
    void removeWaveSigHtColorPlot();
    void removeWindWaveHtColorPlot();
    void removeWindWaveArrowPlot();
    void removeSwellWaveHtColorPlot();
    void removeSwellWaveArrowPlot();
    void removeCurrentArrowPlot();
    void removeIsobarPlot();

private :

    void    initPlotter();
    void    drawWindArrow ();
    void    drawWindColor ();
    void    drawWaveSigHtColor ();
    void    drawWindWaveHtColor ();
    void    drawSwellWaveHtColor ();
    void    drawWindWaveArrow ();
    void    drawSwellWaveArrow ();
    void    drawCurrentArrow ();
    void    drawIsobars ();


    Gridder* m_gridder;
    CustomScene* m_scene;

    bool isInitWindSpeedDrawn;
    bool areInitWindArrowsDrawn;
    bool isInitWaveSigHtDrawn;
    bool isInitWindWaveHtDrawn;
    bool areInitWindWaveArrowsDrawn;
    bool isInitSwellWaveHtDrawn;
    bool areInitSwellWaveArrowsDrawn;
    bool areInitCurrentArrowsDrawn;
    bool areInitIsobarsDrawn;


    QVector<QGraphicsItem*> windArrowPlot;
    QVector<QGraphicsItem*> windWaveArrowPlot;
    QVector<QGraphicsItem*> swellWaveArrowPlot;
    QVector<QGraphicsItem*> currentArrowPlot;
    QVector<QGraphicsItem*> isobarPlot;

    QGraphicsPixmapItem*    windColorPlot;
    QGraphicsPixmapItem*    waveSigHtColorPlot;
    QGraphicsPixmapItem*    windWaveHtColorPlot;
    QGraphicsPixmapItem*    swellWaveHtColorPlot;

};

#endif // PLOTTER_H
