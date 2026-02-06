
#ifndef HOTSPOTMAP_H
#define HOTSPOTMAP_H


#include <QObject>
#include <QDebug>
#include "qcustomplot.h"
#include "gauss2d.h"
#include "qcpitemtextoutline.h"


struct HotSpot{
    double x = 0;
    double y = 0;
    double t = 0;
};


class HotSpotMap : public QCPColorMap
{
    Q_OBJECT
public:
    explicit HotSpotMap(QCustomPlot* plot, Gauss2d startGauss, Gauss2d minGauss, Gauss2d maxGauss, int fontsize);
    void rescaleReplot();
    void setSize(int width, int height);
    void setRanges(QCPRange xRange, QCPRange yRange);
    bool autoScaleData = false;
    void addHotSpot(const HotSpot hs);
    void setTemperatureAt(int index,double t);
    // TODO: void removeHotSpot() ???????
    void plotGauss();
    void calculateGausses(int stepsCount);
    void setAngle(double a);
    void clearHistory();

    Gauss2d currentGauss;
    Gauss2d minGauss;
    Gauss2d maxGauss;
    int historyLength = 300;
private:
    double maxV = 0;
    double minV = 0;
    QList<HotSpot> hotSpots;
    QList<QCPItemTextOutline*> hotSpotLabels;
    QFont f;
    QPen p;


    QCPItemTracer * center;
    QCPCurve * historyCurve;
    int curveCounter = 0;
    double currentAngle = 0;

signals:

};

#endif // HOTSPOTMAP_H
