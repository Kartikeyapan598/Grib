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
    explicit Form(QWidget* parent = nullptr, QGraphicsScene* scene = nullptr, bool val = true);
    ~Form();

    void SetGraphicsScene();
    void CreateImageScene();

    QGraphicsScene* GetGraphicsScene() { return m_scene; }

    QImage& GetImage() { return m_img; }
    void SetImage(QImage& image) { m_img = image; }

public:

    void SetScaleFactor(double val) { m_scaleFactor = val; }
    QSize sizeHint() const { qDebug("%d: %d", this->width(), this->height()); return QSize(this->width(), this->height()); }

protected Q_SLOTS :

    void wheelEvent(QWheelEvent* e);
    void keyPressEvent(QKeyEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);

private:
    Ui::Form *ui;
    QGraphicsScene* m_scene;
    QGraphicsView* m_view;
    QLayout* m_layout;

    double m_scaleFactor;

    //

    int mapWidth, mapHeight;
    QPointF zero;
    bool _pan;
    int panX, panY;

    QImage m_img;
};

#endif // FORM_H
