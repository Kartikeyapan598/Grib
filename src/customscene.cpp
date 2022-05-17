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

    QPen meridianPen = QPen(Qt::red, 0.5);
    QPen gridPen = QPen(QColor(30, 30, 30), 0.5);
    meridianPen.setCosmetic(true);
    gridPen.setCosmetic(true);
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

    initWindColourScale();
    initWaveColourScale();
}

void CustomScene::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    QPointF p = e->scenePos();

    // lon, lat at (x, y)
    QPointF ll;
    ll.setX((p.x() - zero.x())*(360.0/mapWidth));

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

void CustomScene::clearScene ()
{
    for (int i = 0; i < m_sceneItems.size(); ++i)
        removeItem(m_sceneItems[i]);

    m_sceneItems.clear();
    m_wayPointCoord.clear();

    m_lastPoint.setX(0);
    m_lastPoint.setY(0);
}

void CustomScene::initWindColourScale ()
{
    // pre-defined colour scale
    m_wCol[0] = QColor(40, 140, 255);
    m_wCol[3] = QColor(0, 175, 255);
    m_wCol[6] = QColor(0, 220, 225);
    m_wCol[9] = QColor(0, 247, 176);
    m_wCol[12] = QColor(0, 234, 156);
    m_wCol[15] = QColor(130, 240, 89);
    m_wCol[18] = QColor(240, 245, 3);
    m_wCol[21] = QColor(255, 237, 0);
    m_wCol[24] = QColor(255, 219, 0);
    m_wCol[27] = QColor(255, 199, 0);
    m_wCol[30] = QColor(255, 180, 0);
    m_wCol[33] = QColor(255, 152, 0);
    m_wCol[36] = QColor(255, 126, 0);
    m_wCol[39] = QColor(247, 120, 0);
    m_wCol[42] = QColor(236, 120, 20);
    m_wCol[45] = QColor(228, 113, 30);
    m_wCol[48] = QColor(224, 97, 40);
    m_wCol[51] = QColor(220, 81, 50);
    m_wCol[54] = QColor(213, 69, 60);
    m_wCol[57] = QColor(205, 58, 70);
    m_wCol[60] = QColor(190, 44, 80);
    m_wCol[63] = QColor(180, 26, 90);
    m_wCol[66] = QColor(170, 20, 100);
    m_wCol[69] = QColor(150, 40, 120);
    m_wCol[72] = QColor(140, 50, 140);
    m_wCol[75] = QColor(150, 50, 180);
    m_wCol[78] = QColor(200, 50, 255);

    std::vector<int> v ={0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48, 51, 54, 57, 60, 63, 66, 69, 72, 75, 78};

    fillColorMap(m_wCol, v);
}

void CustomScene::initWaveColourScale ()
{
    // pre-defined colour scale
    m_wvCol[0] = QColor(40, 140, 255);
    m_wvCol[1] = QColor(0, 175, 255);
    m_wvCol[2] = QColor(0, 220, 255);
    m_wvCol[5] = QColor(0, 247, 176);
    m_wvCol[10] = QColor(130, 240, 89);
    m_wvCol[15] = QColor(240, 245, 3);
    m_wvCol[20] = QColor(255, 237, 0);
    m_wvCol[25] = QColor(255, 219, 0);
    m_wvCol[30] = QColor(255, 199, 0);
    m_wvCol[35] = QColor(255, 180, 0);
    m_wvCol[40] = QColor(255, 152, 0);
    m_wvCol[45] = QColor(255, 126, 0);
    m_wvCol[50] = QColor(247, 120, 0);
    m_wvCol[55] = QColor(236, 120, 20);
    m_wvCol[60] = QColor(228, 113, 30);
    m_wvCol[70] = QColor(224, 97, 40);
    m_wvCol[80] = QColor(220, 81, 50);
    m_wvCol[90] = QColor(213, 69, 60);
    m_wvCol[100] = QColor(205, 58, 70);
    m_wvCol[120] = QColor(190, 44, 80);
    m_wvCol[140] = QColor(180, 26, 90);
    m_wvCol[160] = QColor(170, 20, 100);
    m_wvCol[180] = QColor(150, 40, 120);
    m_wvCol[200] = QColor(140, 50, 140);

    std::vector<int> v ={0, 1, 2, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 70, 80, 90, 100, 120, 140, 160, 180, 200};

    fillColorMap(m_wvCol, v);
    m_wvCol[-1] = QColor(0, 0, 0);
}


void CustomScene::clearInitialGrid ()
{
    for (int i = 0; i < lonLatLines.size(); ++i) {
        removeItem(lonLatLines[i]);
    }
    initialGrid = false;
}
