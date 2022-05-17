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

    m_view->setScene(m_scene);
    m_view->setBackgroundBrush(QBrush(QColor(80, 180, 200), Qt::SolidPattern));
    m_layout = this->layout();
    m_layout->addWidget(m_view);
    this->setLayout(m_layout);
}

void Form::wheelEvent(QWheelEvent *event)
{
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
