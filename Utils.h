#pragma once

#include <QWidget>
#include <QMessageBox>

static void CreateMessageBoxInfo(QWidget* parent, QString title, QString mssg)
{
    QMessageBox::information(parent, title, mssg);
    return;
}

static int randomBetween(int low, int high)
{
    return (qrand() % ((high + 1) - low) + low);
}
