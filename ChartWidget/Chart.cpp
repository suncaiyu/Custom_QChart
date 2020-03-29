#include "Chart.h"
#include "ui_Chart.h"
#include <QWheelEvent>

Chart::Chart(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::Chart)
{
    ui->setupUi(this);
    animation = new QPropertyAnimation(this,"rc");
    setMouseTracking(true);
}

Chart::~Chart()
{
    delete ui;
}

void Chart::CreateInitNumber()
{
}

void Chart::SetChartTopAndBtn()
{
    chartTop = myRect.y() + chartTopandBottomSpace;
    chartBottom = myRect.y() + chartHeight - chartTopandBottomSpace;
}

void Chart::SetCenterPos()
{
    centerPos = QPoint(myRect.width() /2,myRect.height() /2 + myRect.y());
}

QRect Chart::GetRect()
{
    return myRect;
}

void Chart::SetRect(QRect rc)
{
    myRect = rc;
    SetChartTopAndBtn();
}

void Chart::InitAnimation()
{
    if (animation->state() == QPropertyAnimation::Running) {
        animation->stop();
    }
}

void Chart::StartAnimation(int duration, QEasingCurve type)
{
    if (animation->state() == QPropertyAnimation::Running) {
        animation->stop();
    }
    animation->setStartValue(myRect);
    animation->setEndValue(fixRect);
    animation->setEasingCurve(type);
    animation->setDuration(duration);
    animation->start();
}

void Chart::CalculateStepY()
{
    stepYnumber = ((float) chartHeight - (2 * chartTopandBottomSpace)) / ((float) chartscaleNumber);
    fixRect.setHeight(chartHeight);
    myRect.setHeight(chartHeight);
}
