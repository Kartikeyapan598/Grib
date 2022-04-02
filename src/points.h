#pragma once

#include <QGraphicsItem>

#include <vector>

using namespace std;

class Points : public QObject, public QGraphicsItem
{
    Q_OBJECT
public :
    explicit Points(QObject *parent = 0);
    ~Points();

signals :

private :
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

public slots:
};

struct PointSet
{
    PointSet(QGraphicsScene* scene);
private :
    QGraphicsScene* m_scene;
    vector<Points> m_wayPoints;
};
