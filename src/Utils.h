#pragma once

#include <QWidget>
#include <QMessageBox>

void CreateMessageBoxInfo(QWidget* parent, QString title, QString mssg)
{
    QMessageBox::information(parent, title, mssg);
    return;
}

void CreateMessageBoxWarn(QWidget* parent, QString title, QString mssg)
{
    QMessageBox::warning(parent, title, mssg);
    return;
}

void CreateMessageBoxCrit(QWidget* parent, QString title, QString mssg)
{
    QMessageBox::critical(parent, title, mssg, QMessageBox::Ok, QMessageBox::NoButton);
    return;
}

void CreateMessageBoxAbout(QWidget* parent, QString title, QString mssg)
{
    QMessageBox::about(parent, title, mssg);
    return;
}

QMessageBox::StandardButton MultipleButtonBox(QWidget* parent, QString title, QString mssg, QMessageBox var)
{
    return QMessageBox::question(parent, "Standard Button", "question",  QMessageBox::Yes | QMessageBox::No | QMessageBox::Close, QMessageBox::NoButton);

    //QMessageBox::StandardButton ret = QMessageBox::question(this, "Standard Button", "question",  QMessageBox::Yes | QMessageBox::No | QMessageBox::Close, QMessageBox::NoButton);
    //switch (ret)
    //{
    //case QMessageBox::Yes:
    //{
    //    ui->plainTextEdit->appendPlainText("QMessageBox::yes");
    //    break;
    //}
    //case QMessageBox::No:
    //{
    //    ui->plainTextEdit->appendPlainText("QMessageBox::No");
    //    break;
    //}
    //case QMessageBox::Close:
    //{
    //    ui->plainTextEdit->appendPlainText("QMessageBox::Close");
    //    break;
    //}
    //default:
    //{
    //    ui->plainTextEdit->appendPlainText("switch default ");
    //    break;
    //}
    //}
}

static int randomBetween(int low, int high)
{
    return (rand() % ((high + 1) - low) + low);
}
