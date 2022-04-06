#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QImage>
#include <QString>
#include <QPixmap>
#include <QMainWindow>
#include <QFileDialog>
#include <QGraphicsPixmapItem>

#include "customscene.h"
#include "customview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QImage m_bgImg;
    void drawGrid();
    void loadImage();
    void openImage(const QString& fname);
    Ui::MainWindow *ui;
    CustomScene* m_scene;

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();
    void on_actionClear_routes_triggered();
    void on_actionClose_triggered();
};
#endif // MAINWINDOW_H
