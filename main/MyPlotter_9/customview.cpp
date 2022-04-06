#include "customview.h"

CustomView::CustomView(QWidget* parent)
    : QGraphicsView(parent)
{
}

void CustomView::wheelEvent(QWheelEvent *e)
{
    double factor;
    ViewportAnchor oldAnchor = transformationAnchor();

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse); // set focus to mouse coords

    if (e->delta() > 0)
        factor = 1.15;
    else
        factor = 0.8;

    scale(factor, factor);
    setTransformationAnchor(oldAnchor); // reset anchor
}
