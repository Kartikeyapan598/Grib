#ifndef CUSTOMSCENE_H
#define CUSTOMSCENE_H

#include <QLine>
#include <QPoint>
#include <QDebug>
#include <QObject>
#include <QWidget>
#include <QString>
#include <QVector>
#include <QMessageBox>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>

class CustomScene : public QGraphicsScene
{
    QPointF m_lastPoint;
    QVector<QGraphicsEllipseItem*> m_points;
    QVector<QGraphicsLineItem*> m_linesDrawn;
    QVector<QGraphicsTextItem*> m_coordinates;
    QVector<QGraphicsItem*> m_sceneItems;

public:
    CustomScene(QWidget* parent = nullptr);

    void mousePressEvent(QGraphicsSceneMouseEvent* e) override;

    void clearScene();
};

#endif // CUSTOMSCENE_H
