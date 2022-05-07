#pragma once

#include <QWidget>
#include <QMessageBox>

inline void CreateMessageBoxInfo(QWidget* parent, QString title, QString mssg)
{
    QMessageBox::information(parent, title, mssg);
    return;
}

inline void CreateMessageBoxWarn(QWidget* parent, QString title, QString mssg)
{
    QMessageBox::warning(parent, title, mssg);
    return;
}

inline void CreateMessageBoxCrit(QWidget* parent, QString title, QString mssg)
{
    QMessageBox::critical(parent, title, mssg, QMessageBox::Ok, QMessageBox::NoButton);
    return;
}

inline void CreateMessageBoxAbout(QWidget* parent, QString title, QString mssg)
{
    QMessageBox::about(parent, title, mssg);
    return;
}

inline QMessageBox::StandardButton MultipleButtonBox(QWidget* parent, QString title, QString mssg, QMessageBox var)
{
    return QMessageBox::question(parent, "Standard Button", "question",  QMessageBox::Yes
                                                                        | QMessageBox::No
                                                                        | QMessageBox::Close,
                                                                        QMessageBox::NoButton);

}

static int randomBetween(int low, int high)
{
    return (rand() % ((high + 1) - low) + low);
}

inline float toMerc (float latInDeg) {
    float rad = latInDeg * M_PI /180.0;
    return log (tan (M_PI / 4 + rad / 2)) * 180 / M_PI;
}

inline float fromMerc (float deg) {
    float rad = deg * M_PI /180.0;
    return atan (sinh (rad)) * 180 / M_PI;
}
