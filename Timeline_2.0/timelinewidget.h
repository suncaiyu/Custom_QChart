#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QQuickPaintedItem>
#include <QFontMetrics>
#include "TimelineContext.h"

class TimelineWidget : public QQuickPaintedItem
{
    Q_OBJECT
public:
    explicit TimelineWidget(QQuickItem *parent = nullptr);
    virtual ~TimelineWidget();

protected:
    void paint(QPainter *painter) override;
    void wheelEvent(QWheelEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void hoverMoveEvent(QHoverEvent *e) override;

private:
    void Layout();
    QString TransTimeToNatureString(long long nsVal);
    void PaintMouseMeasureLine(QPainter *p);

    TimelineContext mContext;
    std::unique_ptr<QTimer> mTimer;
    long long mLeftTime; // 屏幕最左侧时间，一ns为单位
    long long mRightTime; // 同理
//    long long mPerPixel2Ns;
    int unitTickPixel = 1;
    long long mMinStep;
    bool mIsPressed = false;
    int mPressedPoint;

signals:

private slots:
    void WidthChangedSlot();
};

#endif // TIMELINEWIDGET_H
