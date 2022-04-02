#pragma once

#include <QMainWindow>
#include <QDockWidget>
#include <QGraphicsScene>

#include "form.h"
#include "Square.h"
#include "rendertypes.h"

#include <vector>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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

private:

    void addSquare();
    void createDockView(QGraphicsScene* scene);

private:
    QDockWidget *m_pDockWidget2;

    Form *m_pForm;
    QAction *m_pExit;
    Ui::MainWindow *ui;

    RenderTypes* m_rendertypes;

    std::vector<Square*> sq;

    static MainWindow* m_instance;
};
