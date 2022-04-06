#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Grib2 Plotter");

    m_scene = new CustomScene(this);
    m_scene->setSceneRect(ui->graphicsView->rect());
    drawGrid();

    ui->graphicsView->setScene(m_scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    loadImage();
}

void MainWindow::drawGrid()
{
    int skip = 10;

//    qDebug() << ui->graphicsView->rect();

    for (int x = ui->graphicsView->x(), y = ui->graphicsView->y(); y <= ui->graphicsView->height(); y += skip)
        m_scene->addLine(x, y, x + ui->graphicsView->width(), y);

    for (int x = ui->graphicsView->x(), y = ui->graphicsView->y(); x <= ui->graphicsView->width(); x += skip)
        m_scene->addLine(x, y, x, y + ui->graphicsView->height());
}

void MainWindow::openImage(const QString &fname)
{
    if (!m_bgImg.load(fname))
        return;

    m_scene->clearScene();

    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(m_bgImg));
    item->setPos(ui->graphicsView->x(), ui->graphicsView->y());
    m_scene->addItem(item);
}

void MainWindow::loadImage()
{
    QString fname { };
    fname = QFileDialog::getOpenFileName(this, tr("Open Image"), "/home/rudra/", tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));

    if (!fname.size())
    {
        QMessageBox msg;
        msg.setWindowTitle(QString("Error!"));
        msg.setText("Error opening the file: " + fname);
        msg.exec();

        return;
    }

    openImage(fname);
}

void MainWindow::on_actionClear_routes_triggered()
{
    m_scene->clearScene();
}

void MainWindow::on_actionClose_triggered()
{
    close();
}
