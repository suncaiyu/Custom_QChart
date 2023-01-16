#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QWidget>
#include <qfontmetrics.h>
#include "TimelineContext.h"

class TimelineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TimelineWidget(QWidget *parent = nullptr);
    virtual ~TimelineWidget();
    TimelineContext &GetTimelineContext();
    void ZoomOut();
    void ZoomIn();

private:
    void paintEvent(QPaintEvent *painter) override;
    void wheelEvent(QWheelEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;

    void Layout();
    void DrawBackGround(QPainter &p);
    void DrawTickAndLabel(QPainter &p, long long start, int unitTickWidth, QFontMetrics &fm);
    void DrawControlBar(QPainter &p);
    void DrawMouseMeasureLine(QPainter &p);
    bool CanChangeControlWidth();
    QBrush BackGroundBrush();
    QString TransTime2String(long long nsVal);

    TimelineContext mContext;
    long long mMinStep;
    bool mIsPressed = false;
    bool mCanChangeControlWidth = false;
    int mPressedPoint;
    int mUnitTickPixel = 0;

signals:

};

#endif // TIMELINEWIDGET_H
