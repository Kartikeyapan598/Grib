#pragma once

#include <QDir>
#include <QFileDialog>
#include <QMainWindow>
#include <QDockWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
// // //
#include <DataQString.h>
#include <GribReaderTop.h>
#include <QTextEdit>
#include <QLayout>
#include <QGroupBox>
// // //
#include "form.h"
#include "points.h"
#include "Square.h"
#include "rendertypes.h"
#include "customscene.h"
#include "customview.h"

#include <vector>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Paths
{
    QString curr_dir = QDir::currentPath();
    QString read_path = QDir::currentPath();
    QString filter = "FileType(*.grib2 *grib);;Picture(*.jpg *.gif);;All(*.*)";
    QString save_to = QDir::currentPath();
};

//Box to output information about grib file - can be removed later
class box : public QWidget {
    public:

    QTextEdit* gribInfo;
    QHBoxLayout* lay;
    QVBoxLayout* mainLay;


    box (QWidget* parent) {
       mainLay = new QVBoxLayout(this);
       QGroupBox* horizontalGroupBox = new QGroupBox;
       gribInfo = new QTextEdit;
       lay = new QHBoxLayout;
       horizontalGroupBox->setLayout(lay);
       mainLay->addWidget(gribInfo);
       mainLay->addWidget(horizontalGroupBox);

    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static MainWindow& GetMainWindowInstance() { return *m_instance; }
    void UpdateRenders();
    // // //
    void openMeteoDataFile (const QString& fileName); // for opening grib file
    void showGribInfo (); // displaying grib file info
    // // //

private slots:

    void on_actionExit_triggered();
    void on_actionQt_triggered();
    void on_actionDrawSquare_triggered();
    void on_actionViewPort_triggered();
    void on_actionRenderTypes_triggered();

    void on_actionOpen_triggered();

    void on_actionClose_triggered();

    void on_actionSave_triggered();

    void on_actionDraw_Circle_triggered();

    void on_actionAdd_WayPoints_triggered();

    void on_actionClear_scene_triggered();

private:

    void addSquare();
    void createDockView(CustomScene* scene);
    void wheelEvent(QWheelEvent *event);
    // // //
    QString windData(); // wind data as string
    QString currentData(); // current data as string
    // // //
private:
    QDockWidget *m_pDockWidget2;

    Form *m_pForm;
    QAction *m_pExit;
    Ui::MainWindow *ui;

    RenderTypes* m_rendertypes;

    std::vector<Square*> sq;

    static MainWindow* m_instance;
    Paths path;

    bool is_loaded = false;
    bool is_saved = false;

    std::vector<Points*> m_wayPoints;
    std::vector<QLineF> m_line;

    // // //
    QString      gribFilePath;
    GribReaderTop* gribReaderTop;
    std::vector<long> listGribDates;
    time_t currentDate;
    box* textBox;
    // // //
};
