#pragma once

#include <QDialog>

namespace Ui
{
    class RenderTypes;
}

class RenderTypes : public QDialog
{
    Q_OBJECT
public:

    explicit RenderTypes(QWidget *parent = nullptr);
    ~RenderTypes();

private :
    bool MultiHeightBoxWarning(bool a, bool b, bool c);

public slots:

//    void on_checkBox_clicked();
//    void on_checkBox_3_clicked();
//    void on_checkBox_2_clicked();
    void on_pushButton_clicked();

private slots:



//    void on_WaveGroupBox_clicked();

    void on_SHeightWindSwellBox_clicked();

    void on_SwellWaveHeightBox_clicked();

    void on_WindWaveHeightBox_clicked();

    void on_CurrentBox_clicked();

    void on_SwellWaveDirectionBox_clicked();

    void on_WindWaveDirectionBox_clicked();

    void on_WindSpeedColorBox_clicked();

    void on_WindBarbsBox_clicked();

    void on_IsobarsBox_clicked();

private:

    Ui::RenderTypes *ui;
    bool m_weather, m_temp, m_wind;

};

