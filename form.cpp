#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent, QGraphicsScene* scene)
    : QWidget(parent)
    , ui(new Ui::Form)
    , m_scene(scene)
{
    ui->setupUi(this);
}

void Form::SetGraphicsScene()
{
    ui->graphicsView->setScene(m_scene);
}

Form::~Form()
{
    delete ui;
}
