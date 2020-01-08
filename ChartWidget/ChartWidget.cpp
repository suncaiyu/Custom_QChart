#include "ChartWidget.h"
#include "ui_ChartWidget.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

ChartWidget::ChartWidget(TimeLine *timeline,QWidget *parent)
    : QMainWindow(parent),onlyTimeLine(timeline)
      , ui(new Ui::ChartWidget)
{
    ui->setupUi(this);

    chart1 = new Chart();
    chart2 = new Chart();
    chart3 = new Chart();

    chart1->myIndex = 0;
    chart2->myIndex = 1;
    chart3->myIndex = 2;

    chart1->CreateInitNumber();
    chart2->CreateInitNumber();
    chart3->CreateInitNumber();

    chart1->chartHeight = 150;
    chart2->chartHeight = 150;
    chart3->chartHeight = 150;

//    chart1->chartscaleNumber = 100;
//    chart2->chartscaleNumber = 100;
//    chart3->chartscaleNumber = 100;
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
    //    startIndex++;
    chart1->timeVector.push_back(onlyTimeLine->rightTimeInt);
    chart2->timeVector.push_back(onlyTimeLine->rightTimeInt);
    chart3->timeVector.push_back(onlyTimeLine->rightTimeInt);

    chart1->dataVector.push_back(qrand() % 100);
    chart2->dataVector.push_back(qrand() % 100);
    chart3->dataVector.push_back(qrand() % 100);
    for (int i = 0; i < iList.size(); i++) {
        qDebug() << i << "```````" << iList[i]->fixRect.y();
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
    p->fillRect(c->myRect, QBrush(c->backgroundColor));
    QPen pen; //创建一个画笔
    pen.setColor(c->lineColor);
    pen.setWidth(1);
    p->setPen(pen);
    float secondData;
    qreal index = GetDate(c);
//    qDebug() << index;
    if (static_cast<int>(index) == -2) {
        return;
    }
    if (static_cast<int>(index) == -1) {
        int tempRightInt = onlyTimeLine->showRightTime;
        if (tempRightInt < 0) {
            return;
        }
        int num = tempRightInt - c->timeVector[0];

        for (int i = 0; i <= num; i++) {
            if (i == 0) {
                c->tempData = QPointF(onlyTimeLine->rightPos.x(),
                                      c->dataVector[num - i] * (c->stepYnumber) + heightSpacing
                                          + c->chartTop);
            } else {
                secondData = c->dataVector[num - i];
                QPointF one = c->tempData;
                QPointF two = QPointF(c->timespace
                                          + onlyTimeLine->distanceOfTag
                                                * (onlyTimeLine->GetTagNumber() - i),
                                      heightSpacing + secondData * (c->stepYnumber) + c->chartTop);
                p->drawLine(one, two);
                c->tempData = two;
            }
        }
    } else {
        for (qreal i = index; i <= onlyTimeLine->GetTagNumber() + index && i < c->dataVector.size();
             i++) {
            if (i == index) {
                c->tempData = QPointF(c->timespace,
                                      c->dataVector[i] * (c->stepYnumber) + heightSpacing
                                          + c->chartTop);
            } else {
                secondData = c->dataVector[i];
                QPointF one = c->tempData;
                QPointF two = QPointF(c->timespace + onlyTimeLine->distanceOfTag * (i - index),
                                      heightSpacing + secondData * (c->stepYnumber) + c->chartTop);
                p->drawLine(one, two);
                c->tempData = two;
            }
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
        iList[i]->myRect = iList[i]->fixRect = QRect(0,
                                                     iList[i]->fixRect.y(),
                                                     width(),
                                                     iList[i]->fixRect.height());
        iList[i]->SetChartTopAndBtn();
    }
}

void ChartWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    //for (int i = 0; i < iList.size(); i++) {
    //    if (iList[i]->myRect.contains(event->pos())) {
    //        if (event->button() == Qt::LeftButton) {
    //            iList[i]->chartHeight = iList[i]->chartHeight + 2;
    //            iList[i]->CalculateStepY();
    //            ResetiListRect();
    //        }
    //        if (event->button() == Qt::RightButton) {
    //            iList[i]->chartHeight = iList[i]->chartHeight - 2;
    //            iList[i]->CalculateStepY();
    //            ResetiListRect();
    //        }
    //    }
    //}
}

void ChartWidget::mousePressEvent(QMouseEvent *e)
{
    for (int i = 0; i < iList.size(); i++) {
        if (iList[i]->myRect.contains(e->pos())) {
            iList[i]->InitAnimation();
            iList[i]->isSelect = true;
            dragindex = i;
            dragChart = iList[i];
            iList[i]->lastPosY = e->pos().y();
            iList[i]->SetCenterPos();
        }
    }
    if (dragindex != -1) {
        iList.removeAt(dragindex);
        iList << dragChart;
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
                    if (iList[j]->fixRect.contains(iList[i]->centerPos)) {
                        int indexi = iList[i]->myIndex;
                        int indexj = iList[j]->myIndex;
                        iList[i]->myIndex = indexj;
                        iList[j]->myIndex = indexi;
                        QRect tempi = iList[i]->fixRect;
                        QRect tempj = iList[j]->fixRect;
                        iList[j]->fixRect = tempi;
                        iList[j]->StartAnimation(300,QEasingCurve::InCubic);
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

bool ChartWidget::comQt(const Chart a, const Chart b)
{
    if (a.fixRect.y() > b.fixRect.y()) {
        return false;
    }
    else {
        return true;
    }
}
#include <QtAlgorithms>
void ChartWidget::mouseReleaseEvent(QMouseEvent *e)
{
    for (int i = 0; i < iList.size(); i++) {
        iList[i]->lastPosY = 0;
        iList[i]->isSelect = false;
        iList[i]->centerPos = QPoint(-1, -1);
        if (iList[i]->myRect != iList[i]->fixRect) {
            //iList[i]->myRect = iList[i]->fixRect;
            iList[i]->InitAnimation();
            iList[i]->StartAnimation(600, QEasingCurve::OutCubic);
        }
        iList[i]->SetChartTopAndBtn();
    }
}



void ChartWidget::GuiUpdate()
{
    update();
}

qreal ChartWidget::GetDate(Chart *c)
{
    if (c->dataVector.size() <= onlyTimeLine->GetTagNumber()) {
        c->chartPause = onlyTimeLine->isPause;
        c->tempFlag = -1;
        return -1;
    }

    for (int i = 0; i < c->dataVector.size(); i++) {
        c->chartPause = onlyTimeLine->isPause;
        if (c->chartPause == true) {
            if (c->tempFlag == -1) {
                return -1;
            }
        }
        if (c->timeVector[i] == onlyTimeLine->showLeftTime) {
            return i;
        }
    }
    return -2;
}

void ChartWidget::ResetiListRect()
{
    //int tempHeight = 0;
    //for (int i = 0; i < iList.size(); i++) {
    //    int thisH = iList[i]->fixRect.height();
    //    iList[i]->fixRect.setY(tempHeight);
    //    iList[i]->myRect.setY(tempHeight);
    //    iList[i]->fixRect.setHeight(iList[i]->chartHeight);
    //    iList[i]->myRect.setHeight(iList[i]->chartHeight);
    //    iList[i]->SetChartTopAndBtn();
    //    tempHeight = thisH + tempHeight;
    //}
}

//TODO：chart高度的增减

