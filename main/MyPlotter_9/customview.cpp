#include "customview.h"

CustomView::CustomView(QWidget* parent)
    : QGraphicsView(parent)
{
    m_scaleFactor = 1.0;
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

    qDebug() << factor << '\n';

    scale(factor, factor);
    setTransformationAnchor(oldAnchor); // reset anchor
}
