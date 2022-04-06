#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QGridLayout>
#include <QGraphicsScene>

namespace Ui
{
    class Form;
}

class Form : public QWidget
{
    Q_OBJECT
public:
    Form() = delete;
    explicit Form(QWidget* parent = nullptr, QGraphicsScene* scene = nullptr);
    ~Form();
    void SetGraphicsScene();
    QGraphicsScene* GetGraphicsScene() { return m_scene; }
public:
    QSize sizeHint() const
    {
        qDebug("%d: %d", this->width(), this->height());
        return QSize(this->width(), this->height());
    }

    void SetScaleFactor(double val) { m_scaleFactor = val; }
protected Q_SLOTS :
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
private:
    Ui::Form *ui;
    QGraphicsScene* m_scene;
    QGraphicsView* m_view;
    //QGridLayout* m_layout;
    QVBoxLayout* AllBox;

    double m_scaleFactor;
};

#endif // FORM_H
