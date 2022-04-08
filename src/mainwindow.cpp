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
    QGraphicsScene* m_scene = new QGraphicsScene();

    QPen pen = QPen(Qt::red);
    m_scene->setSceneRect(-400, -200, 300, 300);

    // Draw Grid Pattern

    m_scene->setBackgroundBrush(QBrush(QPixmap(QString("C:/dev/Grib/Grib/src/Resrc/pngegg.png"))));
    //setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

    //

    createDockView(m_scene);
    addDockWidget(Qt::LeftDockWidgetArea, m_pDockWidget2);

    // Select various rendering options like temp, weather, wind etc
    m_rendertypes = new RenderTypes(this);

    QLineF Topline(m_scene->sceneRect().topLeft(), m_scene->sceneRect().topRight());
    QLineF Leftline(m_scene->sceneRect().topLeft(), m_scene->sceneRect().bottomLeft());
    QLineF Rightline(m_scene->sceneRect().topRight(), m_scene->sceneRect().bottomRight());
    QLineF Bottomline(m_scene->sceneRect().bottomLeft(), m_scene->sceneRect().bottomRight());

    m_scene->addLine(Topline, pen);
    m_scene->addLine(Leftline, pen);
    m_scene->addLine(Rightline, pen);
    m_scene->addLine(Bottomline, pen);

    QBrush red = QBrush(Qt::red);
    QBrush blue = QBrush(Qt::blue);
    QPen blackpen = QPen(Qt::black);
    blackpen.setWidth(2);
    m_scene->addEllipse(50, 50, 100, 100, blackpen, red);
    QGraphicsRectItem* rect = m_scene->addRect(-100, -100, 50, 50, blackpen, blue);
    rect->setFlag(QGraphicsItem::ItemIsMovable);
    qApp->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createDockView(QGraphicsScene* scene)
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
    QString fileNmae = QFileDialog::getOpenFileName(this, "FileName", path.curr_dir, path.filter);
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


void MainWindow::on_actionAdd_WayPoints_triggered()
{
    if(m_wayPoints.size() <= 1)
    {
        CreateMessageBoxInfo(this, "WayPoints", "Not enough Points to make Paths");
    }
    if(m_wayPoints.size() > 1)
    {
        QGraphicsScene* scene = m_pForm->GetGraphicsScene();
        for(int i = 0; i < m_wayPoints.size() - 1; i++)
        {
            QLineF line(m_wayPoints[i]->pos(), m_wayPoints[i + 1]->pos());
            m_line.push_back(line);
            scene->addLine(line);
        }
    }
}

