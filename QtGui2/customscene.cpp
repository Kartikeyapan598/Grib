#include "customscene.h"

CustomScene::CustomScene(QWidget* parent)
    : QGraphicsScene(parent)
{
    // World Map Background
    QGraphicsPixmapItem * map = new QGraphicsPixmapItem(QPixmap("/home/pr6khar/Xtreme/QtGui4/world_map.png"));

    mapWidth = map->pixmap().width();
    mapHeight = map->pixmap().height();
    map->setOffset(-mapWidth / 2, -mapHeight / 2);
    map->setPos(0, 0);
    setSceneRect(-mapWidth / 2, -mapHeight / 2, mapWidth, mapHeight);
    addItem(map);

    // Draw Grid
    zero.setX(0.0);
    zero.setY(0.0);

    // Draw Grid Pattern
    // Vertical Lines
    float stepi = mapWidth/360.0;
    for (int x = 0; x < 360; x++) {
        if (x == 179){
            zero.setX(x*stepi - mapWidth/2);
            addLine(x*stepi - mapWidth/2, -mapHeight/2 ,x*stepi - mapWidth/2 , mapHeight/2, QPen(Qt::red));
        }
        else
            addLine(x*stepi - mapWidth/2, -mapHeight/2 ,x*stepi - mapWidth/2 , mapHeight/2, QPen(Qt::gray));
    }

    // Horizontal lines
    float stepj = mapHeight/180.0;
    for (int y = 0; y < 180; y++) {
        if (y == 89) {
            zero.setY(y*stepj - mapHeight/2);
            addLine(-mapWidth/2, y*stepj - mapHeight/2, mapWidth/2, y*stepj - mapHeight/2, QPen(Qt::green));
        }
        else
            addLine(-mapWidth/2, y*stepj - mapHeight/2, mapWidth/2, y*stepj - mapHeight/2, QPen(Qt::gray));
    }

}

void CustomScene::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    QPointF p = e->scenePos();

    qDebug() << p;

    QPointF ll;
    ll.setX((p.x() - zero.x())*(360.0/mapWidth));
    ll.setY(-(p.y() - zero.y())*(180.0/mapHeight));



    if (e->button() == Qt::LeftButton)
    {
        double rad = 1;
        QGraphicsEllipseItem* el = addEllipse(p.x() - rad, p.y() - rad, rad*2.0, rad*2.0, QPen(Qt::red), QBrush(Qt::SolidPattern));
//        el->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
        m_sceneItems.push_back(el);

        QString s = "(" + QString::number(ll.x()) + ", " + QString::number(ll.y()) + ")";
        QGraphicsTextItem* item = new QGraphicsTextItem(s);
        item->setPos(p.x() + 2, p.y() + 2);
        m_sceneItems.push_back(item);
        addItem(item);

        if (!m_lastPoint.isNull())
        {
            QGraphicsLineItem* item2 = addLine(m_lastPoint.x(), m_lastPoint.y(), p.x(), p.y(), QPen(Qt::green));
            m_sceneItems.push_back(item2);
        }

        m_lastPoint = p;
    }
    else
    {
        QMessageBox msg;
        msg.setWindowTitle(QString("Alert!"));
        msg.setText(QString("You clicked the coordinates: ") + QString::number(p.x() + sceneRect().width() / 2) + QString(", ") + QString::number(p.y()) + QString::number(sceneRect().height() / 2));
        qDebug() << sceneRect().x() << '/' << sceneRect().y();
        msg.exec();
    }
}


void CustomScene::clearScene()
{
    for (int i = 0; i < m_sceneItems.size(); ++i)
        removeItem(m_sceneItems[i]);

    m_sceneItems.clear();

    m_lastPoint.setX(0);
    m_lastPoint.setY(0);
}
