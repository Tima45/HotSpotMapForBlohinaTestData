
#include "hotspotmap.h"

HotSpotMap::HotSpotMap(QCustomPlot *plot, Gauss2d startGauss, Gauss2d minGauss, Gauss2d maxGauss,int fontsize)
    : QCPColorMap(plot->xAxis,plot->yAxis)
{
    this->minGauss = minGauss;
    this->maxGauss = maxGauss;
    f.setPixelSize(fontsize);
    f.setPointSize(fontsize);

    p.setColor(QColor(0,0,255,128));
    p.setWidth(2);



    currentGauss = startGauss;
    this->setInterpolate(false);

    historyCurve = new QCPCurve(this->parentPlot()->xAxis,this->parentPlot()->yAxis);
    historyCurve->setPen(p);
    center = new QCPItemTracer(this->parentPlot());
    center->setStyle(QCPItemTracer::TracerStyle::tsPlus);
    center->setPen(p);
}


void HotSpotMap::rescaleReplot()
{
    if(autoScaleData)
        this->setDataRange(QCPRange(minV,maxV));

    for(int i = 0; i < hotSpotLabels.count(); i++){
        hotSpotLabels.at(i)->setText(QString::number(qRound(hotSpots.at(i).t*10.0)/10.0));
    }

    this->parentPlot()->replot();
}

void HotSpotMap::setSize(int width, int height)
{
    this->data()->setSize(width,height);
}

void HotSpotMap::setRanges(QCPRange xRange, QCPRange yRange)
{
    this->data()->setRange(xRange,yRange);
    this->parentPlot()->xAxis->setRange(this->data()->keyRange());
    this->parentPlot()->yAxis->setRange(this->data()->valueRange());
}

void HotSpotMap::addHotSpot(const HotSpot hs)
{
    hotSpots.append(hs);



    hotSpotLabels.append(new QCPItemTextOutline(this->parentPlot()));
    //hotSpotLabels.last()->setOutlinePen(QPen(QColor(Qt::black))));
    hotSpotLabels.last()->position->setCoords(hs.x,hs.y);
    hotSpotLabels.last()->setFont(f);
    hotSpotLabels.last()->setColor(QColor(Qt::green));


}

void HotSpotMap::setTemperatureAt(int index, double t)
{
    hotSpots[index].t = t;
}

void HotSpotMap::plotGauss()
{
    maxV = -99999999;
    minV = 999999999;
    for(int x = 0; x < this->data()->keySize(); x++){
        for(int y = 0; y < this->data()->valueSize(); y++){
            double pointX = 0;
            double pointY = 0;
            this->data()->cellToCoord(x,y,&pointX,&pointY);
            double v = currentGauss.valueAt(pointX,pointY);
            this->data()->setCell(x,y,v);
            if(v > maxV)
                maxV = v;
            if(v < minV)
                minV = v;
        }
    }

    if(curveCounter >= INT_MAX-10){
        curveCounter = 0;
        historyCurve->data()->clear();
    }

    historyCurve->addData(curveCounter,currentGauss.x0,currentGauss.y0);
    curveCounter++;
    while(historyCurve->dataCount() >= historyLength){
        historyCurve->data()->remove(curveCounter - historyCurve->dataCount());
    }
    center->position->setCoords(currentGauss.x0,currentGauss.y0);
}

void HotSpotMap::calculateGausses(int stepsCount)
{
    //currentGauss = Gauss2d(50,-10,5,30,30,25);
    int c = 0;
    while(c < stepsCount){
        double correction_A  = 0;
        double correction_x0 = 0;
        double correction_y0 = 0;
        double correction_lx = 0;
        double correction_ly = 0;
        double correction_c  = 0;
        for(int i = 0; i < hotSpots.count(); i++){
            double error = 0.9 * (hotSpots.at(i).t - currentGauss.valueAt(hotSpots.at(i).x,hotSpots.at(i).y)) / (1.0*hotSpots.count());
            correction_A += -error*currentGauss.dA(hotSpots.at(i).x,hotSpots.at(i).y);
            correction_x0 += -error*currentGauss.dx0(hotSpots.at(i).x,hotSpots.at(i).y);
            correction_y0 += -error*currentGauss.dy0(hotSpots.at(i).x,hotSpots.at(i).y);
            correction_lx += -error*currentGauss.dlx(hotSpots.at(i).x,hotSpots.at(i).y);
            correction_ly += -error*currentGauss.dly(hotSpots.at(i).x,hotSpots.at(i).y);
            correction_c += -error*currentGauss.dc();
        }
        currentGauss.applyCorrections(correction_A,correction_x0,correction_y0,correction_lx,correction_ly,correction_c,minGauss,maxGauss);
        c++;
    }
}

void HotSpotMap::setAngle(double a)
{
    double aShift = a - currentAngle;
    double aRad = aShift * M_PI / 180.0;
    double cosA = cos(aRad);
    double sinA = sin(aRad);
    for(int i = 0; i < hotSpots.count(); i++){
        double x = hotSpots.at(i).x;
        double y = hotSpots.at(i).y;
        double newX = x*cosA - y*sinA;
        double newY = x*sinA + y*cosA;
        hotSpots[i].x = newX;
        hotSpots[i].y = newY;
        hotSpotLabels[i]->position->setCoords(newX,newY);
    }
    currentAngle = a;
}

void HotSpotMap::clearHistory()
{
    historyCurve->data()->clear();
}

