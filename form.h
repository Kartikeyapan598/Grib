#ifndef FORM_H
#define FORM_H

#include <QWidget>
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

private:
    Ui::Form *ui;
    QGraphicsScene* m_scene;
};

#endif // FORM_H
