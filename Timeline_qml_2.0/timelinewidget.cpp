#include "timelinewidget.h"
#include <QPainter>
#include <QTimer>
#include <QPainterPath>
#include <QCursor>
namespace  {
constexpr int TICK_HEIGHT = 8;
constexpr float TEXT_SPACING = 1.5f;
}
TimelineWidget::TimelineWidget(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    connect(this, &QQuickPaintedItem::widthChanged, this, &TimelineWidget::WidthChangedSlot);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::AllButtons);
    setFlag(ItemAcceptsInputMethod, true);
    mContext.AddUpdateWidget(this);
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
    painter->fillRect(boundingRect(), GetBackgroundColor());
    QPen pen = painter->pen();
    pen.setColor(mBorderColor);
    painter->setPen(pen);
    painter->drawRect(boundingRect());
    int baseLine = 0;
    painter->drawLine(0, baseLine, boundingRect().width(), baseLine);
    QString maxText = QString::number(mRightTime);
    int ticketTextWidth = fm.horizontalAdvance(maxText);
    DrawTickAndLabel(painter);
    PaintMouseMeasureLine(painter);
    DrawControlBar(painter);
}

void TimelineWidget::wheelEvent(QWheelEvent *event)
{
    int dic = event->angleDelta().y();
    double mousePos = event->position().x();
    double timeOnMousePoint = (mousePos - mContext.mDistance - mContext.mControlBarWidth)
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
    mContext.mDistance = -(timeOnMousePoint * mContext.mZoom - mousePos) - mContext.mControlBarWidth;
    Layout();
}

void TimelineWidget::mousePressEvent(QMouseEvent *e)
{
    mPressedPoint = e->pos().x();
    if (CanChangeControlWidth()) {
        mChangeControlWidth = true;
    }
    if (e->x() > mContext.mControlBarWidth) {
        mIsPressed = true;
    }
}

void TimelineWidget::mouseMoveEvent(QMouseEvent *e)
{
    mContext.mMouseOnX = e->pos().x();
    if (mIsPressed) {
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
    if (mChangeControlWidth) {
        mContext.mControlBarWidth = e->x();
    }
    Layout();
}

void TimelineWidget::mouseReleaseEvent(QMouseEvent *e)
{
    mChangeControlWidth = false;
    mIsPressed = false;
    QQuickPaintedItem::mouseReleaseEvent(e);
}

void TimelineWidget::hoverMoveEvent(QHoverEvent *e)
{
    QQuickPaintedItem::hoverMoveEvent(e);
    if (!mIsPressed) {
        mContext.mMouseOnX = e->pos().x();
    }
    if (CanChangeControlWidth()) {
        setCursor(Qt::SplitHCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
}

void TimelineWidget::Layout()
{
    mLeftTime = (long long)(-mContext.mDistance / mContext.mZoom);
    mRightTime = (long long)((double)(boundingRect().width() - mContext.mDistance - mContext.mControlBarWidth) / mContext.mZoom);
    long long tickOnWindow = mRightTime - mLeftTime + 1;
    // 计算一个像素代表多少ns
    long long mPerPixel2Ns = tickOnWindow / (boundingRect().width() - mContext.mControlBarWidth);
    // 计算最小显示的步长(间隔多少显示一个ticket)
    mMinStep = 1;
    while (mPerPixel2Ns > mMinStep || mMinStep * mContext.mZoom < 10) {
        mMinStep *= 10;
    }
    unitTickPixel = mMinStep * mContext.mZoom;
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
        - mContext.mDistance - mContext.mControlBarWidth)
        / mContext.mZoom);

    //对应的坐标x
    long long lineLeft = mouseOnTime * mContext.mZoom + mContext.mDistance + mContext.mControlBarWidth;
    long long lineRight = (mouseOnTime + 1) * mContext.mZoom
        + mContext.mDistance + mContext.mControlBarWidth;
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
    int textWidth = fm.horizontalAdvance(showTime) + 10;
    if (showX < boundingRect().width() && showX > boundingRect().x()) {
        p->save();
        p->setRenderHint(QPainter::Antialiasing);
        pen.setColor(mLabelTextColor);
        p->setPen(pen);
        QRect label(showX - textWidth / 2, 0, textWidth, height());
        QPainterPath path;
        path.addRoundedRect(label, label.height() / 2, label.height() / 2);
        p->fillPath(path, mLabelBackgroundColor);
        p->drawText(label, Qt::AlignCenter, showTime);
        p->restore();
    }
}

void TimelineWidget::DrawTickAndLabel(QPainter *painter)
{
    QFontMetrics fm = painter->fontMetrics();
    int baseLine = 0;
    long long startTick = mLeftTime - (mLeftTime % mMinStep) - mMinStep;
    for (long long drawTime = startTick; drawTime < mRightTime + mMinStep; drawTime += mMinStep) {
        double onScreenX = double(drawTime) * mContext.mZoom + mContext.mDistance + mContext.mControlBarWidth;
        painter->drawLine(onScreenX, baseLine, onScreenX, baseLine + TICK_HEIGHT);

        if (drawTime % (mMinStep * 100) == 0) {
            QString show = TransTimeToNatureString(drawTime);
            int textWidth = fm.horizontalAdvance(show);
            if (unitTickPixel * 100 > textWidth * TEXT_SPACING) {
                painter->drawText(onScreenX - textWidth / 2, baseLine + TICK_HEIGHT + fm.height(), show);
            }
            continue;
        }

        if (drawTime % (mMinStep * 50) == 0) {
            QString show = TransTimeToNatureString(drawTime);
            int textWidth = fm.horizontalAdvance(show);
            if (unitTickPixel * 50 > textWidth * TEXT_SPACING) {
                painter->drawText(onScreenX - textWidth / 2, baseLine + TICK_HEIGHT + fm.height(), show);
            }
            continue;
        }

        // 每10个ticket做一个标记
        if (drawTime % (mMinStep * 10) == 0) {
            QString show = TransTimeToNatureString(drawTime);
            int textWidth = fm.horizontalAdvance(show);
            if (unitTickPixel * 10 > textWidth * TEXT_SPACING) {
                painter->drawText(onScreenX - textWidth / 2, baseLine + TICK_HEIGHT + fm.height(), show);
            }
            continue;
        }

        if (drawTime % (mMinStep * 5) == 0) {
            QString show = TransTimeToNatureString(drawTime);
            int textWidth = fm.horizontalAdvance(show);
            if (unitTickPixel * 5 > textWidth * TEXT_SPACING) {
                painter->drawText(onScreenX - textWidth / 2, baseLine + TICK_HEIGHT + fm.height(), show);
            }
            continue;
        }

        QString show = TransTimeToNatureString(drawTime);
        int textWidth = fm.horizontalAdvance(show);
        if (unitTickPixel > textWidth * TEXT_SPACING) {
            painter->drawText(onScreenX - textWidth / 2, baseLine + TICK_HEIGHT + fm.height(), show);
        }
    }
}

void TimelineWidget::DrawControlBar(QPainter *painter)
{
    painter->fillRect(QRect(0, 0, mContext.mControlBarWidth, height()), GetBackgroundColor());
    painter->drawRect(QRect(0, 0, mContext.mControlBarWidth, height()));
}

QBrush TimelineWidget::GetBackgroundColor()
{
    QLinearGradient linear;
    linear.setStart(0, 0);
    linear.setFinalStop(0, height());
    linear.setColorAt(0.0, mBackgroundStart);
    linear.setColorAt(1.0, mBackgroundEnd);
    return linear;
}

bool TimelineWidget::CanChangeControlWidth()
{
    if (mContext.mMouseOnX > mContext.mControlBarWidth - 5 && mContext.mMouseOnX < mContext.mControlBarWidth) {
        return true;
    }
    return false;
}
