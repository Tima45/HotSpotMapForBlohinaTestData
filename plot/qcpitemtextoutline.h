#ifndef QCPITEMTEXTOUTLINE_H
#define QCPITEMTEXTOUTLINE_H

#include "qcustomplot.h"

class QCPItemTextOutline : public QCPItemText
{
public:
    QCPItemTextOutline(QCustomPlot *parentPlot);
    // Методы для настройки контура
    void setOutlinePen(const QPen &pen);
    QPen outlinePen() const;
protected:
    QPen mOutlinePen;
    virtual void draw(QCPPainter *painter) override;
};

#endif // QCPITEMTEXTOUTLINE_H
