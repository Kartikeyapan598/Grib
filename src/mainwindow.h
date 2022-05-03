#pragma once

#include <QDir>
#include <QFileDialog>
#include <QMainWindow>
#include <QDockWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QStandardItemModel>

#include "form.h"
#include "points.h"
#include "Square.h"
#include "rendertypes.h"
#include "DownloadManager.h"

#include <vector>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Paths
{
    QString save_to = QDir::currentPath();
    QString curr_dir = QDir::currentPath();
    QString read_path = QDir::currentPath();
    QString filter = "FileType(*.grib2 *.grib *.csv);;Picture(*.jpg *.gif);;All(*.*)";
};

class MainWindow : public QMainWindow
{

    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void SetDarkMode();
    void UpdateRenders();

    void ShowDock1(bool val);
    void ShowDock2(bool val);
    void ShowDock3(bool val);

    static MainWindow& GetMainWindowInstance() { return *m_instance; }

private slots:

    void on_actionQt_triggered();
    void on_actionExit_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionClose_triggered();
    void on_actionViewPort_triggered();
    void on_actionDrawSquare_triggered();
    void on_actionDraw_Circle_triggered();
    void on_actionRenderTypes_triggered();
    void on_actionAdd_WayPoints_triggered();

    void on_actionOpen_Csv_triggered();

    void on_actionDownLoad_File_triggered();

private :

    void addSquare();

    void createDockView();
    void createDockView2();
    void createDockView3();

    void wheelEvent(QWheelEvent *event);

private:

    Ui::MainWindow* ui;
    static MainWindow* m_instance;

    Paths path;
    QImage* m_img;

    //Form* m_pForm;
    //Form* m_pForm2;
    //Form* m_pForm3;

    std::vector<Form*> m_forms;


    QAction *m_pExit;

    bool is_saved = false;
    bool is_loaded = false;

    std::vector<Square*> sq;

    RenderTypes* m_rendertypes;

    std::vector<QLineF> m_line;
    std::vector<Points*> m_wayPoints;
    std::vector<QDockWidget*> m_dockWidgets;

    QStandardItemModel* m_csvModel;

    //DownloadManager* m_dataManager;
};
