#include "points.h"

#include <QCursor>
#include <QPainter>
#include <QGraphicsSceneEvent>

Points::Points(QObject* parent)
    :QObject(parent), QGraphicsItem()
{
}

Points::~Points()
{}

QRectF Points::boundingRect() const
{
    return QRectF(-30,-30,60,60);
}

void Points::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::black);
    painter->setBrush(Qt::green);
    painter->drawEllipse(-15,-15,30,30);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Points::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    this->setPos(mapToScene(event->pos()));
}

void Points::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    this->setCursor(QCursor(Qt::ClosedHandCursor));
    Q_UNUSED(event);
}

void Points::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    this->setCursor(QCursor(Qt::ArrowCursor));
    Q_UNUSED(event);
}
