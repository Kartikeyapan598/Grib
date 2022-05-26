#ifndef CUSTOMSCENE_H
#define CUSTOMSCENE_H

#include <QWidget>
#include <QMessageBox>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QtMath>
#include "Utils.h"


class CustomScene : public QGraphicsScene
{

public :

    CustomScene(QWidget* parent = nullptr);

    void    mousePressEvent (QGraphicsSceneMouseEvent* e) override;
    void    mouseMoveEvent(QGraphicsSceneMouseEvent* e) override;
    void    clearScene ();
    void    clearInitialGrid();

    int     xExtent ()  const  {  return mapWidth / 2; }
    int     yExtent ()  const  {  return mapHeight / 2; }
    float   stepI ()    const  {  return mapWidth / 360.0; }
    float   stepJ ()    const  {  return mapHeight / (2 * toMerc (80.0)); }
    QPointF zeroLonLat () const { return zero; }

    const std::unordered_map<int, QColor> &GetWindColourScale() const { return m_wCol; }
    const std::unordered_map<int, QColor> &GetWaveColourScale() const { return m_wvCol; }

private :

    void initWindColourScale();
    void initWaveColourScale();

    bool initialGrid = true;

    QPointF m_lastPoint;
    QVector<QGraphicsItem*>     m_sceneItems;
    QVector<QGraphicsItem*>     lonLatLines;
    QVector<QGraphicsTextItem*> m_wayPointCoord;

    QPointF zero; // (x, y) at (0, 0) lon lat
    int     mapWidth;
    int     mapHeight;
    std::unordered_map<int, QColor> m_wCol;
    std::unordered_map<int, QColor> m_wvCol;
};


#endif // CUSTOMSCENE_H
