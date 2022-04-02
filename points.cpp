#include "points.h"

Points::Points(QObject* parent)
    :QObject(parent), QGraphicsItem()
{
}

Points::~Points()
{}

QRectF Points::boundingRect() const
{
    return QRectF(200, 100, 100, 100);
}

void Points::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}

void Points::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

}

void Points::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}

void Points::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

}
