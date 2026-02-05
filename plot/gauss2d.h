
#ifndef GAUSS2D_H
#define GAUSS2D_H

#include <math.h>




class Gauss2d
{
public:
    Gauss2d();
    Gauss2d(double A,double x0,double y0,double lx,double ly,double C);
    double A;
    double x0;
    double y0;
    double lx;
    double ly;
    double C;
    bool isValid = false;
    double valueAt(double x,double y);  
    double dA(double x, double y);
    double dx0(double x,double y);
    double dy0(double x,double y);
    double dlx(double x,double y);
    double dly(double x,double y);
    double dc();
    void applyCorrections(double cA, double cx0, double cy0, double clx, double cly, double cC, Gauss2d min, Gauss2d max);
};

#endif // GAUSS2D_H
