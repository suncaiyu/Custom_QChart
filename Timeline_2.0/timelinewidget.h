#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QQuickPaintedItem>
#include <QFontMetrics>
#include "TimelineContext.h"

class TimelineWidget : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QColor backgroundStart READ GetBackgroundStart WRITE SetBackgroundStart)
    Q_PROPERTY(QColor backgroundEnd READ GetBackgroundEnd WRITE SetBackgroundEnd)
    Q_PROPERTY(QColor borderColor READ GetBorderColor WRITE SetBorderColor)
    Q_PROPERTY(QColor labelTextColor READ GetLabelTextColor WRITE SetLabelTextColor)
    Q_PROPERTY(QColor labelBackgroundColor READ GetLabelBackgroundColor WRITE SetLabelBackgroundColor)

public:
    explicit TimelineWidget(QQuickItem *parent = nullptr);
    virtual ~TimelineWidget();
    Q_INVOKABLE TimelineContext *getTimelineContext() {return &mContext;}
    QColor GetBackgroundStart(){ return mBackgroundStart;}
    void SetBackgroundStart(QColor c) { qDebug() << c;mBackgroundStart =c;}

    QColor GetBackgroundEnd(){ return mBackgroundEnd;}
    void SetBackgroundEnd(QColor c) { mBackgroundEnd =c;}

    QColor GetBorderColor(){ return mBorderColor;}
    void SetBorderColor(QColor c) { mBorderColor =c;}

    QColor GetLabelTextColor(){ return mLabelTextColor;}
    void SetLabelTextColor(QColor c) { mLabelTextColor =c;}

    QColor GetLabelBackgroundColor(){ return mLabelBackgroundColor;}
    void SetLabelBackgroundColor(QColor c) { mLabelBackgroundColor =c;}

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
    void DrawTickAndLabel(QPainter *p);
    void DrawControlBar(QPainter *p);
    QBrush GetBackgroundColor();
    bool CanChangeControlWidth();

    TimelineContext mContext;
    long long mLeftTime; // 屏幕最左侧时间，一ns为单位
    long long mRightTime; // 同理
//    long long mPerPixel2Ns;
    int unitTickPixel = 1;
    long long mMinStep;
    bool mIsPressed = false;
    int mPressedPoint;
    bool mChangeControlWidth = false;

    QColor mBackgroundStart;
    QColor mBackgroundEnd;
    QColor mBorderColor;
    QColor mLabelTextColor;
    QColor mLabelBackgroundColor;
signals:

private slots:
    void WidthChangedSlot();
};

#endif // TIMELINEWIDGET_H
