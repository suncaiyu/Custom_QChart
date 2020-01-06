#include "Chart.h"
#include "ui_Chart.h"

Chart::Chart(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::Chart)
{
    ui->setupUi(this);
}

Chart::~Chart()
{
    delete ui;
}

void Chart::CreateInitNumber()
{
    for (int i = 0; i <= spliteNumber; i++) {
        dataVector.push_back(qrand() % 100);
    }
}

void Chart::SetChartTopAndBtn()
{
    //    chartTop = chartHeight * myIndex;
    chartTop = myRect.y();
    chartBottom = myRect.y() + myRect.height();
    //    chartBottom = chartHeight * (myIndex+1);
}

void Chart::SetCenterPos()
{
    centerPos = QPoint(myRect.width() /2,myRect.height() /2 + myRect.y());
}
