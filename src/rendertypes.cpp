#include "rendertypes.h"
#include "ui_rendertypes.h"

#include "Utils.h"
#include "mainwindow.h"

RenderTypes::RenderTypes(QWidget *parent) :
    QDialog(parent), ui(new Ui::RenderTypes), m_wind(true)
{
    ui->setupUi(this);

    m_temp = false;
    m_weather = false;

    ui->checkBox->setChecked(true);
    ui->checkBox_2->setChecked(false);
    ui->checkBox_3->setChecked(false);

    this->setWindowTitle(QString("Select Render Types"));
}

RenderTypes::~RenderTypes()
{
    delete ui;
}

void RenderTypes::on_checkBox_clicked() // For Wind
{
    m_wind = !m_wind;
    MainWindow::GetMainWindowInstance().ShowDock1(m_wind);
    return;
}

void RenderTypes::on_checkBox_2_clicked()   // For Weather
{
    m_weather = !m_weather;
    MainWindow::GetMainWindowInstance().ShowDock2(m_weather);
    return;
}

void RenderTypes::on_checkBox_3_clicked()   // For Temp
{
    m_temp = !m_temp;
    MainWindow::GetMainWindowInstance().ShowDock3(m_temp);
    return;
}

void RenderTypes::on_pushButton_clicked()
{
    this->close();
}

