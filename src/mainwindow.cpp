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

    gridData = NULL;

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
    delete gridData;
    gridData = new GridData(fileName.toStdString().c_str());

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

void MainWindow::on_actionDownLoad_File_triggered()
{
    //Crashing Why?
    //m_dataManager->append(QStringList("https://www.win-rar.com/fileadmin/winrar-versions/winrar/winrar-x64-611.exe"));
    DownloadManager* manager = new DownloadManager(NULL);
    QStringList list;
    list.append(QString("https://speed.hetzner.de/100MB.bin"));
    manager->append(list);
    return;
}


