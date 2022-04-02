#include "rendertypes.h"
#include "ui_rendertypes.h"

#include "mainwindow.h"

RenderTypes::RenderTypes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RenderTypes),
    m_wind(true)
{
    ui->setupUi(this);
    m_weather = false;
    m_temp = false;

    ui->checkBox->setChecked(true);
    ui->checkBox_2->setChecked(false);
    ui->checkBox_3->setChecked(false);

    this->setWindowTitle(QString("Select Render Types"));
    //this->resize(200, 100);
}

RenderTypes::~RenderTypes()
{
    delete ui;
}

void RenderTypes::on_checkBox_clicked() // For Wind
{
    if(ui->checkBox->isChecked())
    {
        m_wind = true;
    }
    else
    {
        m_wind = false;
    }
}


void RenderTypes::on_checkBox_3_clicked()   // For Temp
{
    if(ui->checkBox->isChecked())
    {
        m_temp = true;
    }
    else
    {
        m_temp = false;
    }
}


void RenderTypes::on_checkBox_2_clicked()   // For Weather
{
    if(ui->checkBox->isChecked())
    {
        m_weather = true;
    }
    else
    {
        m_weather = false;
    }
}


void RenderTypes::on_pushButton_clicked()
{
    this->close();
    MainWindow::GetMainWindowInstance().UpdateRenders();
}

