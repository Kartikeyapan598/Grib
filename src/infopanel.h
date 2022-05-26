#ifndef INFOPANEL_H
#define INFOPANEL_H

#include "qdatetime.h"
#include <QDialog>
#include "customscene.h"

namespace Ui {
class InfoPanel;
}

class InfoPanel : public QDialog
{
    Q_OBJECT

public:

    InfoPanel () = delete;

    explicit InfoPanel (QWidget *parent = nullptr);

    ~InfoPanel ();

    static InfoPanel &getInfoPanelInstance () { return *m_infoPanelInstance; }

    void setForecastPtr (int newPtr);
    void setRefDate (time_t refDate);

private slots:
    void on_prevDayFcInfoBtn_clicked();

    void on_prevFcInfoBtn_clicked();

    void on_nextFcInfoBtn_clicked();

    void on_nextDayFcInfoBtn_clicked();


private:
    static InfoPanel* m_infoPanelInstance;
    void friend CustomScene::mouseMoveEvent(QGraphicsSceneMouseEvent* e);

    void updateCurrentDate (int fc);

    Ui::InfoPanel *ui;
    QDateTime refDate;
    QString refDateStr;
    const qint64 step_secs = 21600;

};

#endif // INFOPANEL_H
