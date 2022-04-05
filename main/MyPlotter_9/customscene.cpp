#include "customscene.h"

CustomScene::CustomScene(QWidget* parent)
    : QGraphicsScene(parent)
{
}

void CustomScene::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    QPointF p = e->scenePos();
    qDebug() << p;

    if (e->button() == Qt::LeftButton)
    {
        QGraphicsEllipseItem* el = addEllipse(p.x(), p.y(), 5, 5);
//        m_points.push_back(el);
        m_sceneItems.push_back(el);

        QString s = "(" + QString::number(p.x()) + ", " + QString::number(p.y()) + ")";
        QGraphicsTextItem* item = new QGraphicsTextItem(s);
        item->setPos(p.x() + 2, p.y() + 2);
//        m_coordinates.push_back(item);
        m_sceneItems.push_back(item);
        addItem(item);

        if (!m_lastPoint.isNull())
        {
            QGraphicsLineItem* item2 = addLine(m_lastPoint.x(), m_lastPoint.y(), p.x(), p.y());
//            m_linesDrawn.push_back(item2);
            m_sceneItems.push_back(item2);
        }

        m_lastPoint = p;
    }
    else
    {
        QMessageBox msg;
        msg.setWindowTitle(QString("Alert!"));
        msg.setText(QString("You clicked the coordinates: ") + QString::number(p.x() + sceneRect().width() / 2) + QString(", ") + QString::number(p.y()) + sceneRect().height() / 2);
        qDebug() << sceneRect().x() << '/' << sceneRect().y();
        msg.exec();
    }
}

void CustomScene::clearScene()
{
//    for (int i = 0; i < m_points.size(); ++i)
//    {
////        qDebug() << i << ' ';
//        removeItem(m_points[i]);
//        removeItem(m_coordinates[i]);
//    }

//    for (int i = 0; i < m_linesDrawn.size(); ++i)
//        removeItem(m_linesDrawn[i]);

//    m_points.clear();
//    m_coordinates.clear();
//    m_linesDrawn.clear();

    for (int i = 0; i < m_sceneItems.size(); ++i)
        removeItem(m_sceneItems[i]);

    m_sceneItems.clear();
    m_lastPoint.setX(0);
    m_lastPoint.setY(0);
}


