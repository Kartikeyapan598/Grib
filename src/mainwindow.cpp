#include "Utils.h"
#include "csvloader.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QStyleFactory>

MainWindow* MainWindow::m_instance;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    m_instance = this;
    ui->setupUi(this);

    m_dockWidgets.resize(3);

    // DarkMode
    SetDarkMode();
    //

    // Draw Grid Pattern

    //m_scene->setBackgroundBrush(QBrush(QPixmap(QString("C:/dev/Grib/Grib/src/Resrc/pngegg.png"))));

    //
    createDockView();
    createDockView2();
    createDockView3();

    // Select various rendering options like temp, weather, wind etc
    m_rendertypes = new RenderTypes(this);

    //m_dataManager = new DownloadManager();
    CreateMessageBoxInfo(NULL, "HAHA", QSslSocket::supportsSsl() + '-' + QSslSocket::sslLibraryBuildVersionString() + '-' + QSslSocket::sslLibraryVersionString());

    qDebug() << QSslSocket::sslLibraryBuildVersionString() << " - " << QSslSocket::supportsSsl() << " - "  << QSslSocket::sslLibraryVersionString();
    qApp->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ShowDock1(bool val)
{
    if(val == true)
    {
        addDockWidget(Qt::LeftDockWidgetArea, m_dockWidgets[0]);
        m_dockWidgets[0]->show();
    }
    else
    {
        m_dockWidgets[0]->close();
    }
}

void MainWindow::ShowDock2(bool val)
{
    if(val == true)
    {
        addDockWidget(Qt::LeftDockWidgetArea, m_dockWidgets[1]);
        m_dockWidgets[1]->show();
    }
    else
    {
        m_dockWidgets[1]->close();
    }
    return;
}

void MainWindow::ShowDock3(bool val)
{
    if(val == true)
    {
        addDockWidget(Qt::LeftDockWidgetArea, m_dockWidgets[2]);
        m_dockWidgets[2]->show();
    }
    else
    {
        m_dockWidgets[2]->close();
    }
    return;
}

void MainWindow::createDockView()
{
    QGraphicsScene* scene = new QGraphicsScene();
    scene->setSceneRect(-400, -200, 300, 300);
    m_dockWidgets[0] = new QDockWidget(this);
    m_dockWidgets[0]->setWindowTitle("Wind Data");
    m_dockWidgets[0]->setAllowedAreas(Qt::AllDockWidgetAreas);
    m_dockWidgets[0]->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    Form* m_form0 = new Form(m_dockWidgets[0], scene);
    m_form0->SetGraphicsScene();
    m_dockWidgets[0]->setWidget(m_form0);
    m_forms.push_back(m_form0);
    this->setCentralWidget(m_dockWidgets[0]);
}

void MainWindow::createDockView2()
{
    QGraphicsScene* scene = new QGraphicsScene();
    scene->setSceneRect(-200, -100, 300, 300);

    m_dockWidgets[1] = new QDockWidget();
    m_dockWidgets[1]->setWindowTitle("Weather Data");
    m_dockWidgets[1]->setAllowedAreas(Qt::AllDockWidgetAreas);
    m_dockWidgets[1]->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    Form* m_form1 = new Form(m_dockWidgets[1], scene, false);
    m_forms.push_back(m_form1);
    m_dockWidgets[1]->setWidget(m_form1);

    QBrush blue = QBrush(QColorConstants::Svg::brown);
    QPen blackpen = QPen(Qt::black);
    QGraphicsRectItem* rect = scene->addRect(-100, -100, 50, 50, blackpen, blue);
    rect->setFlag(QGraphicsItem::ItemIsMovable);

    QBrush red = QBrush(Qt::red);
    blackpen.setWidth(2);
    scene->addEllipse(50, 50, 100, 100, blackpen, red);

}

void MainWindow::createDockView3()
{
    QGraphicsScene* scene = new QGraphicsScene();
    scene->setSceneRect(-200, -100, 300, 300);

    m_dockWidgets[2] = new QDockWidget();
    m_dockWidgets[2]->setWindowTitle("Temp Data");
    m_dockWidgets[2]->setAllowedAreas(Qt::AllDockWidgetAreas);
    m_dockWidgets[2]->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    Form* m_form2 = new Form(m_dockWidgets[2], scene, true);
    m_forms.push_back(m_form2);
    m_dockWidgets[2]->setWidget(m_form2);

    QBrush blue = QBrush(Qt::green);
    QPen blackpen = QPen(Qt::black);
    QGraphicsRectItem* rect = scene->addRect(-100, -100, 50, 50, blackpen, blue);
    rect->setFlag(QGraphicsItem::ItemIsMovable);

    QBrush red = QBrush(Qt::red);
    blackpen.setWidth(2);
    scene->addEllipse(50, 50, 100, 100, blackpen, red);
}

void MainWindow::SetDarkMode()
{
    qApp->setStyle(QStyleFactory::create("fusion"));
    QPalette newPalette;
    newPalette.setColor(QPalette::Window,          QColor( 37,  37,  37));
    newPalette.setColor(QPalette::WindowText,      QColor(212, 212, 212));
    newPalette.setColor(QPalette::Base,            QColor( 60,  60,  60));
    newPalette.setColor(QPalette::AlternateBase,   QColor( 45,  45,  45));
    newPalette.setColor(QPalette::PlaceholderText, QColor(127, 127, 127));
    newPalette.setColor(QPalette::Text,            QColor(212, 212, 212));
    newPalette.setColor(QPalette::Button,          QColor( 45,  45,  45));
    newPalette.setColor(QPalette::ButtonText,      QColor(212, 212, 212));
    newPalette.setColor(QPalette::BrightText,      QColor(240, 240, 240));
    newPalette.setColor(QPalette::Highlight,       QColor( 38,  79, 120));
    newPalette.setColor(QPalette::HighlightedText, QColor(240, 240, 240));

    newPalette.setColor(QPalette::Light,           QColor( 60,  60,  60));
    newPalette.setColor(QPalette::Midlight,        QColor( 52,  52,  52));
    newPalette.setColor(QPalette::Dark,            QColor( 30,  30,  30) );
    newPalette.setColor(QPalette::Mid,             QColor( 37,  37,  37));
    newPalette.setColor(QPalette::Shadow,          QColor( 0,    0,   0));

    newPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));

    qApp->setPalette(newPalette);
    return;
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
    QGraphicsScene* scene = m_forms[0]->GetGraphicsScene();
    scene->addItem(item);
}

void MainWindow::on_actionDraw_Circle_triggered()
{
    Points* point = new Points();
    m_wayPoints.push_back(point);
    point->setPos(randomBetween(-600, 200), randomBetween(-300, 200));
    QGraphicsScene* scene = m_forms[0]->GetGraphicsScene();
    scene->addItem(point);
}

void MainWindow::UpdateRenders()
{
    // Update ViewPort rendering and rendering of Wind, Temp, Weather
}

void MainWindow::on_actionViewPort_triggered()
{
    // Hide or Show viewport on Trigger
    m_dockWidgets[0]->addAction(m_dockWidgets[0]->toggleViewAction());
}

void MainWindow::on_actionRenderTypes_triggered()
{
    m_rendertypes->show();
}

void MainWindow::on_actionOpen_triggered()
{
    // DownLoad file from backend

    // Store file on local system

    // Read file and plot on screen

    // Download Csv file
    // Store and Read Csv file

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
        QGraphicsScene* scene = m_forms[0]->GetGraphicsScene();
        for(int i = 0; i < m_wayPoints.size() - 1; i++)
        {
            QLineF line(m_wayPoints[i]->pos(), m_wayPoints[i + 1]->pos());
            m_line.push_back(line);
            scene->addLine(line);
        }
    }
}

void MainWindow::on_actionOpen_Csv_triggered()
{
    CSVOut(m_csvModel);
    //ReadFromFile();
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

