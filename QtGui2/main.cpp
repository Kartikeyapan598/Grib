#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Xtreme Technologies");
    QCoreApplication::setApplicationName("BasicGui");
    QCoreApplication::setApplicationVersion("0.3");

    MainWindow* w = new MainWindow(800, 600);
    w->show();
    return app.exec();
}
