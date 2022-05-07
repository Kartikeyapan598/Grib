#ifndef CUSTOMVIEW_H
#define CUSTOMVIEW_H

#include <QWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QGraphicsView>

class CustomView : public QGraphicsView
{
public:
    CustomView(QWidget* parent = nullptr);

    void wheelEvent(QWheelEvent* e) override;
    void resizeEvent(QResizeEvent *event) override;
};

#endif // CUSTOMVIEW_H
