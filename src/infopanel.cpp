#include "infopanel.h"
#include "ui_infopanel.h"
#include "mainwindow.h"

InfoPanel* InfoPanel::m_infoPanelInstance;

InfoPanel::InfoPanel(QWidget *parent) :
                        QDialog(parent),
                        ui(new Ui::InfoPanel)
{
    if(!m_infoPanelInstance) {
        m_infoPanelInstance = this;
    }
    else {
        return;
    }

    ui->setupUi(this);

    setWindowTitle("Info Panel");
    setWindowFlag(Qt::NoDropShadowWindowHint);

    ui->prevDayFcInfoBtn->setDisabled(true);
    ui->prevFcInfoBtn->setDisabled(true);
}

InfoPanel::~InfoPanel()
{
    delete ui;
}

void InfoPanel::on_prevDayFcInfoBtn_clicked()
{
    MainWindow& mw_instance = MainWindow::GetMainWindowInstance();

    if (mw_instance.getForecastPtr() - 4 < 1)
    {
        ui->prevFcInfoBtn->setDisabled(true);
    }
    if (mw_instance.getForecastPtr() - 4 < 4)
    {
        ui->prevDayFcInfoBtn->setDisabled(true);
    }
    if (mw_instance.getForecastPtr() - 4 < mw_instance.getNbFc() - 1)
    {
        ui->nextFcInfoBtn->setDisabled(false);
    }
    if (mw_instance.getForecastPtr() - 4 < mw_instance.getNbFc() - 4)
    {
        ui->nextDayFcInfoBtn->setDisabled(false);
    }

    int prevFc = mw_instance.getForecastPtr();
    updateCurrentDate(mw_instance.getForecastPtr() - 4);
    setForecastPtr(mw_instance.getForecastPtr() - 4);
    mw_instance.updateElementsOnScene(prevFc);

}

void InfoPanel::on_prevFcInfoBtn_clicked()
{
    MainWindow& mw_instance = MainWindow::GetMainWindowInstance();

    if (mw_instance.getForecastPtr() - 1 < 1)
    {
        ui->prevFcInfoBtn->setDisabled(true);
    }
    if (mw_instance.getForecastPtr() - 1 < 4)
    {
        ui->prevDayFcInfoBtn->setDisabled(true);
    }
    if (mw_instance.getForecastPtr() - 1 < mw_instance.getNbFc() - 1)
    {
        ui->nextFcInfoBtn->setDisabled(false);
    }
    if (mw_instance.getForecastPtr() - 1 < mw_instance.getNbFc() - 4)
    {
        ui->nextDayFcInfoBtn->setDisabled(false);
    }

    int prevFc = mw_instance.getForecastPtr();
    updateCurrentDate(mw_instance.getForecastPtr() - 1);
    setForecastPtr(mw_instance.getForecastPtr() - 1);
    mw_instance.updateElementsOnScene(prevFc);
}


void InfoPanel::on_nextFcInfoBtn_clicked()
{
    MainWindow& mw_instance = MainWindow::GetMainWindowInstance();

    if (mw_instance.getForecastPtr() + 1 >= mw_instance.getNbFc() - 1)
    {
        ui->nextFcInfoBtn->setDisabled(true);
    }
    if (mw_instance.getForecastPtr() + 1 >= mw_instance.getNbFc() - 4)
    {
        ui->nextDayFcInfoBtn->setDisabled(true);
    }
    if (mw_instance.getForecastPtr() + 1 > 0)
    {
        ui->prevFcInfoBtn->setDisabled(false);
    }
    if (mw_instance.getForecastPtr() + 1 >= 4)
    {
        ui->prevDayFcInfoBtn->setDisabled(false);
    }

    int prevFc = mw_instance.getForecastPtr();
    updateCurrentDate(mw_instance.getForecastPtr() + 1);
    setForecastPtr(mw_instance.getForecastPtr() + 1);
    mw_instance.updateElementsOnScene(prevFc);
}


void InfoPanel::on_nextDayFcInfoBtn_clicked()
{
    MainWindow& mw_instance = MainWindow::GetMainWindowInstance();

    if (mw_instance.getForecastPtr() + 4 >= mw_instance.getNbFc() - 1)
    {
        ui->nextFcInfoBtn->setDisabled(true);
    }
    if (mw_instance.getForecastPtr() + 4 >= mw_instance.getNbFc() - 4)
    {
        ui->nextDayFcInfoBtn->setDisabled(true);
    }
    if (mw_instance.getForecastPtr() + 4 > 0)
    {
        ui->prevFcInfoBtn->setDisabled(false);
    }
    if (mw_instance.getForecastPtr() + 4 >= 4)
    {
        ui->prevDayFcInfoBtn->setDisabled(false);
    }

    int prevFc = mw_instance.getForecastPtr();
    updateCurrentDate(mw_instance.getForecastPtr() + 4);
    setForecastPtr(mw_instance.getForecastPtr() + 4);
    mw_instance.updateElementsOnScene(prevFc);
}

void InfoPanel::setRefDate (time_t refDate)
{
    this->refDate = QDateTime::fromSecsSinceEpoch(refDate);
    refDateStr = QLocale::system().toString(this->refDate, QLocale::ShortFormat);
    ui->refDateTextLabel->setText(refDateStr);
    ui->currDateTextLabel->setText(refDateStr);
}

void InfoPanel::setForecastPtr (int newPtr)
{
    MainWindow& mw_instance = MainWindow::GetMainWindowInstance();
    mw_instance.m_fcPtr = newPtr;
}

void InfoPanel::updateCurrentDate (int fc)
{
    QDateTime currDate = refDate.addSecs(step_secs * fc);
    QString currDateStr = QLocale::system().toString(currDate, QLocale::ShortFormat);
    ui->currDateTextLabel->setText(currDateStr);
}
