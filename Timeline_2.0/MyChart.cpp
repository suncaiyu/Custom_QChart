#include "MyChart.h"
#include <QPainter>
MyChart::MyChart(QQuickItem *parent) :QQuickPaintedItem(parent)
{

}

MyChart::~MyChart()
{

}

void MyChart::paint(QPainter *painter)
{
    painter->fillRect(boundingRect(), Qt::white);
}
