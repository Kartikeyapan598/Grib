#pragma once

#include <QDir>
#include <QFileDialog>
#include <QMainWindow>
#include <QDockWidget>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "form.h"
//#include "points.h"
//#include "Square.h"
#include "rendertypes.h"
#include "customscene.h"
#include "customview.h"
#include "DownloadManager.h"
#include "griddata.h"
#include <vector>
#include "data.h"
#include "plotter.h"
#include "csvroutes.h"
#include "infopanel.h"

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

    Plotter &getPlotter(int fc) { return *(m_plotters.at(fc)); }

    time_t getRefDt () const { return gridData->getRefDate(); }
    int getNbFc () const { return gridData->getNbForecasts(); }

    void addCsvRoutePlot();

    bool windSpeedOnScene() {return isWindSpeedOnScene;}
    bool windArrowsOnScene() { return areWindArrowsOnScene; }
    bool waveSigHtOnScene() { return isWaveSigHtOnScene; }
    bool windWaveHtOnScene() { return isWindWaveHtOnScene; }
    bool windWaveArrowsOnScene() { return areWindWaveArrowsOnScene; }
    bool swellWaveHtOnScene() { return isSwellWaveHtOnScene; }
    bool swellWaveArrowsOnScene() { return areSwellWaveArrowsOnScene; }
    bool currentArrowsOnScene() { return areCurrentArrowsOnScene; }
    bool isobarsOnScene() { return areIsobarsOnScene; }

    void setWindSpeedOnScene(bool drawn) {isWindSpeedOnScene = drawn;}
    void setWindArrowsOnScene(bool drawn) { areWindArrowsOnScene = drawn; }
    void setWaveSigHtOnScene(bool drawn) { isWaveSigHtOnScene = drawn; }
    void setWindWaveHtOnScene(bool drawn) { isWindWaveHtOnScene = drawn; }
    void setWindWaveArrowsOnScene(bool drawn) { areWindWaveArrowsOnScene = drawn; }
    void setSwellWaveHtOnScene(bool drawn) { isSwellWaveHtOnScene = drawn; }
    void setSwellWaveArrowsOnScene(bool drawn) { areSwellWaveArrowsOnScene = drawn; }
    void setCurrentArrowsOnScene(bool drawn) {areCurrentArrowsOnScene = drawn; }
    void setIsobarsOnScene(bool drawn) {areIsobarsOnScene = drawn; }

    inline int getForecastPtr() const { return m_fcPtr; }
    void updateElementsOnScene(int prevFc);


private slots:

    void on_actionExit_triggered ();
    void on_actionQt_triggered ();
    void on_actionRenderTypes_triggered ();
    void on_actionOpen_triggered ();
    void on_actionClose_triggered ();
    void on_actionSave_triggered ();
    void on_actionDownLoad_File_triggered ();

    void on_actionUpload_CSV_triggered();

    void on_actionInfo_Panel_triggered();

private:

    void createDockView (CustomScene* scene);
    void wheelEvent (QWheelEvent *event);

    void    drawGrid();

    void friend InfoPanel::setForecastPtr(int newPtr);



private:

    QDockWidget *m_pDockWidget2;
    Form        *m_pForm;
    QAction     *m_pExit;

    Ui::MainWindow *ui;

    RenderTypes* m_rendertypes;
    InfoPanel* m_infoPanel;

//    std::vector<Square*> sq;

    static MainWindow* m_instance;
    Paths path;

    bool is_loaded = false;
    bool is_saved = false;

//    std::vector<Points*>    m_wayPoints;
    std::vector<QLineF>     m_line;

    QString fileName;
    GridData* gridData;
    CsvRoutes* routes;
    vector<Plotter*> m_plotters;
    int m_fcPtr;

    bool isGridDrawn;


    bool isWindSpeedOnScene;
    bool areWindArrowsOnScene;
    bool isWaveSigHtOnScene;
    bool isWindWaveHtOnScene;
    bool areWindWaveArrowsOnScene;
    bool isSwellWaveHtOnScene;
    bool areSwellWaveArrowsOnScene;
    bool areCurrentArrowsOnScene;
    bool areIsobarsOnScene;


};
