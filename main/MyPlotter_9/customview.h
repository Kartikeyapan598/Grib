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
    double m_scaleFactor;
    const double m_EPSILON = 1e-6;

public:
    CustomView(QWidget* parent = nullptr);

    void wheelEvent(QWheelEvent* e) override;
};

#endif // CUSTOMVIEW_H
