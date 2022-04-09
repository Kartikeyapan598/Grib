#include "form.h"
#include "ui_form.h"

#include <QtMath>
#include <QScrollBar>
#include <QWheelEvent>
#include <QGraphicsView>

Form::Form(QWidget *parent, CustomScene* scene)
    : QWidget(parent)
    , ui(new Ui::Form)
    , m_scene(scene)
{
    ui->setupUi(this);
    m_view = new CustomView(this);

    //m_view->resize(700, 600);
    m_view->setScene(m_scene);

    //m_view->setDragMode(QGraphicsView::ScrollHandDrag);
    //setDragMode(QGraphicsView::ScrollHandDrag);

    m_layout = this->layout();
    m_layout->addWidget(m_view);
    this->setLayout(m_layout);
}

void Form::wheelEvent(QWheelEvent *event)
{
    //event->delta() > 0 ? m_view->scale(1.25, 1.25) : m_view->scale(0.8, 0.8);

    //event->angleDelta().y() > 0 ? m_view->scale(1.25, 1.25) : m_view->scale(0.8, 0.8);
    return;
}

void Form::keyPressEvent(QKeyEvent *event)
{
    return;
}

void Form::clearScene()
{
    m_scene->clearScene();
}

void Form::SetGraphicsScene()
{
    //ui->graphicsView->setScene(m_scene);
}

Form::~Form()
{
    delete ui;
}
