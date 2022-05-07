#include "customscene.h"
#include <QGraphicsSvgItem>

CustomScene::CustomScene(QWidget* parent)
    : QGraphicsScene(parent)
{

    QGraphicsSvgItem *map = new QGraphicsSvgItem("/home/pr6khar/Xtreme/Projects/Main/Grib/src/Resrc/world_map.svg");

    QRectF mapRect = map->boundingRect();
    mapWidth = mapRect.width();
    mapHeight = mapRect.height();

    map->setPos(-mapWidth / 2, -mapHeight / 2);
    setSceneRect(-mapWidth / 2, -mapHeight / 2, mapWidth, mapHeight);
    addItem(map);

    zero.setX(0.0);
    zero.setY(0.0);

    // Draw Grid Pattern

    QPen meridianPen = QPen(Qt::red, 0.3);
    QPen gridPen = QPen(Qt::black, 0.3);
    float stepi = stepI();
    float stepj = stepJ();
    float mapXmax = xExtent();
    float mapYmax = yExtent();

    // Vertical Lines (longitudes)

    addLine(0, -mapYmax, 0, mapYmax, meridianPen); // prime meridian

    for (int x = 1; x <= 180; x++) {
        QGraphicsLineItem *lon = addLine(x*stepi, -mapYmax, x*stepi, mapYmax, gridPen);
        lonLatLines.push_back(lon);
    }
    for (int x = -1; x >= -180; x--) {
        QGraphicsLineItem *lon = addLine(x*stepi, -mapYmax, x*stepi, mapYmax, gridPen);
        lonLatLines.push_back(lon);
    }

    // Horizontal lines (latitudes)

    addLine(-mapXmax, 0, mapXmax, 0, meridianPen);

    for (int y = 1; y <= 80; y++) {
        float y_merc = toMerc(y);
        QGraphicsLineItem *lat = addLine(-mapXmax, -y_merc*stepj, mapXmax, -y_merc*stepj, gridPen);
        lonLatLines.push_back(lat);

    }

    for (int y = -1; y >= -80; y--) {
        float y_merc = toMerc(y);
        QGraphicsLineItem *lat = addLine(-mapXmax, -y_merc*stepj, mapXmax, -y_merc*stepj, gridPen);
        lonLatLines.push_back(lat);
    }

    initColourScale();
}

void CustomScene::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    QPointF p = e->scenePos();

    // lon, lat at (x, y)
    QPointF ll;
    ll.setX((p.x() - zero.x())/stepI());
    float lat_mercInv = -(p.y() - zero.y())/stepJ();
    ll.setY(fromMerc(lat_mercInv));

    if (e->button() == Qt::LeftButton)
    {
        double rad = 1;
        QGraphicsEllipseItem* el = addEllipse(p.x() - rad, p.y() - rad, rad*2.0, rad*2.0,
                                                    QPen(Qt::red), QBrush(Qt::SolidPattern));
        el->setZValue(std::numeric_limits<qreal>::max()); // on top of every item

        m_sceneItems.push_back(el);

        QString s = "(" + QString::number(ll.x()) + ", " + QString::number(ll.y()) + ")"; // (lon, lat)

        QGraphicsTextItem* item = new QGraphicsTextItem(s);
        item->setPos(p.x() + 2, p.y() + 2);

        m_sceneItems.push_back(item);

        if (!m_wayPointCoord.empty())
            removeItem(m_wayPointCoord.back());

        m_wayPointCoord.push_back(item);
        addItem(m_wayPointCoord.back());

        if (!m_lastPoint.isNull())
        {
            QGraphicsLineItem* item2 = addLine(m_lastPoint.x(), m_lastPoint.y(), p.x(), p.y(), QPen(Qt::green));
            item2->setZValue(std::numeric_limits<qreal>::max()); // on top of every item

            m_sceneItems.push_back(item2);
        }
        m_lastPoint = p;
    }
    else
    {
        // Waypoints table on right click
        QMessageBox msg;
        msg.setWindowTitle(QString("WayPoints"));
        QString str = "";

        for (int i = 0; i < m_wayPointCoord.size(); i++)
            str+= "Waypoint " + QString::number(i + 1) + " :" + m_wayPointCoord.at(i)->toPlainText() + "\n";
        msg.setText(str);
        msg.exec();
    }
}

void CustomScene::clearScene()
{
    for (int i = 0; i < m_sceneItems.size(); ++i)
        removeItem(m_sceneItems[i]);

    m_sceneItems.clear();
    m_wayPointCoord.clear();

    m_lastPoint.setX(0);
    m_lastPoint.setY(0);
}

void CustomScene::initColourScale() {

    QMap<int, QColor> colScale;            // pre-defined colour scale
    colScale[0] = QColor(40, 140, 255);
    colScale[3] = QColor(0, 175, 255);
    colScale[6] = QColor(0, 220, 225);
    colScale[9] = QColor(0, 247, 176);
    colScale[12] = QColor(0, 234, 156);
    colScale[15] = QColor(130, 240, 89);
    colScale[18] = QColor(240, 245, 3);
    colScale[21] = QColor(255, 237, 0);
    colScale[24] = QColor(255, 219, 0);
    colScale[27] = QColor(255, 199, 0);
    colScale[30] = QColor(255, 180, 0);
    colScale[33] = QColor(255, 152, 0);
    colScale[36] = QColor(255, 126, 0);
    colScale[39] = QColor(247, 120, 0);
    colScale[42] = QColor(236, 120, 20);
    colScale[45] = QColor(228, 113, 30);
    colScale[48] = QColor(224, 97, 40);
    colScale[51] = QColor(220, 81, 50);
    colScale[54] = QColor(213, 69, 60);
    colScale[57] = QColor(205, 58, 70);
    colScale[60] = QColor(190, 44, 80);
    colScale[63] = QColor(180, 26, 90);
    colScale[66] = QColor(170, 20, 100);
    colScale[69] = QColor(150, 40, 120);
    colScale[72] = QColor(140, 50, 140);
    colScale[75] = QColor(150, 50, 180);
    colScale[78] = QColor(200, 50, 255);

    for (int i = 0; i < 78; i += 3) {
        for (int j = i; j < i + 3; j++) {

            int ra = colScale[i].red(), ga = colScale[i].green(), ba = colScale[i].blue();
            int rb = colScale[i + 3].red(), gb = colScale[i + 3].green(), bb = colScale[i + 3].blue();

            int rj = (rb - ra > 0) ? ra + (rb - ra)*(j - i)/3 : ra - (ra - rb)*(j - i)/3;
            int gj = (gb - ga > 0) ? ga + (gb - ga)*(j - i)/3 : ga - (ga - gb)*(j - i)/3;
            int bj = (bb - ba > 0) ? ba + (bb - ba)*(j - i)/3 : ba - (ba - bb)*(j - i)/3;

            m_col[j] = QColor(rj, gj, bj);
        }
    }
    m_col[78] = colScale[78];
}
