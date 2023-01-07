#include "timelinewidget.h"
#include <QPainter>
#include <QTimer>
#include <QPainterPath>
TimelineWidget::TimelineWidget(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    connect(this, &QQuickPaintedItem::widthChanged, this, &TimelineWidget::WidthChangedSlot);
    mTimer = std::make_unique<QTimer>();
    connect(mTimer.get(), &QTimer::timeout, this, [this]() {update(); });
    mTimer->start(50);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::AllButtons);
    setFlag(ItemAcceptsInputMethod, true);
}

TimelineWidget::~TimelineWidget()
{

}
QString TimelineWidget::TransTimeToNatureString(long long nsVal)
{
    //if (nsVal < 0 || natureShow == 0) {
    //    return QString::number(nsVal);
    //}
    int zeroRight = 0;
    long long tpv = nsVal;
    while (tpv > 0) {
        if (tpv % 10 == 0) {
            zeroRight++;
            tpv /= 10;
        }
        else {
            break;
        }
    }
    QString rsVal;
    if (zeroRight >= 9) {
        rsVal = QString::number(nsVal / 1000000000) + "s";
    }
    else if (zeroRight >= 6) {
        rsVal = QString::number(nsVal / 1000000) + "ms";
    }
    else if (zeroRight >= 3) {
        rsVal = QString::number(nsVal / 1000) + "us";
    }
    else {
        rsVal = QString::number(nsVal) + "ns";
    }
    return rsVal;
}

void TimelineWidget::paint(QPainter *painter)
{
    QFont font = painter->font();
    font.setPointSize(8);
    painter->setFont(font);
    QFontMetrics fm = painter->fontMetrics();
    painter->fillRect(boundingRect(), Qt::white);
    int baseLine = 30;
    painter->drawLine(0, baseLine, boundingRect().width(), baseLine);
    QString maxText = QString::number(mRightTime);
    int ticketTextWidth = fm.horizontalAdvance(maxText);
    long long startTick = mLeftTime - (mLeftTime % mMinStep) - mMinStep;
    for (long long drawTime = startTick; drawTime < mRightTime + mMinStep; drawTime += mMinStep) {
        double onScreenX = double(drawTime) * mContext.mZoom + mContext.mDistance;
        painter->drawLine(onScreenX, baseLine, onScreenX, baseLine + 5);

        if (drawTime % (mMinStep * 100) == 0) {
            QString show = TransTimeToNatureString(drawTime);
            int textWidth = fm.horizontalAdvance(show);
            if (unitTickPixel * 100 > textWidth * 1.5) {
                painter->drawText(onScreenX - textWidth / 2, baseLine + 15, show);
            }
            continue;
        }

        if (drawTime % (mMinStep * 50) == 0) {
            QString show = TransTimeToNatureString(drawTime);
            int textWidth = fm.horizontalAdvance(show);
            if (unitTickPixel * 50 > textWidth * 1.5) {
                painter->drawText(onScreenX - textWidth / 2, baseLine + 15, show);
            }
            continue;
        }

        // 每10个ticket做一个标记
        if (drawTime % (mMinStep * 10) == 0) {
            QString show = TransTimeToNatureString(drawTime);
            int textWidth = fm.horizontalAdvance(show);
            if (unitTickPixel * 10 > textWidth * 1.5) {
                painter->drawText(onScreenX - textWidth / 2, baseLine + 15, show);
            }
            continue;
        }

        if (drawTime % (mMinStep * 5) == 0) {
            QString show = TransTimeToNatureString(drawTime);
            int textWidth = fm.horizontalAdvance(show);
            if (unitTickPixel * 5 > textWidth * 1.5) {
                painter->drawText(onScreenX - textWidth / 2, baseLine + 15, show);
            }
            continue;
        }

        QString show = TransTimeToNatureString(drawTime);
        int textWidth = fm.horizontalAdvance(show);
        if (unitTickPixel > textWidth * 1.5) {
            painter->drawText(onScreenX - textWidth / 2, baseLine + 15, show);
        }
    }
    PaintMouseMeasureLine(painter);
}

void TimelineWidget::wheelEvent(QWheelEvent *event)
{
    int dic = event->angleDelta().y();
    double mousePos = event->position().x();
    double mouseOnTimePoint = (mousePos - mContext.mDistance)
        / mContext.mZoom;
    if (dic > 0) {
        mContext.mZoom *= 1.22;
    }
    else {
        mContext.mZoom /= 1.22;
    }
    if (mContext.mZoom > 500) {
        mContext.mZoom = 500;
    }
    if (mContext.mZoom < 1.0e-10) {
        mContext.mZoom = 1.0e-10;
    }
    mContext.mDistance = -(mouseOnTimePoint * mContext.mZoom - mousePos);
    update();
    Layout();
}

void TimelineWidget::mousePressEvent(QMouseEvent *e)
{
    mIsPressed = true;
    mPressedPoint = e->pos().x();
}

void TimelineWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (mIsPressed) {
        mContext.mMouseOnX = e->pos().x();
        double distance = e->pos().x() - mPressedPoint;
        mPressedPoint = e->pos().x();
        // 暂定可向左拖动1000像素
        if (mContext.mDistance + distance < 1000) {
            mContext.mDistance = mContext.mDistance + distance;
        }
        else {
            mContext.mDistance = 1000;
        }
    }
    Layout();
}

void TimelineWidget::mouseReleaseEvent(QMouseEvent *e)
{

    mIsPressed = false;
    QQuickPaintedItem::mouseReleaseEvent(e);
}

void TimelineWidget::hoverMoveEvent(QHoverEvent *e)
{
    QQuickPaintedItem::hoverMoveEvent(e);
    if (!mIsPressed) {
        mContext.mMouseOnX = e->pos().x();
    }
}

void TimelineWidget::Layout()
{
    mLeftTime = (long long)(-mContext.mDistance / mContext.mZoom);
    mRightTime = (long long)((double)(boundingRect().width() - mContext.mDistance) / mContext.mZoom);
    long long tickOnWindow = mRightTime - mLeftTime + 1;
    // 计算一个像素代表多少ns
    long long mPerPixel2Ns = tickOnWindow / boundingRect().width() /** 8*/;
    // 计算最小显示的步长(间隔多少显示一个ticket)
    mMinStep = 1;
    while (mPerPixel2Ns > mMinStep) {
        mMinStep *= 10;
    }
    unitTickPixel = mMinStep * mContext.mZoom;
    // 当单元的tick显示距离太短的时候，控制一下
    while (unitTickPixel < 10) {
        mMinStep = mMinStep * 10;
        unitTickPixel = mMinStep * mContext.mZoom;
    }
    update();
}

void TimelineWidget::WidthChangedSlot()
{
    Layout();
}

void TimelineWidget::PaintMouseMeasureLine(QPainter *p)
{
    int baseYLocation = 20;
    QPen pen;
    int textHeight = 8;
    QFont font("Calibri", textHeight, QFont::Normal, false);
    p->setFont(font);
    QFontMetrics fm = p->fontMetrics();
    long long mouseOnTime = (long long)(((double)(mContext.mMouseOnX)
        - mContext.mDistance)
        / mContext.mZoom);

    //对应的坐标x

    long long lineLeft = mouseOnTime * mContext.mZoom + mContext.mDistance;
    long long lineRight = (mouseOnTime + 1) * mContext.mZoom
        + mContext.mDistance;
    long long chartHead = 0;
    int showX = 0;
    QString showTime;
    if (mContext.mMouseOnX - lineLeft > lineRight - mContext.mMouseOnX) {
        showX = lineRight;
        showTime = QString::number(mouseOnTime + 1) + "ns";
        chartHead = mouseOnTime + 1;
    }
    else {
        showX = lineLeft;
        showTime = QString::number(mouseOnTime) + "ns";
        chartHead = mouseOnTime;
    }
    int textWidth = fm.horizontalAdvance(showTime);
    if (showX < boundingRect().width() && showX > boundingRect().x()) {
        pen.setColor(QColor(0, 0, 0));
        p->setPen(pen);
        p->drawText(showX - textWidth / 2 - 1, baseYLocation - 5, showTime);
        pen.setColor(QColor(205, 205, 0, 100));
        p->setPen(pen);
        p->drawLine(showX, baseYLocation - 17, showX, boundingRect().height() - 1);
    }
}
