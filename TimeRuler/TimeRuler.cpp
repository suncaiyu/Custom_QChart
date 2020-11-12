#include "TimeRuler.h"
#include "ui_TimeRuler.h"
#include <QWheelEvent>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

TimeRuler::TimeRuler(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::TimeRuler)
{
    ui->setupUi(this);
    pWindowContext = new ChartWindowContext;
    setMouseTracking(true);
    setFixedHeight(50);
}

void TimeRuler::wheelEvent(QWheelEvent *event)
{
    int dic = event->delta();
    double mousePos = event->pos().x();
    double mouseOnTimePoint = (mousePos - pWindowContext->viewWindowB)
                              / pWindowContext->viewWindowA;
    if (dic > 0) {
        pWindowContext->viewWindowA *= 1.22;
    } else {
        pWindowContext->viewWindowA /= 1.22;
    }
    if (pWindowContext->viewWindowA > 500) {
        pWindowContext->viewWindowA = 500;
    }
    if (pWindowContext->viewWindowA < 1.0e-10) {
        pWindowContext->viewWindowA = 1.0e-10;
    }
    pWindowContext->viewWindowB = (mouseOnTimePoint * pWindowContext->viewWindowA - mousePos) * -1;
    update();
}

QString TimeRuler::TransTimeToNatureString(long long nsVal)
{
    if (nsVal < 0 || natureShow == 0) {
        return QString::number(nsVal);
    }
    int zeroRight = 0;
    long long tpv = nsVal;
    while (tpv > 0) {
        if (tpv % 10 == 0) {
            zeroRight++;
            tpv /= 10;
        } else {
            break;
        }
    }
    QString rsVal;
    if (zeroRight >= 9) {
        rsVal = QString::number(nsVal / 1000000000) + "s";
    } else if (zeroRight >= 6) {
        rsVal = QString::number(nsVal / 1000000) + "ms";
    } else if (zeroRight >= 3) {
        rsVal = QString::number(nsVal / 1000) + "us";
    } else {
        rsVal = QString::number(nsVal) + "ns";
    }
    return rsVal;
}

void TimeRuler::paintEvent(QPaintEvent *event)
{
    double left, right, top, bottom;
    left = rect().x();
    right = rect().width();
    top = rect().y();
    bottom = rect().height();
    QPainter p(this);
    QPen pen;
    p.fillRect(rect(), Qt::white);
    pen.setColor(Qt::black);
    p.setPen(pen);
    double baseYLocation = bottom - 20;
    p.drawLine(0, baseYLocation, right, baseYLocation);
    p.drawLine(0, baseYLocation + 2, right, baseYLocation + 2);
    int textHeight = TEXT_HEIGHT;
    QFont font("Calibri", textHeight, QFont::Normal, false);
    p.setFont(font);
    QFontMetrics fm = p.fontMetrics();
    long long leftTime = (long long) ((-pWindowContext->viewWindowB) / pWindowContext->viewWindowA);
    long long rightTime = (long long) ((double(right) - pWindowContext->viewWindowB)
                                       / pWindowContext->viewWindowA);
    long long ticksInWindow = rightTime - leftTime + 1;
    // 计算最小标度在时间点的步长
    long long pre = (double) ((ticksInWindow) / (double) (right - left) * 4.);
    long long minStep = 1;
    while (pre > minStep) {
        minStep *= 10;
    }
    p.setPen(Qt::black);
    // 计算刻度宽度对象
    fm = p.fontMetrics();
    // 每个最小格站的像素点
    int um = (int) (double) (minStep * pWindowContext->viewWindowA);
    // tick值的宽度
    QString show = QString::number(rightTime);
    int tickTextWidth = fm.width(show);
    // 计算从哪个刻度开始画
    long long fromTick = leftTime - (leftTime % minStep) - minStep;
    double zeroPos = -100000;
    // 自然数据模式
    natureShow = 1;
    if (pWindowContext->mouseLocationY < 12) {
        natureShow = 0;
    }
    for (long long drawTime = fromTick; drawTime < rightTime + minStep; drawTime += minStep) {
        double onScreenX = double(drawTime) * pWindowContext->viewWindowA
                           + pWindowContext->viewWindowB;
        if (drawTime == 0 && onScreenX > 0) {
            zeroPos = onScreenX;
        }
        if (drawTime % (minStep * 100) == 0) {
            if (drawTime < 0) {
                pen.setColor(QColor(150, 0, 0, 100));
            } else {
                pen.setColor(QColor(11, 11, 11, 100));
            }
            p.setPen(pen);
            p.drawLine(onScreenX - 1, baseYLocation - 1, onScreenX - 1, baseYLocation + 5);
            p.drawLine(onScreenX + 1, baseYLocation - 1, onScreenX + 1, baseYLocation + 5);
            p.drawLine(onScreenX + 0, baseYLocation - 2, onScreenX - 0, baseYLocation + 5);
            if (drawTime < 0) {
                pen.setColor(QColor(150, 0, 0));
            } else {
                pen.setColor(QColor(100, 100, 100));
            }
            p.setPen(pen);
            p.drawLine(onScreenX, baseYLocation - 1, onScreenX, baseYLocation + 7);
            if (um * 100 > tickTextWidth + 30) {
                show = TransTimeToNatureString(drawTime);
                int textWidth = fm.width(show);
                p.drawText(onScreenX - textWidth / 2, baseYLocation + 16, show);
            }
            continue;
        }
        if (drawTime % (minStep * 10) == 0) {
            if (drawTime < 0) {
                pen.setColor(QColor(150, 0, 0));
            } else {
                pen.setColor(QColor(0, 0, 0));
            }
            p.setPen(pen);
            p.drawLine(onScreenX, baseYLocation - 1, onScreenX, baseYLocation + 5);
            if (um * 10 > tickTextWidth + 35) {
                show = TransTimeToNatureString(drawTime);
                int textwidth = fm.width(show);
                p.drawText(onScreenX - textwidth / 2, baseYLocation + 15, show);
            } else {
                show = TransTimeToNatureString(drawTime % (minStep * 100));
                int textWidth = fm.width(show);
                if (um * 10 > textWidth + 20) {
                    p.drawText(onScreenX - textWidth / 2, baseYLocation + 15, show);
                }
            }
            continue;
        }
        if (drawTime % (minStep * 5) == 0) {
            if (drawTime < 0) {
                pen.setColor(QColor(150, 0, 0));
            } else {
                pen.setColor(QColor(155, 155, 155));
            }
            p.setPen(pen);
            p.drawLine(onScreenX, baseYLocation, onScreenX, baseYLocation + 5);
        }
        if (drawTime < 0) {
            pen.setColor(QColor(150, 0, 0));
        } else {
            pen.setColor(QColor(44, 44, 44));
        }
        p.setPen(pen);
        p.drawLine(onScreenX, baseYLocation + 1, onScreenX, baseYLocation + 3);
        if (um > tickTextWidth + 20) {
            show = TransTimeToNatureString(drawTime);
            int textWidth = fm.width(show);
            p.drawText(onScreenX - textWidth / 2, baseYLocation + 15, show);
        } else {
            show = TransTimeToNatureString(drawTime % (minStep * 100));
            int textWidth = fm.width(show);
            if (um > textWidth + 35) {
                p.drawText(onScreenX - textWidth / 2, baseYLocation + 15, show);
            }
        }
    }
    if (zeroPos > 0) {
        pen.setColor(QColor(150, 0, 0));
        p.setPen(pen);
        p.drawLine(left - 1, baseYLocation, zeroPos - 1, baseYLocation);
        pWindowContext->darkBeforeZero = zeroPos;
    } else {
        pWindowContext->darkBeforeZero = -100;
    }
    PaintMouseMeasureLine(&p);
}

void TimeRuler::mouseMoveEvent(QMouseEvent *event)
{
    pWindowContext->mouseLocationY = event->pos().y();
    pWindowContext->mouseLocationX = event->pos().x();
    if (isPress) {
        distance = event->pos().x() - startPressX;
        startPressX = event->pos().x();
        if (pWindowContext->viewWindowB + distance < 0) {
            pWindowContext->viewWindowB = pWindowContext->viewWindowB + distance;
        } else {
            pWindowContext->viewWindowB = 0;
        }
    }
    update();
}
TimeRuler::~TimeRuler()
{
    delete ui;
}

void TimeRuler::mousePressEvent(QMouseEvent *e) 
{
    isPress = true;
    startPressX = e->pos().x();
}

void TimeRuler::mouseReleaseEvent(QMouseEvent *event) 
{
    isPress = false;
    if (distance > -0.001 && distance < 0.001) {
        qDebug() << QStringLiteral("单击");
    }
    else {
        qDebug() << QStringLiteral("拖拽");
    }
    startPressX = -100;
}

void TimeRuler::PaintMouseMeasureLine(QPainter *p) 
{
    double left, right, top, bottom;
    left = rect().x();
    right = width();
    top = rect().y();
    bottom = rect().height();
    double baseYLocation = bottom - 20;
    QPen pen;
    int textHeight = TEXT_HEIGHT;
    QFont font("Calibri", textHeight, QFont::Normal, false);
    p->setFont(font);
    QFontMetrics fm = p->fontMetrics();
    if (pWindowContext->ifShowMouseMeasureLine) {
        long long mouseOnTime = (long long) (((double) (pWindowContext->mouseLocationX)
                                              - pWindowContext->viewWindowB)
                                             / pWindowContext->viewWindowA);
        long long line1X = mouseOnTime * pWindowContext->viewWindowA + pWindowContext->viewWindowB;
        long long line2X = (mouseOnTime + 1) * pWindowContext->viewWindowA
                           + pWindowContext->viewWindowB;
        long long chartHead = 0;
        int showX = 0;
        QString showTime;
        if (pWindowContext->mouseLocationX - line1X > line2X - pWindowContext->mouseLocationX) {
            showX = line2X;
            showTime = QString::number(mouseOnTime + 1) + "ns";
            chartHead = mouseOnTime + 1;
        } else {
            showX = line1X;
            showTime = QString::number(mouseOnTime) + "ns";
            chartHead = mouseOnTime;
        }
        int textWidth = fm.width(showTime);
        if (pWindowContext->pressShift) {
            showX = pWindowContext->mouseLocationX;
        }
        if (showX < right && showX > left) {
            QPainterPath path;
            path.moveTo(showX - textWidth / 2 - 2, baseYLocation - 15);
            path.lineTo(showX - textWidth / 2 - 2, baseYLocation - 26);
            path.lineTo(showX + textWidth / 2 + 2, baseYLocation - 26);
            path.lineTo(showX + textWidth / 2 + 2, baseYLocation - 15);
            p->fillPath(path, QColor(255, 255, 255));
            if (pWindowContext->mouseLocationY > 30) {
                pen.setColor(QColor(0, 0, 0));
                p->setPen(pen);
                p->drawText(showX - textWidth / 2 - 1, baseYLocation - 19, showTime);
            }
            pen.setColor(QColor(205, 205, 0, 100));
            p->setPen(pen);
            p->drawLine(showX, baseYLocation - 17, showX, bottom - 1);
        }
    }
}

QString TimeRuler::TransTimeToNatureString_2(long long nsVal)
{
    if (nsVal < 0 || natureShow == 0) {
        return QString::number(nsVal);
    }
    int zeroRight = 0;
    long long tpv = nsVal;
    while (tpv > 0) {
        if (tpv % 10 == 0) {
            zeroRight++;
            tpv /= 10;
        } else {
            break;
        }
    }
    QString resVal;
    if (zeroRight >= 9) {
        resVal = QString::number(nsVal / 1000000000) + "s";
    } else if (zeroRight >= 6) {
        long long tp = nsVal / 1000000;
        if (tp > 1000) {
            resVal = QString::number((double) tp / 1000., 'f', 2) + "s";
        } else {
            resVal = QString::number(tp) + "ms";
        }
    } else if (zeroRight >= 3) {
        long long tp = nsVal / 1000;
        if (tp > 10000) {
            tp /= 1000;
            if (tp > 1000) {
                resVal = QString::number((double) tp / 1000., 'f', 2) + "s";
            } else {
                resVal = QString::number(tp) + "ms";
            }
        } else {
            resVal = QString::number(tp) + "us";
        }
    } else {
        long long tp = nsVal;
        if (tp > 10000) {
            tp /= 1000;
            if (tp > 10000) {
                tp /= 1000;
                if (tp > 1000) {
                    resVal = QString::number((double) tp / 1000., 'f', 2) + "s";
                } else {
                    resVal = QString::number(tp) + "ms";
                }
            } else {
                resVal = QString::number(tp) + "us";
            }
        } else {
            resVal = QString::number(tp) + "ns";
        }
    }
    return resVal;
}
