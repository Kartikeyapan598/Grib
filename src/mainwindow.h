#pragma once

#include <QDir>
#include <QFileDialog>
#include <QMainWindow>
#include <QDockWidget>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "form.h"
#include "points.h"
#include "Square.h"
#include "rendertypes.h"
#include "customscene.h"
#include "customview.h"
//#include "DownloadManager.h"
#include "griddata.h"
#include <vector>
#include "data.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Paths
{
    QString curr_dir = QDir::currentPath();
    QString read_path = QDir::currentPath();
    QString filter = "FileType(*.grib2 *.grib *.grb *.grb2);;CSV(*.csv);;All(*.*)";
    QString save_to = QDir::currentPath();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow (QWidget *parent = nullptr);
    ~MainWindow ();
    static MainWindow& GetMainWindowInstance () { return *m_instance; }
    void UpdateRenders ();

    void addWindArrowPlot(int fc);
    void addWindColorPlot(int fc);
    void addWaveSigHtColorPlot(int fc);
    void addWindWaveHtColorPlot(int fc);
    void addWindWaveArrowPlot(int fc);
    void addSwellWaveHtColorPlot(int fc);
    void addSwellWaveArrowPlot(int fc);
    void addCurrentArrowPlot(int fc);

    void removeWindArrowPlot(int fc);
    void removeWindColorPlot(int fc);
    void removeWaveSigHtColorPlot(int fc);
    void removeWindWaveHtColorPlot(int fc);
    void removeWindWaveArrowPlot(int fc);
    void removeSwellWaveHtColorPlot(int fc);
    void removeSwellWaveArrowPlot(int fc);
    void removeCurrentArrowPlot(int fc);

private slots:

    void on_actionExit_triggered ();
    void on_actionQt_triggered ();
    void on_actionDrawSquare_triggered ();
    void on_actionViewPort_triggered ();
    void on_actionRenderTypes_triggered ();
    void on_actionOpen_triggered ();
    void on_actionClose_triggered ();
    void on_actionSave_triggered ();
    void on_actionDraw_Circle_triggered ();
//    void on_actionDownLoad_File_triggered ();

    void on_actionUpload_CSV_triggered();

private:

    void addSquare();
    void createDockView (CustomScene* scene);
    void wheelEvent (QWheelEvent *event);

    void    drawWindArrow (int fc);
    void    drawWindColor (int fc);
    void    drawWaveSigHtColor (int fc);
    void    drawWindWaveHtColor (int fc);
    void    drawSwellWaveHtColor (int fc);
    void    drawWindWaveArrow (int fc);
    void    drawSwellWaveArrow (int fc);
    void    drawCurrentArrow (int fc);
    void    drawGrid();



private:

    QDockWidget *m_pDockWidget2;
    Form        *m_pForm;
    QAction     *m_pExit;

    Ui::MainWindow *ui;

    RenderTypes* m_rendertypes;

    std::vector<Square*> sq;

    static MainWindow* m_instance;
    Paths path;

    bool is_loaded = false;
    bool is_saved = false;

    std::vector<Points*>    m_wayPoints;
    std::vector<QLineF>     m_line;

    QString fileName;
    GridData* gridData;

    bool isGridDrawn;

    bool isInitWindSpeedDrawn;
    bool areInitWindArrowsDrawn;
    bool isInitWaveSigHtDrawn;
    bool isInitWindWaveHtDrawn;
    bool areInitWindWaveArrowsDrawn;
    bool isInitSwellWaveHtDrawn;
    bool areInitSwellWaveArrowsDrawn;
    bool areInitCurrentArrowsDrawn;

    bool isWindSpeedDrawn;
    bool areWindArrowsDrawn;
    bool isWaveSigHtDrawn;
    bool isWindWaveHtDrawn;
    bool areWindWaveArrowsDrawn;
    bool isSwellWaveHtDrawn;
    bool areSwellWaveArrowsDrawn;
    bool areCurrentArrowsDrawn;

    QVector<QGraphicsItem*> windArrowPlot;
    QVector<QGraphicsItem*> windWaveArrowPlot;
    QVector<QGraphicsItem*> swellWaveArrowPlot;
    QVector<QGraphicsItem*> currentArrowPlot;

    QGraphicsPixmapItem*    windColorPlot;
    QGraphicsPixmapItem*    waveSigHtColorPlot;
    QGraphicsPixmapItem*    windWaveHtColorPlot;
    QGraphicsPixmapItem*    swellWaveHtColorPlot;

};
