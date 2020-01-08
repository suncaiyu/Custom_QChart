#include "TimeLine.h"
#include "ui_TimeLine.h"
#include <QDateTime>
#include <QTimer>
#include <QDebug>
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>

TimeLine::TimeLine(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::TimeLine)
{
    ui->setupUi(this);
    leftTime = new QDateTime;

    QString strDateTime = leftTime->currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    qDebug() << strDateTime;
    t_timer = new QTimer(this);
    connect(t_timer, SIGNAL(timeout()), this, SLOT(UpdateTime()));
    t_timer->start(1000);

    scale = 2;
    SetDistanceOfTag(scale);
    tagNumber = -1;

    colBg = QColor(0,0,0,155);
    colTimeline = QColor(255,255,255);
    colText = QColor(255,12,12,155);

    isPause = false;
}


void TimeLine::UpdateTime()
{
    leftTime->addSecs(1);
    leftTimeInt = leftTime->currentDateTime().toTime_t();
    rightTimeInt = leftTimeInt + tagNumber;
    leftPos = QPoint(0,timeLineY);
    rightPos = QPoint(tagNumber * distanceOfTag, timeLineY);
    qDebug() << leftTimeInt << "******" << rightTimeInt;
    qDebug() << width() << "//////" << rightPos;
    if (isPause == false) {
        showLeftTime = leftTimeInt;
    }
    update();
}
TimeLine::~TimeLine()
{
    delete ui;
}

void TimeLine::paintEvent(QPaintEvent *event)
{
    QPainter paint(this);
    paint.fillRect(rect(),QBrush(colBg));
    PaintTimelineTag(&paint);
}

void TimeLine::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        if (isPause == false) {
            isPause = true;
        }
    }
    if (e->button() == Qt::RightButton) {
        if (isPause) {
            isPause = false;
        }
    }

    if (e->button() == Qt::LeftButton && isPause) {
        leftPressPos = e->pos();
    }
}

void TimeLine::mouseMoveEvent(QMouseEvent *e)
{
    QPoint nowPos = e->pos();
    int difference = leftPressPos.x() - nowPos.x();
    int sec = difference / distanceOfTag;
    if (showLeftTime + sec >= leftTimeInt) {
        return;
    }
    else {
        showLeftTime = showLeftTime + sec;
    }

    update();
}

void TimeLine::mouseReleaseEvent(QMouseEvent *e)
{
    leftPressPos = QPoint(0, 0);
}

void TimeLine::wheelEvent(QWheelEvent *e)
{
    if (e->delta() > 0) {
        if (scale == 3) {
            return;
        }
        scale++;
        SetDistanceOfTag(scale);
    }
    else {
        if (scale == 1) {
            return;
        }
        scale--;
        SetDistanceOfTag(scale);
    }
}

void TimeLine::PaintTimelineTag(QPainter *p)
{
    int shortTag = 5;
    int longTag = 10;
    QPen pen;
    pen.setColor(colTimeline);
    p->setPen(pen);
    p->drawLine(QPoint(0, timeLineY),QPoint(width(), timeLineY));
    for (int i = 0; i <= tagNumber; i++) {
        int tagX = i * distanceOfTag;
        QPoint p1(tagX, timeLineY);
        QPoint p2(tagX, timeLineY + shortTag);
        QPoint p3(tagX, timeLineY + longTag);
        if ((showLeftTime + i) % 10 == 0) {
            p->drawLine(p1, p3);
            QPen pen1;
            pen1.setColor(colText);
            p->setPen(pen1);
            QFontMetrics fm = p->fontMetrics();
            int textWidth = fm.width(QString::number(showLeftTime + i));
            QRect rectTect(tagX - (textWidth / 2), timeLineY - longTag *2 , textWidth, distanceOfTag / 2);
            p->drawText(rectTect, QString::number(showLeftTime + i));
        }
        p->setPen(pen);
        p->drawLine(p1,p2);
    }
}

int TimeLine::GetTagNumber()
{
    tagNumber = width() / distanceOfTag;
    return tagNumber;
}

void TimeLine::resizeEvent(QResizeEvent *event)
{
    GetTagNumber();
    timeLineY = height() / 2;
}

void TimeLine::SetColBG(QColor c)
{
    colBg = c;
}

void TimeLine::SetColText(QColor c)
{
    colText = c;
}

void TimeLine::SetColTimeline(QColor c)
{
    colTimeline = c;
}

void TimeLine::SetDistanceOfTag(int scale)
{
    if (scale == 1) {
        distanceOfTag = 20;
        GetTagNumber();
    }
    if (scale == 2) {
        distanceOfTag = 30;
        GetTagNumber();
    }
    if (scale == 3) {
        distanceOfTag = 40;
        GetTagNumber();
    }
}
