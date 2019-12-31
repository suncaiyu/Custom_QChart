#include "ChartWidget.h"
#include "ui_ChartWidget.h"
#include <QPainter>
#include <QDebug>

ChartWidget::ChartWidget(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::ChartWidget)
{
    ui->setupUi(this);

    chart1 = new Chart();
    chart2 = new Chart();
    chart3 = new Chart();
    chart1->spliteNumber = 50;
    chart2->spliteNumber = 50;
    chart3->spliteNumber = 50;

    chart1->timespace = 10;
    chart2->timespace = 10;
    chart3->timespace = 10;

    chart1->myIndex = 0;
    chart2->myIndex = 1;
    chart3->myIndex = 2;

    chart1->CreateInitNumber();
    chart2->CreateInitNumber();
    chart3->CreateInitNumber();

    chart1->chartHeight = 150;
    chart2->chartHeight = 150;
    chart3->chartHeight = 150;


    chart1->SetChartTopAndBtn();
    chart2->SetChartTopAndBtn();
    chart3->SetChartTopAndBtn();

    chart1->chartYNumber = 100;
    chart2->chartYNumber = 100;
    chart3->chartYNumber = 100;
    chart1->stepYnumber = 1.3;
    chart2->stepYnumber = 1.3;
    chart3->stepYnumber = 1.3;
    chart1->backgroundColor = QColor(0, 0, 0, 155);
    chart2->backgroundColor = QColor(0, 0, 0,155);
    chart3->backgroundColor = QColor(05, 0, 0,155);

    chart1->lineColor = QColor(qrand() % 255, qrand() % 255, qrand() % 255);
    chart2->lineColor = QColor(qrand() % 255, qrand() % 255, qrand() % 255);
    chart3->lineColor = QColor(qrand() % 255, qrand() % 255, qrand() % 255);
}

void ChartWidget::updateData()
{
    startIndex++;
    chart1->dataVector.push_back(qrand() % 100);
    chart2->dataVector.push_back(qrand() % 100);
    chart3->dataVector.push_back(qrand() % 100);
    if(isUpdate){
    update();
    }
}

ChartWidget::~ChartWidget()
{
    delete ui;
}

void ChartWidget::StopUpdateSlot()
{
    isUpdate = false;
}

void ChartWidget::StartUpdateSlot()
{
    isUpdate = true;
}


void ChartWidget::PaintChart(QPainter *p, Chart *c)
{
    p->fillRect(QRect(0,(c->myIndex)*c->chartHeight,width(),c->chartHeight), QBrush(c->backgroundColor));
    QPen pen; //创建一个画笔
    pen.setColor(c->lineColor);
    pen.setWidth(1);
    p->setPen(pen);
    float secondData;
    for (int i = startIndex; i < c->dataVector.size(); i++) {
        if (i == startIndex) {
            c->tempData = QPointF(c->timespace , c->dataVector[i] * (c->stepYnumber) + heightSpacing + c->chartTop);
        } else {
            secondData = c->dataVector[i];
            QPointF one = c->tempData;

            QPointF two = QPointF(c->timespace + c->stepXNumber * (i - startIndex), heightSpacing + secondData * (c->stepYnumber) + c->chartTop);

            p->drawLine(one, two);
            c->tempData = two;
        }
    }
}
void ChartWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, 1);

    PaintChart(&painter, chart1);
    PaintChart(&painter, chart2);
    PaintChart(&painter, chart3);
}

void ChartWidget::resizeEvent(QResizeEvent *event)
{
    float panelwidth = width() - 2 * chart1->timespace;
    chart1->stepXNumber = (float) panelwidth / chart1->spliteNumber;
    chart2->stepXNumber = (float) panelwidth / chart2->spliteNumber;
    chart3->stepXNumber = (float) panelwidth / chart3->spliteNumber;
}

