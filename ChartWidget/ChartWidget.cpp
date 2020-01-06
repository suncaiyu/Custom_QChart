#include "ChartWidget.h"
#include "ui_ChartWidget.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

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

    chart1->myRect = chart1->fixRect = QRect(0,
                                             (chart1->myIndex) * chart1->chartHeight,
                                             width(),
                                             chart1->chartHeight);

    chart2->myRect = chart2->fixRect = QRect(0,
                                             (chart2->myIndex) * chart2->chartHeight,
                                             width(),
                                             chart2->chartHeight);

    chart3->myRect = chart3->fixRect = QRect(0,
                                             (chart3->myIndex) * chart3->chartHeight,
                                             width(),
                                             chart3->chartHeight);

    chart1->SetChartTopAndBtn();
    chart2->SetChartTopAndBtn();
    chart3->SetChartTopAndBtn();

    iList << chart1 << chart2 << chart3;
}

void ChartWidget::updateData()
{
    startIndex++;
    chart1->dataVector.push_back(qrand() % 100);
    chart2->dataVector.push_back(qrand() % 100);
    chart3->dataVector.push_back(qrand() % 100);
//    if(isUpdate){
//    update();
//    }
    update();
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
    p->fillRect(c->myRect, QBrush(c->backgroundColor));
    QPen pen; //创建一个画笔
    pen.setColor(c->lineColor);
    pen.setWidth(1);
    p->setPen(pen);
    float secondData;
    for (int i = startIndex; i < c->dataVector.size(); i++) {
        if (i == startIndex) {
            c->tempData = QPointF(c->timespace , c->dataVector[i] * (c->stepYnumber) + heightSpacing + c->chartTop );
        } else {
            secondData = c->dataVector[i];
            QPointF one = c->tempData;

            QPointF two = QPointF(c->timespace + c->stepXNumber * (i - startIndex), heightSpacing + secondData * (c->stepYnumber) + c->chartTop);

            if(isUpdate){
            p->drawLine(one, two);
            }
            c->tempData = two;
        }
    }
}
void ChartWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, 1);

    for (int i = 0; i < iList.size(); i++) {
        PaintChart(&painter, iList[i]);
    }
}

void ChartWidget::resizeEvent(QResizeEvent *event)
{
    float panelwidth = width() - 2 * chart1->timespace;
    for (int i = 0; i < iList.size(); i++) {
        iList[i]->stepXNumber = (float) panelwidth / iList[i]->spliteNumber;
        iList[i]->myRect = iList[i]->fixRect = QRect(0,
                                                     iList[i]->fixRect.y(),
                                                     width(),
                                                     iList[i]->fixRect.height());
        iList[i]->SetChartTopAndBtn();
    }
}

void ChartWidget::mousePressEvent(QMouseEvent *e)
{
    Chart *temp;
    int tempi = -1;
    for (int i = 0; i < iList.size(); i++) {
        if (iList[i]->myRect.contains(e->pos())) {
            iList[i]->isSelect = true;
            tempi = i;
            temp = iList[i];
            iList[i]->lastPosY = e->pos().y();
            iList[i]->SetCenterPos();
        }
    }
    if (tempi != -1) {
        iList.removeAt(tempi);
        iList << temp;
    }
}

void ChartWidget::mouseMoveEvent(QMouseEvent *e)
{
    for (int i = 0; i < iList.size(); i++) {
        if (iList[i]->isSelect == true) {
            int nowY = e->pos().y();
            int chaju = nowY - iList[i]->lastPosY;
            iList[i]->lastPosY = nowY;
            iList[i]->myRect.moveTo(0, iList[i]->myRect.y() + chaju);
            iList[i]->SetChartTopAndBtn();
            iList[i]->SetCenterPos();
            for (int j = 0; j < iList.size(); j++) {
                if (i == j) {
                    continue;
                } else {
                    if (iList[j]->myRect.contains(iList[i]->centerPos)) {
                        QRect tempi = iList[i]->fixRect;
                        QRect tempj = iList[j]->fixRect;

                        iList[j]->fixRect = iList[j]->myRect = tempi;

                        iList[i]->fixRect = tempj;
                        iList[i]->SetChartTopAndBtn();
                        iList[j]->SetChartTopAndBtn();
                    }
                }
            }
            update();
        }
    }
}

void ChartWidget::mouseReleaseEvent(QMouseEvent *e)
{
    for (int i = 0; i < iList.size(); i++) {
        iList[i]->lastPosY = 0;
        iList[i]->isSelect = false;
        iList[i]->centerPos = QPoint(-1, -1);
        if (iList[i]->myRect != iList[i]->fixRect) {
            iList[i]->myRect = iList[i]->fixRect;
        }
        iList[i]->SetChartTopAndBtn();
    }
}

//TODO：将painter的数据点与timeline的时间点结合起来，做到读取timelien的时间值，然后找到这段时间值之内的点，painter出来

