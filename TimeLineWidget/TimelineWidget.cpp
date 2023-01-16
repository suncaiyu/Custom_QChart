#include "TimelineWidget.h"
#include <QPainter>
#include <QWheelEvent>
#include <QPainterPath>

namespace {
    constexpr int AMPLIFICATION_FACTOR = 15;
    constexpr int WINDOW_HEIGHT = 20;
    constexpr int FONT_SIZE = 7;
    constexpr int BASE_LINE = 0;
    constexpr float TEXT_SPACING = 1.5;
    constexpr int TICK_HEIGHT = 5;
    constexpr float ZOOM_RATIO = 1.22;
    constexpr double MIN_ZOOM = 1.0e-10;
    constexpr double MAX_ZOOM = 500.0;
}

TimelineWidget::TimelineWidget(QWidget *parent)
    : QWidget{parent}, mMinStep(0), mPressedPoint(0), mContext(this)
{
    setMouseTracking(true);
    setFixedHeight(WINDOW_HEIGHT);
    mContext.AddUpdateWidget(this);
}

TimelineWidget::~TimelineWidget()
{

}

TimelineContext &TimelineWidget::GetTimelineContext()
{
    return mContext;
}

void TimelineWidget::ZoomOut()
{
    //suo xiao
    double mousePos = mapFromGlobal(QCursor::pos()).x();
    double mouseOnTimePoint = (mousePos - mContext.mDistance - mContext.mControlBarWidth)
        / mContext.mZoom;
    mContext.mZoom = (mContext.mZoom - 0.01) < MIN_ZOOM ? MIN_ZOOM : mContext.mZoom -0.01;
    mContext.mDistance = -(mouseOnTimePoint * mContext.mZoom - mousePos + mContext.mControlBarWidth);
    Layout();
}

void TimelineWidget::ZoomIn()
{
    double mousePos = mapFromGlobal(QCursor::pos()).x();
    double mouseOnTimePoint = (mousePos - mContext.mDistance - mContext.mControlBarWidth)
        / mContext.mZoom;
    mContext.mZoom = (mContext.mZoom + 0.01)  < MIN_ZOOM ? MIN_ZOOM : mContext.mZoom + 0.01;
    mContext.mDistance = -(mouseOnTimePoint * mContext.mZoom - mousePos + mContext.mControlBarWidth);
    Layout();
}

void TimelineWidget::paintEvent(QPaintEvent *painter)
{
    QPainter p(this);
    DrawBackGround(p);
    QFont font = p.font();
    //font.setFamily(PaintParameters::FONT_NAME);
    font.setPointSize(FONT_SIZE);
    p.setFont(font);
    QFontMetrics fm = p.fontMetrics();
    QPen pen = p.pen();
    long long startTick = mContext.mLeftTime - (mContext.mLeftTime % mMinStep) - mMinStep;
    p.save();
    pen.setColor(mContext.mColor.GetTimelineTextColor());
    p.setPen(pen);
    DrawTickAndLabel(p, startTick, mUnitTickPixel, fm);
    p.restore();
    DrawMouseMeasureLine(p);
    DrawControlBar(p);
    p.end();
}

void TimelineWidget::wheelEvent(QWheelEvent *e)
{
    int delta = e->angleDelta().y();
    double mousePos = e->position().x();
    double mouseOnTimePoint = (mousePos - mContext.mDistance - mContext.mControlBarWidth)
        / mContext.mZoom;
    if (delta > 0) {
        mContext.mZoom = mContext.mZoom * ZOOM_RATIO > MAX_ZOOM ? MAX_ZOOM : mContext.mZoom * ZOOM_RATIO;
    } else {
        mContext.mZoom = mContext.mZoom / ZOOM_RATIO < MIN_ZOOM ? MIN_ZOOM : mContext.mZoom / ZOOM_RATIO;
    }
    mContext.mDistance = -(mouseOnTimePoint * mContext.mZoom - mousePos + mContext.mControlBarWidth);
    Layout();
}

bool TimelineWidget::CanChangeControlWidth()
{
    if (mContext.mMouseOnX > mContext.mControlBarWidth - 5 && mContext.mMouseOnX <= mContext.mControlBarWidth) {
        return true;
    } else {
        return false;
    }
}

void TimelineWidget::DrawMouseMeasureLine(QPainter &p)
{
    if (!mContext.mShowTimelineLabel) {
        return;
    }
    QFontMetrics fm = p.fontMetrics();
    long long mouseOnTime = (long long)(((double)(mContext.mMouseOnX)
        - mContext.mDistance - mContext.mControlBarWidth)
        / mContext.mZoom);
    long long lineLeft = mouseOnTime * mContext.mZoom + mContext.mDistance + mContext.mControlBarWidth;
    long long lineRight = (mouseOnTime + 1) * mContext.mZoom
        + mContext.mDistance + mContext.mControlBarWidth;
    int showX = 0;
    QString showTime;
    if (mContext.mMouseOnX - lineLeft > lineRight - mContext.mMouseOnX) {
        showX = lineRight;
        showTime = QString::number(mouseOnTime + 1) + "ns";
    } else {
        showX = lineLeft;
        showTime = QString::number(mouseOnTime) + "ns";
    }
    int textWidth = fm.horizontalAdvance(showTime) + 10;
    if (showX < rect().width() && showX > rect().x()) {
        QRect rc(showX - textWidth / 2, 0, textWidth, rect().height());
        QPainterPath path;
        path.addRoundedRect(rc, rect().height() / static_cast<qreal>(2), rect().height() / static_cast<qreal>(2));
        p.save();
        QPen pen = p.pen();
        pen.setColor(Qt::white);
        p.setPen(pen);
        p.setRenderHint(QPainter::Antialiasing);
        p.fillPath(path, mContext.mColor.GetLabelBackGround());
        pen.setColor(mContext.mColor.GetLabelTextColor());
        p.setPen(pen);
        p.drawText(rc, Qt::AlignCenter, showTime);
        p.restore();
    }
}

void TimelineWidget::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);
    if (e->pos().x() > mContext.mControlBarWidth) {
        mIsPressed = true;
    }
    if (CanChangeControlWidth()) {
        mCanChangeControlWidth = true;
    }
    mPressedPoint = e->pos().x();
}

void TimelineWidget::mouseMoveEvent(QMouseEvent *e)
{
    QWidget::mouseMoveEvent(e);
    mContext.mShowTimelineLabel = false;
    mContext.mMouseOnX = e->pos().x();
    if (CanChangeControlWidth()) {
        this->setCursor(Qt::SplitHCursor);
    } else {
        this->setCursor(Qt::ArrowCursor);
    }
    if (mIsPressed) {
        double distance = e->pos().x() - mPressedPoint;
        mPressedPoint = e->pos().x();
        // 暂定可向左拖动1000像素
        if (mContext.mDistance + distance < 1000) {
            mContext.mDistance = mContext.mDistance + distance;
        } else {
            mContext.mDistance = 1000;
        }
    }
    if (mCanChangeControlWidth) {
        mContext.mControlBarWidth = e->x();
    }
    Layout();
}

void TimelineWidget::mouseReleaseEvent(QMouseEvent *e)
{
    QWidget::mouseReleaseEvent(e);
    mIsPressed = false;
    mCanChangeControlWidth = false;
}

void TimelineWidget::resizeEvent(QResizeEvent *e)
{
    Layout();
    QWidget::resizeEvent(e);
}

void TimelineWidget::Layout()
{
    if (qFuzzyIsNull(mContext.mZoom)) {
        return;
    }
    long long leftTime = (long long)(-mContext.mDistance / mContext.mZoom);
    long long rightTime = (long long)((double)(rect().width() - mContext.mControlBarWidth - mContext.mDistance) /
            mContext.mZoom);
    mContext.mLeftTime = leftTime;
    mContext.mRightTime = rightTime;
    long long tickOnWindow = rightTime - leftTime/* + 1*/;
    long long mPerPixel2Ns = tickOnWindow / rect().width();
    mMinStep = 1;
    while (mPerPixel2Ns > mMinStep || mMinStep * mContext.mZoom < 5) {
        mMinStep *= 10;
    }
    mUnitTickPixel = mMinStep * mContext.mZoom;
}

void TimelineWidget::DrawBackGround(QPainter &p)
{
    p.fillRect(rect(), BackGroundBrush());
    p.save();
    QPen pen = p.pen();
    pen.setColor(mContext.mColor.GetBorderColor());
    p.setPen(pen);
    p.drawRect(rect().adjusted(0, 0, -1, -1));
    p.restore();
}

void TimelineWidget::DrawTickAndLabel(QPainter &p, long long startTick, int unitTickPixel, QFontMetrics &fm)
{
    QString maxText = QString::number(mContext.mRightTime);
    int maxTextWidth = fm.horizontalAdvance(maxText);
    for (long long drawTime = startTick; drawTime < mContext.mRightTime + mMinStep; drawTime += mMinStep) {
        double onScreenX = double(drawTime) * mContext.mZoom
            + mContext.mDistance + mContext.mControlBarWidth;
        if (onScreenX < mContext.mControlBarWidth) {
            continue;
        }
        p.drawLine(onScreenX, BASE_LINE, onScreenX, BASE_LINE + TICK_HEIGHT);

        if (drawTime % (mMinStep * 100) == 0) {
            QString show = TransTime2String(drawTime);
            int textWidth = fm.horizontalAdvance(show);
            if (unitTickPixel * 100 > maxTextWidth * TEXT_SPACING) {
                p.drawText(onScreenX - textWidth / 2, BASE_LINE + TICK_HEIGHT + fm.height(), show);
            }
            continue;
        }

        if (drawTime % (mMinStep * 10) == 0) {
            QString show = TransTime2String(drawTime);
            int textWidth = fm.horizontalAdvance(show);
            if (unitTickPixel * 10 > maxTextWidth * TEXT_SPACING) {
                p.drawText(onScreenX - textWidth / 2, BASE_LINE + TICK_HEIGHT + fm.height(), show);
            } else {
                show = TransTime2String(drawTime % (mMinStep * 100));
                textWidth = fm.horizontalAdvance(show);
                if (unitTickPixel * 10 > textWidth * TEXT_SPACING) {
                    p.drawText(onScreenX - textWidth / 2, BASE_LINE + TICK_HEIGHT + fm.height(), show);
                }
            }
            continue;
        }

        if (drawTime % (mMinStep * 5) == 0) {
            QString show = TransTime2String(drawTime);
            int textWidth = fm.horizontalAdvance(show);
            if (unitTickPixel * 5 > maxTextWidth * TEXT_SPACING) {
                p.drawText(onScreenX - textWidth / 2, BASE_LINE + TICK_HEIGHT + fm.height(), show);
            }
            continue;
        }

        QString show = TransTime2String(drawTime);
        int textWidth = fm.horizontalAdvance(show);
        if (unitTickPixel > maxTextWidth * TEXT_SPACING) {
            p.drawText(onScreenX - textWidth / 2, BASE_LINE + TICK_HEIGHT + fm.height(), show);
        }
    }
}

QString TimelineWidget::TransTime2String(long long nsVal)
{
    //if (nsVal < 0 || natureShow == 0) {
    //    return QString::number(nsVal);
    //}
    int zero = 0;
    long long tmpValue = nsVal;
    while (tmpValue > 0) {
        if (tmpValue % 10 == 0) {
            zero++;
            tmpValue /= 10;
        } else {
            break;
        }
    }
    QString resultVal;
    if (zero >= 9) {
        resultVal = QString::number(nsVal / 1000000000) + "s";
    } else if (zero >= 6) {
        resultVal = QString::number(nsVal / 1000000) + "ms";
    } else if (zero >= 3) {
        resultVal = QString::number(nsVal / 1000) + "us";
    } else {
        resultVal = QString::number(nsVal) + "ns";
    }
    return resultVal;
}

void TimelineWidget::DrawControlBar(QPainter &p)
{
    QRect controlBarRect = QRect(0, 0, mContext.mControlBarWidth, height());
    p.fillRect(controlBarRect, BackGroundBrush());
    p.save();
    QPen pen = p.pen();
    pen.setColor(mContext.mColor.GetBorderColor());
    p.setPen(pen);
    p.drawRect(controlBarRect.adjusted(0, 0, 0, -1));
    p.restore();
}

QBrush TimelineWidget::BackGroundBrush()
{
    QLinearGradient linearGradient;
    linearGradient.setStart(0, 0);
    linearGradient.setFinalStop(0, rect().height());
    linearGradient.setColorAt(0.0, mContext.mColor.GetTimelineBackGroundStart());
    linearGradient.setColorAt(1.0, mContext.mColor.GetTimelineBackGroundEnd());
    return linearGradient;
}
