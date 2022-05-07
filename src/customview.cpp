#include "customview.h"

CustomView::CustomView(QWidget* parent)
    : QGraphicsView(parent)
{
}

void CustomView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
}
void CustomView::wheelEvent(QWheelEvent *e)
{
    static const double factor = 1.1;
    static double currentScale = 1.0;
    static const double scaleMin = 0.7;
    ViewportAnchor oldAnchor = transformationAnchor();

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse); // set focus to mouse coords

    if (e->angleDelta().y() > 0){
        scale(factor, factor);
        currentScale *= factor;
    }
    else if (currentScale > scaleMin){  // restrict zoom out after a certain scale
        scale(1 / factor, 1 / factor);
        currentScale /= factor;
    }
    setTransformationAnchor(oldAnchor); // reset anchor
}
