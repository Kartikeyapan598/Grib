#include "rendertypes.h"
#include "ui_rendertypes.h"

#include "Utils.h"
#include "mainwindow.h"
#include "infopanel.h"

RenderTypes::RenderTypes(QWidget *parent) :
    QDialog(parent), ui(new Ui::RenderTypes), m_wind(true)
{
    ui->setupUi(this);

    m_temp = false;
    m_weather = false;

    ui->WindSpeedColorBox->setChecked(true);
    ui->WindBarbsBox->setChecked(true);

    this->setFixedSize(430, 400);
    this->setWindowTitle(QString("Select Render Types"));
}

RenderTypes::~RenderTypes()
{
    delete ui;
}


void RenderTypes::on_pushButton_clicked()
{
    this->close();
}


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
    InfoPanel& info_instance = InfoPanel::getInfoPanelInstance();

    if (ui->SHeightWindSwellBox->isChecked())
    {        
        if (instance.windWaveHtOnScene())
            instance.getPlotter(instance.getForecastPtr()).removeWindWaveHtColorPlot();

        if (instance.swellWaveHtOnScene())
            instance.getPlotter(instance.getForecastPtr()).removeSwellWaveHtColorPlot();

        if (instance.windSpeedOnScene())
            instance.getPlotter(instance.getForecastPtr()).removeWindColorPlot();

        instance.setWindWaveHtOnScene(false);
        instance.setSwellWaveHtOnScene(false);
        instance.setWindSpeedOnScene(false);

        ui->WindWaveHeightBox->setChecked(false);
        ui->WindSpeedColorBox->setChecked(false);
        ui->SwellWaveHeightBox->setChecked(false);

        instance.getPlotter(instance.getForecastPtr()).addWaveSigHtColorPlot();
        instance.setWaveSigHtOnScene(true);

        if (info_instance.getWindScaleInfo())
        {
            info_instance.setWindScaleInfo(false);
            info_instance.removeWindColourScale();
            info_instance.addWaveColourScale();
            info_instance.setWaveScaleInfo(true);
        }
    }
    else
    {
        instance.getPlotter(instance.getForecastPtr()).removeWaveSigHtColorPlot();
        instance.setWaveSigHtOnScene(false);
    }
    return;

}

void RenderTypes::on_SwellWaveHeightBox_clicked()
{
    MainWindow& instance = MainWindow::GetMainWindowInstance();
    InfoPanel& info_instance = InfoPanel::getInfoPanelInstance();

    if (ui->SwellWaveHeightBox->isChecked())
    {
        if (instance.windWaveHtOnScene())
            instance.getPlotter(instance.getForecastPtr()).removeWindWaveHtColorPlot();

        if (instance.waveSigHtOnScene())
            instance.getPlotter(instance.getForecastPtr()).removeWaveSigHtColorPlot();

        if (instance.windSpeedOnScene())
            instance.getPlotter(instance.getForecastPtr()).removeWindColorPlot();

        instance.setWindWaveHtOnScene(false);
        instance.setWaveSigHtOnScene(false);
        instance.setWindSpeedOnScene(false);

        ui->SHeightWindSwellBox->setChecked(false);
        ui->WindWaveHeightBox->setChecked(false);
        ui->WindSpeedColorBox->setChecked(false);

        instance.getPlotter(instance.getForecastPtr()).addSwellWaveHtColorPlot();
        instance.setSwellWaveHtOnScene(true);

        if (info_instance.getWindScaleInfo())
        {
            info_instance.setWindScaleInfo(false);
            info_instance.removeWindColourScale();
            info_instance.addWaveColourScale();
            info_instance.setWaveScaleInfo(true);
        }
    }
    else
    {
        instance.getPlotter(instance.getForecastPtr()).removeSwellWaveHtColorPlot();
        instance.setSwellWaveHtOnScene(false);
    }
    return;

}

void RenderTypes::on_WindWaveHeightBox_clicked()
{
    MainWindow& instance = MainWindow::GetMainWindowInstance();
    InfoPanel& info_instance = InfoPanel::getInfoPanelInstance();
    if (ui->WindWaveHeightBox->isChecked())
    {
        if (instance.waveSigHtOnScene())
            instance.getPlotter(instance.getForecastPtr()).removeWaveSigHtColorPlot();

        if (instance.windSpeedOnScene())
            instance.getPlotter(instance.getForecastPtr()).removeWindColorPlot();

        if (instance.swellWaveHtOnScene())
            instance.getPlotter(instance.getForecastPtr()).removeSwellWaveHtColorPlot();

        instance.setWaveSigHtOnScene(false);
        instance.setSwellWaveHtOnScene(false);
        instance.setWindSpeedOnScene(false);

        ui->SHeightWindSwellBox->setChecked(false);
        ui->SwellWaveHeightBox->setChecked(false);
        ui->WindSpeedColorBox->setChecked(false);

        instance.getPlotter(instance.getForecastPtr()).addWindWaveHtColorPlot();
        instance.setWindWaveHtOnScene(true);

        if (info_instance.getWindScaleInfo())
        {
            info_instance.setWindScaleInfo(false);
            info_instance.removeWindColourScale();
            info_instance.addWaveColourScale();
            info_instance.setWaveScaleInfo(true);
        }
    }
    else
    {
        instance.getPlotter(instance.getForecastPtr()).removeWindWaveHtColorPlot();
        instance.setWindWaveHtOnScene(false);
    }
    return;

}


void RenderTypes::on_CurrentBox_clicked()
{
    MainWindow& instance = MainWindow::GetMainWindowInstance();

    if(ui->CurrentBox->isChecked())
    {
        instance.getPlotter(instance.getForecastPtr()).addCurrentArrowPlot();
        instance.setCurrentArrowsOnScene(true);
    }
    else
    {
        instance.getPlotter(instance.getForecastPtr()).removeCurrentArrowPlot();
        instance.setCurrentArrowsOnScene(false);
    }
    return;
}


void RenderTypes::on_SwellWaveDirectionBox_clicked()
{
    MainWindow& instance = MainWindow::GetMainWindowInstance();
    if(ui->SwellWaveDirectionBox->isChecked())
    {
        instance.getPlotter(instance.getForecastPtr()).addSwellWaveArrowPlot();
        instance.setSwellWaveArrowsOnScene(true);
    }
    else
    {
        instance.getPlotter(instance.getForecastPtr()).removeSwellWaveArrowPlot();
        instance.setSwellWaveArrowsOnScene(false);
    }
    return;
}


void RenderTypes::on_WindWaveDirectionBox_clicked()
{
    MainWindow& instance = MainWindow::GetMainWindowInstance();
    if(ui->WindWaveDirectionBox->isChecked())
    {
        instance.getPlotter(instance.getForecastPtr()).addWindWaveArrowPlot();
        instance.setWindWaveArrowsOnScene(true);
    }
    else
    {
        instance.getPlotter(instance.getForecastPtr()).removeWindWaveArrowPlot();
        instance.setWindWaveArrowsOnScene(false);
    }
}


void RenderTypes::on_WindSpeedColorBox_clicked()
{
    MainWindow& instance = MainWindow::GetMainWindowInstance();
    InfoPanel& info_instance = InfoPanel::getInfoPanelInstance();

    if (ui->WindSpeedColorBox->isChecked())
    {
        if (instance.windWaveArrowsOnScene())
            instance.getPlotter(instance.getForecastPtr()).removeWindWaveHtColorPlot();

        if (instance.waveSigHtOnScene())
            instance.getPlotter(instance.getForecastPtr()).removeWaveSigHtColorPlot();

        if (instance.swellWaveHtOnScene())
            instance.getPlotter(instance.getForecastPtr()).removeSwellWaveHtColorPlot();

        instance.setWaveSigHtOnScene(false);
        instance.setSwellWaveHtOnScene(false);
        instance.setWindWaveHtOnScene(false);

        ui->SHeightWindSwellBox->setChecked(false);
        ui->SwellWaveHeightBox->setChecked(false);
        ui->WindWaveHeightBox->setChecked(false);

        instance.getPlotter(instance.getForecastPtr()).addWindColorPlot();
        instance.setWindSpeedOnScene(true);

        if (info_instance.getWaveScaleInfo())
        {
            info_instance.setWaveScaleInfo(false);
            info_instance.removeWaveColourScale();
            info_instance.addWindColourScale();
            info_instance.setWindScaleInfo(true);
        }
    }
    else
    {
        instance.getPlotter(instance.getForecastPtr()).removeWindColorPlot();
        instance.setWindSpeedOnScene(false);
    }
}


void RenderTypes::on_WindBarbsBox_clicked()
{
    MainWindow& instance = MainWindow::GetMainWindowInstance();
    if(ui->WindBarbsBox->isChecked())
    {

        instance.getPlotter(instance.getForecastPtr()).addWindArrowPlot();
        instance.setWindArrowsOnScene(true);
    }
    else
    {
        instance.getPlotter(instance.getForecastPtr()).removeWindArrowPlot();
        instance.setWindArrowsOnScene(false);
    }
}




void RenderTypes::on_IsobarsBox_clicked()
{
    MainWindow& instance = MainWindow::GetMainWindowInstance();
    if(ui->IsobarsBox->isChecked())
    {
        instance.getPlotter(instance.getForecastPtr()).addIsobarPlot();
        instance.setIsobarsOnScene(true);
    }
    else
    {
        instance.getPlotter(instance.getForecastPtr()).removeIsobarPlot();
        instance.setIsobarsOnScene(false);
    }
}

