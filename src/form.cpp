#include "form.h"
#include "Utils.h"
#include "ui_form.h"

#include <QtMath>
#include <QScrollBar>
#include <QWheelEvent>
#include <QMessageBox>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsBlurEffect>

Form::Form(QWidget *parent, QGraphicsScene* scene, bool val)
    : QWidget(parent)
    , ui(new Ui::Form)
    , m_scene(scene)
{
    ui->setupUi(this);
    m_view = new QGraphicsView(this);

    m_view->resize(700, 600);
    m_view->setScene(m_scene);

    m_view->setDragMode(QGraphicsView::ScrollHandDrag);

    m_layout = this->layout();
    m_layout->addWidget(m_view);
    this->setLayout(m_layout);

    m_img.load("C:/dev/Grib/Grib/src/Resrc/world_map.png");

    mapWidth = m_img.width();
    mapHeight = m_img.height();

    m_view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    // Draw Grid
    zero.setX(0.0);
    zero.setY(0.0);

    if(val)
    {
        CreateImageScene();
    }
}


void Form::CreateImageScene()
{

    float stepi = mapWidth/360.0;

    // Verticacl  lines
    for (int x = 0; x < 360; x++)
    {
        if (x == 179)
        {
            zero.setX(x*stepi - mapWidth/2);
            m_scene->addLine(x*stepi - mapWidth/2, -mapHeight/2 ,x*stepi - mapWidth/2 , mapHeight/2, QPen(Qt::red));
        }
        else m_scene->addLine(x*stepi - mapWidth/2, -mapHeight/2 ,x*stepi - mapWidth/2 , mapHeight/2, QPen(Qt::gray));
    }

    // Horizontal lines
    float stepj = mapHeight/180.0;
    for (int y = 0; y < 180; y++)
    {
        if (y == 89)
        {
            zero.setY(y*stepj - mapHeight/2);
            m_scene->addLine(-mapWidth/2, y*stepj - mapHeight/2, mapWidth/2, y*stepj - mapHeight/2, QPen(Qt::green));
        }
        else m_scene->addLine(-mapWidth/2, y*stepj - mapHeight/2, mapWidth/2, y*stepj - mapHeight/2, QPen(Qt::gray));
    }

    // PixMap

    // x, y, R, G

//    int loop_over = 12;

//    for (int y = 0; y < 1000/*m_Nj*/; y += loop_over)
//    {
//        for (int x = 0; x < 1000/*m_Ni*/; x += loop_over)
//        {
//            QColor col = QColor(0, 255, 0);
//            for(int j = 0; j <= loop_over; j++)
//            {
//                for(int k = 0; k <= loop_over; k++)
//                {
//                    m_img.setPixel(QPoint(200 + y + j, 200 + x + k), /*m[magnitude[x][y]]*/col.rgb());
//                }
//            }
//        }
//    }

    QGraphicsPixmapItem* map = new QGraphicsPixmapItem(QPixmap::fromImage(m_img));

    map->setOffset(-mapWidth / 2, -mapHeight / 2);
    map->setPos(0, 0);
    m_scene->setSceneRect(-mapWidth / 2, -mapHeight / 2, mapWidth, mapHeight);
    m_scene->addItem(map);

}

void Form::wheelEvent(QWheelEvent *event)
{
    event->delta() > 0 ? m_view->scale(1.25, 1.25) : m_view->scale(0.8, 0.8);
    return;
}

void Form::keyPressEvent(QKeyEvent *event)
{
    return;
}

void Form::SetGraphicsScene()
{
    //ui->graphicsView->setScene(m_scene);
}

void Form::mouseMoveEvent(QMouseEvent* e)
{
}

void Form::mousePressEvent(QMouseEvent* e)
{
}

void Form::mouseReleaseEvent(QMouseEvent* e)
{
}

Form::~Form()
{
    delete ui;
}
