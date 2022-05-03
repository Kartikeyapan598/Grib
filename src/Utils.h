#ifndef UTILS
#define UTILS

#include <QWidget>
#include <QMessageBox>

static void CreateMessageBoxInfo(QWidget* parent, QString title, QString mssg)
{
    QMessageBox::information(parent, title, mssg);
    return;
}

static void CreateMessageBoxWarn(QWidget* parent, QString title, QString mssg)
{
    QMessageBox::warning(parent, title, mssg);
    return;
}

static void CreateMessageBoxCrit(QWidget* parent, QString title, QString mssg)
{
    QMessageBox::critical(parent, title, mssg, QMessageBox::Ok, QMessageBox::NoButton);
    return;
}

static void CreateMessageBoxAbout(QWidget* parent, QString title, QString mssg)
{
    QMessageBox::about(parent, title, mssg);
    return;
}

static QMessageBox::StandardButton MultipleButtonBox(QWidget* parent, QString title, QString mssg, QMessageBox var)
{
    return QMessageBox::question(parent, "Standard Button", "question",  QMessageBox::Yes | QMessageBox::No | QMessageBox::Close, QMessageBox::NoButton);
}

static int randomBetween(int low, int high)
{
    return (rand() % ((high + 1) - low) + low);
}

#endif // Utils.h
