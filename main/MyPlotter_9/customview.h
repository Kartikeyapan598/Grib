#ifndef CUSTOMVIEW_H
#define CUSTOMVIEW_H

#include <QDebug>
#include <QWidget>
#include <QString>
#include <QMessageBox>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QGraphicsView>

class CustomView : public QGraphicsView
{
public:
    CustomView(QWidget* parent = nullptr);

    void wheelEvent(QWheelEvent* e) override;
};

#endif // CUSTOMVIEW_H
