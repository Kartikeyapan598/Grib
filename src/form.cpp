#include "form.h"
#include "ui_form.h"

#include <QtMath>
#include <QScrollBar>
#include <QWheelEvent>
#include <QGraphicsView>

Form::Form(QWidget *parent, QGraphicsScene* scene)
    : QWidget(parent)
    , ui(new Ui::Form)
    , m_scene(scene)
{
    ui->setupUi(this);
    m_view = new QGraphicsView(this);
    m_view->setDragMode(QGraphicsView::ScrollHandDrag);
    //setDragMode(QGraphicsView::ScrollHandDrag);

    AllBox = new QVBoxLayout(this);
    AllBox->addWidget(m_view, 1, Qt::Alignment());
    //m_view->resizeAnchor()
    m_view->resize(700, 600);
    m_view->setScene(m_scene);
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

Form::~Form()
{
    delete ui;
}
