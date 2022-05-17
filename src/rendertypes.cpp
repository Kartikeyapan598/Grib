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
//    ui->checkBox->setChecked(true);
//    ui->checkBox_2->setChecked(false);
//    ui->checkBox_3->setChecked(false);
    ui->WindSpeedColorBox->setChecked(true);
    ui->WindBarbsBox->setChecked(true);
    this->setFixedSize(510, 530);
    this->setWindowTitle(QString("Select Render Types"));
}

RenderTypes::~RenderTypes()
{
    delete ui;
}

//void RenderTypes::on_checkBox_clicked() // For Wind
//{
//    m_wind = !m_wind;
//    MainWindow::GetMainWindowInstance().ShowDock1(m_wind);
//    // Show data for wind
//    return;
//}

//void RenderTypes::on_checkBox_2_clicked()   // For Weather
//{
//    m_weather = !m_weather;
//    MainWindow::GetMainWindowInstance().ShowDock2(m_weather);
//    return;
//}

//void RenderTypes::on_checkBox_3_clicked()   // For Temp
//{
//    m_temp = !m_temp;
//    MainWindow::GetMainWindowInstance().ShowDock3(m_temp);
//    return;
//}

void RenderTypes::on_pushButton_clicked()
{
    this->close();
}



//void RenderTypes::on_WaveGroupBox_clicked()
//{
//    MainWindow& instance = MainWindow::GetMainWindowInstance();
//    if(ui->WaveGroupBox->isChecked())
//    {
//        instance.addWaveSigHtColorPlot(0);
//        instance.addWindWaveArrowPlot(0);
//        instance.addSwellWaveArrowPlot(0);

//        ui->SHeightWindSwellBox->setChecked(true);
//        ui->WindWaveDirectionBox->setChecked(true);
//        ui->SwellWaveDirectionBox->setChecked(true);
//    }
//    else
//    {
//        instance.removeSwellWaveArrowPlot();
//        instance.removeWindWaveArrowPlot();
//        instance.removeWaveSigHtColorPlot();
//        instance.removeWindWaveHtColorPlot();
//        instance.removeSwellWaveHtColorPlot();
//    }
//    return;
//}

bool RenderTypes::MultiHeightBoxWarning(bool a, bool b, bool c)
{
    bool val = (a && b) || (b && c) || (c && a);
    if(val)
    {
        CreateMessageBoxCrit(NULL, "Multiple Heights Selected", "Only one type of Height Can be displayed at a \"time\"");
//        CreateMessageBoxInfo(NULL, QString::number(this->height()), QString::number(this->width()));

        ui->SHeightWindSwellBox->setChecked(false);
        ui->SwellWaveHeightBox->setChecked(false);
        ui->WindWaveHeightBox->setChecked(false);
    }
    return val;
}

void RenderTypes::on_SHeightWindSwellBox_clicked()
{
    MainWindow& instance = MainWindow::GetMainWindowInstance();
    if(ui->SHeightWindSwellBox->isChecked())
    {
        instance.removeWindWaveHtColorPlot(0);
        instance.removeSwellWaveHtColorPlot(0);
        instance.removeWindColorPlot(0);
        ui->WindWaveHeightBox->setChecked(false);
        ui->WindSpeedColorBox->setChecked(false);
        ui->SwellWaveHeightBox->setChecked(false);

        instance.addWaveSigHtColorPlot(0);
    }
    else
    {
        instance.removeWaveSigHtColorPlot(0);
    }
    return;

//    bool windPlusSwell = ui->SHeightWindSwellBox->isChecked();
//    bool swellHeight = ui->SwellWaveHeightBox->isChecked();
//    bool windHeight = ui->WindWaveHeightBox->isChecked();

//    if(!MultiHeightBoxWarning(windPlusSwell, swellHeight, windHeight) && windPlusSwell == true)
//    {
//        // Draw wave height Swell + Wind
//    }
//    if(!windPlusSwell)
//    {
//        // Clear
//    }
}


void RenderTypes::on_SwellWaveHeightBox_clicked()
{
    MainWindow& instance = MainWindow::GetMainWindowInstance();
    if(ui->SwellWaveHeightBox->isChecked())
    {
        instance.removeWindWaveHtColorPlot(0);
        instance.removeWaveSigHtColorPlot(0);
        instance.removeWindColorPlot(0);

        ui->SHeightWindSwellBox->setChecked(false);
        ui->WindWaveHeightBox->setChecked(false);
        ui->WindSpeedColorBox->setChecked(false);

        instance.addSwellWaveHtColorPlot(0);
    }
    else
    {
        instance.removeSwellWaveHtColorPlot(0);
    }
    return;
//    bool windPlusSwell = ui->SHeightWindSwellBox->isChecked();
//    bool swellHeight = ui->SwellWaveHeightBox->isChecked();
//    bool windHeight = ui->WindWaveHeightBox->isChecked();

//    if(!MultiHeightBoxWarning(windPlusSwell, swellHeight, windHeight) && swellHeight == true)
//    {
//        // Draw Swell height
//    }
//    if(!swellHeight)
//    {
//        // clear scene
//    }
}

void RenderTypes::on_WindWaveHeightBox_clicked()
{
    MainWindow& instance = MainWindow::GetMainWindowInstance();
    if(ui->WindWaveHeightBox->isChecked())
    {
        instance.removeWaveSigHtColorPlot(0);
        instance.removeWindColorPlot(0);
        instance.removeSwellWaveHtColorPlot(0);
        ui->SHeightWindSwellBox->setChecked(false);
        ui->SwellWaveHeightBox->setChecked(false);
        ui->WindSpeedColorBox->setChecked(false);

        instance.addWindWaveHtColorPlot(0);
    }
    else
    {
        instance.removeWindWaveHtColorPlot(0);
    }
    return;
//    bool windPlusSwell = ui->SHeightWindSwellBox->isChecked();
//    bool swellHeight = ui->SwellWaveHeightBox->isChecked();
//    bool windHeight = ui->WindWaveHeightBox->isChecked();

//    if(!MultiHeightBoxWarning(windPlusSwell, swellHeight, windHeight) && windHeight == true)
//    {
//        // Draw Wind Wave height
//    }
//    if(!windHeight)
//    {
//        // Clear
//        CreateMessageBoxInfo(NULL, "HAHA", "No Message");
//    }
}


void RenderTypes::on_CurrentBox_clicked()
{
    MainWindow& instance = MainWindow::GetMainWindowInstance();
    if(ui->CurrentBox->isChecked())
    {
        instance.addCurrentArrowPlot(0);
    }
    else
    {
        instance.removeCurrentArrowPlot(0);
    }
    return;
}


void RenderTypes::on_SwellWaveDirectionBox_clicked()
{
    MainWindow& instance = MainWindow::GetMainWindowInstance();
    if(ui->SwellWaveDirectionBox->isChecked())
    {
        instance.addSwellWaveArrowPlot(0);
    }
    else
    {
        instance.removeSwellWaveArrowPlot(0);
    }
    return;
}


void RenderTypes::on_WindWaveDirectionBox_clicked()
{
    MainWindow& instance = MainWindow::GetMainWindowInstance();
    if(ui->WindWaveDirectionBox->isChecked())
    {
        instance.addWindWaveArrowPlot(0);
    }
    else
    {
        instance.removeWindWaveArrowPlot(0);
    }
}


void RenderTypes::on_WindSpeedColorBox_clicked()
{
    MainWindow& instance = MainWindow::GetMainWindowInstance();
    if(ui->WindSpeedColorBox->isChecked())
    {
        instance.removeWindWaveHtColorPlot(0);
        instance.removeWaveSigHtColorPlot(0);
        instance.removeSwellWaveHtColorPlot(0);
        ui->SHeightWindSwellBox->setChecked(false);
        ui->SwellWaveHeightBox->setChecked(false);
        ui->WindWaveHeightBox->setChecked(false);

        instance.addWindColorPlot(0);
    }
    else
    {
        instance.removeWindColorPlot(0);
    }
}


void RenderTypes::on_WindBarbsBox_clicked()
{
    MainWindow& instance = MainWindow::GetMainWindowInstance();
    if(ui->WindBarbsBox->isChecked())
    {

        instance.addWindArrowPlot(0);
    }
    else
    {
        instance.removeWindArrowPlot(0);
    }
}

