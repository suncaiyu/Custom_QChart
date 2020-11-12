#ifndef CHARTDATA_H
#define CHARTDATA_H

#include <QObject>
#include <QColor>
#include <QRectF>
class ChartData : public QObject
{
    Q_OBJECT
public:
    ChartData(){};
    virtual ~ChartData(){};
    double height;
    double top;
    double bottom;
    QColor color;
    bool isDraging = false;
    double dragTop;
    bool isSwaping = false;
    double swapingTop;
};

#endif // CHARTDATA_H
