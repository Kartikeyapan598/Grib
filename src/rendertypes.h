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

public slots:

    void on_checkBox_clicked();
    void on_checkBox_3_clicked();
    void on_checkBox_2_clicked();
    void on_pushButton_clicked();

private:

    Ui::RenderTypes *ui;
    bool m_weather, m_temp, m_wind;

};

