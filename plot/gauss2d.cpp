
#include "gauss2d.h"
#include <QDebug>
Gauss2d::Gauss2d()
{

}

Gauss2d::Gauss2d(double A, double x0, double y0, double lx, double ly, double C)
{
    this->A = A;
    this->x0 = x0;
    this->y0 = y0;
    this->lx = lx;
    this->ly = ly;
    this->C = C;
}

double Gauss2d::valueAt(double x, double y)
{
    return C + A*exp(-(((powf(x-x0,2.0))/(2.0*lx*lx))+((powf(y-y0,2.0))/(2.0*ly*ly))));
}

double Gauss2d::dA(double x, double y)
{
    return exp(-(powf(x-x0,2.0)/(2.0*lx*lx))-(powf(y-y0,2.0)/(2.0*ly*ly)));
}

double Gauss2d::dx0(double x, double y)
{
    return (A*exp(-(powf(x-x0,2.0)/(2.0*lx*lx))-(powf(y-y0,2.0)/(2.0*ly*ly)))*(x-x0))/(lx*lx);
}

double Gauss2d::dy0(double x, double y)
{
    return (A*exp(-(powf(x-x0,2.0)/(2.0*lx*lx))-(powf(y-y0,2.0)/(2.0*ly*ly)))*(y-y0))/(ly*ly);
}

double Gauss2d::dlx(double x, double y)
{
    return (A*exp(-(powf(x-x0,2.0)/(2.0*lx*lx))-(powf(y-y0,2.0)/(2.0*ly*ly)))*(x-x0)*(x-x0))/(lx*lx*lx);
}

double Gauss2d::dly(double x, double y)
{
    return (A*exp(-(powf(x-x0,2.0)/(2.0*lx*lx))-(powf(y-y0,2.0)/(2.0*ly*ly)))*(y-y0)*(y-y0))/(ly*ly*ly);
}

double Gauss2d::dc()
{
    return 1;
}

void Gauss2d::applyCorrections(double cA, double cx0, double cy0, double clx, double cly, double cC, Gauss2d min, Gauss2d max)
{
    if(A-cA > min.A && A-cA < max.A)
        A -= cA;
    if(x0-cx0 > min.x0 && x0-cx0 < max.x0)
        x0 -= cx0;
    if(y0-cy0 > min.y0 && y0-cy0 < max.y0)
        y0 -= cy0;
    if(lx-clx > min.lx && lx-clx < max.lx)
        lx -= clx;
    if(ly-cly > min.ly && ly-cly < max.ly)
        ly -= cly;
    if(C-cC > min.C && C-cC < max.C)
        C -= cC;
}

