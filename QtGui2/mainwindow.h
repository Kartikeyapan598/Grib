#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLayout>
#include <QGroupBox>
#include <DataQString.h>
#include <GribReaderTop.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class box : public QWidget {
    public:

    QTextEdit* gribInfo;
    QHBoxLayout* lay;
    QVBoxLayout* mainLay;


    box (QWidget* parent) {
       mainLay = new QVBoxLayout(this);
       QGroupBox* horizontalGroupBox = new QGroupBox;
       gribInfo = new QTextEdit;
       lay = new QHBoxLayout;
       horizontalGroupBox->setLayout(lay);
       mainLay->addWidget(gribInfo);
       mainLay->addWidget(horizontalGroupBox);

    }
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int w, int h, QWidget *parent = nullptr);
    ~MainWindow();
    void openMeteoDataFile (const QString& fileName);
    void slotFile_Info_GRIB ();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
    void createActions();
    QString strippedName(const QString &fullFileName);

private slots:
    void open();


private:

    QString      gribFileName;
    QString      gribFilePath;
    QTextEdit    *gribText;
    QString      gribFile;

    GribReaderTop* gribReaderTop;
    std::vector<long> listGribDates;
    time_t currentDate;
    QString valueIJ;
    box* textBox;

};
#endif // MAINWINDOW_H
