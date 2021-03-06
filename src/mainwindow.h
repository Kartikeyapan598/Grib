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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static MainWindow& GetMainWindowInstance() { return *m_instance; }
    void UpdateRenders();

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

private:

    void addSquare();
    void createDockView(QGraphicsScene* scene);
    void wheelEvent(QWheelEvent *event);

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
};
