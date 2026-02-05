
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QDebug>
#include "plot/qcustomplot.h"
#include "plot/hotspotmap.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void serverConnected();
    void serverDisconnected();
private slots:
    void updateColorMap();
    void on_rescaleButton_clicked();

    void on_rescaleButton_3_clicked();

    void on_spinBox_valueChanged(int arg1);
    void goNext();

    void on_spinBox_2_valueChanged(int arg1);

    void on_checkBox_toggled(bool checked);

    void on_angleShiftBox_valueChanged(double arg1);



private:
    Ui::MainWindow *ui;
    HotSpotMap *colorMap;
    void loadConfig(QString filePath);
    bool cfgok = false;
    double diameter = 1;
    int fontsize = 10000;
    double angleShift = 0;
    void initPlot();
    QVector<HotSpot> hs;
    QTimer updaterT;
    QTimer tim;

    QList<QList<double>> data;
};

#endif // MAINWINDOW_H
