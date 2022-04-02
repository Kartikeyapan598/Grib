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

void MainWindow::UpdateRenders()
{
    // Update ViewPort rendering and rendering of Wind, Temp, Weather
}

void MainWindow::on_actionViewPort_triggered()
{
    // Hide or Show viewport on Trigger
    addAction(m_pDockWidget2->toggleViewAction());
}


void MainWindow::on_actionRenderTypes_triggered()
{
    m_rendertypes->show();
}


void MainWindow::on_actionOpen_triggered()
{

}

