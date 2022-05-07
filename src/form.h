#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QGridLayout>
#include <QGraphicsScene>

#include "customscene.h"
#include "customview.h"

namespace Ui
{
    class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:

    Form() = delete;
    explicit Form(QWidget* parent = nullptr, CustomScene* scene = nullptr);
    ~Form();

    void SetGraphicsScene();
    void clearScene();

    QSize sizeHint() const { return QSize(this->width(), this->height()); }

    CustomScene* GetGraphicsScene() { return m_scene; }
    CustomView* GetGraphicsView() { return m_view; }

protected Q_SLOTS :

    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:

    Ui::Form *ui;

    CustomScene*    m_scene;
    CustomView*     m_view;
    QLayout*        m_layout;

    double m_scaleFactor;
};

#endif // FORM_H
