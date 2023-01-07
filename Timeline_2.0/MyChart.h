#ifndef MYCHART_H
#define MYCHART_H

#include <QQuickPaintedItem>

class MyChart : public QQuickPaintedItem
{
    Q_OBJECT
public:
    MyChart(QQuickItem *parent = nullptr);
    virtual ~MyChart();

protected:
    void paint(QPainter *painter) override;

signals:

};

#endif // MYCHART_H
