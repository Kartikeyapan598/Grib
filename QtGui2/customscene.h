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
private:
    QPointF m_lastPoint;
    QVector<QGraphicsItem*> m_sceneItems;
    QPointF zero; // 0 deg lat lon
    int mapWidth;
    int mapHeight;

public:
    CustomScene(QWidget* parent = nullptr);

    void mousePressEvent(QGraphicsSceneMouseEvent* e) override;
    void clearScene();
};

#endif // CUSTOMSCENE_H
