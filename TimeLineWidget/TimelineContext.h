#ifndef TIMELINECONTEXT_H
#define TIMELINECONTEXT_H
#include <QTimer>
#include <memory>
#include <QColor>
#include <QDebug>
#include <QWidget>
namespace PaintParameters {
}

class Color : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QColor TimelineBackGroundStart READ GetTimelineBackGroundStart WRITE SetTimelineBackGroundStart)
    Q_PROPERTY(QColor TimelineBackGroundEnd READ GetTimelineBackGroundEnd WRITE SetTimelineBackGroundEnd)
    Q_PROPERTY(QColor LabelBackGround READ GetLabelBackGround WRITE SetLabelBackGround)
    Q_PROPERTY(QColor LabelTextColor READ GetLabelTextColor WRITE SetLabelTextColor)
    Q_PROPERTY(QColor ChartTextColor READ GetChartTextColor WRITE SetChartTextColor)
    Q_PROPERTY(QColor TimelineTextColor READ GetTimelineTextColor WRITE SetTimelineTextColor)
    Q_PROPERTY(QColor BorderColor READ GetBorderColor WRITE SetBorderColor)
    Q_PROPERTY(QColor ChartBackGround READ GetChartBackGround WRITE SetChartBackGround)
    Q_PROPERTY(QColor ExpandFlagColor READ GetExpandFlagColor WRITE SetExpandFlagColor)
public:
    Color(QWidget *parent) : QWidget(parent) {}
    ~Color() {};
    void SetTimelineBackGroundStart(QColor c) { mTimelineBackGroundStart = c; }
    QColor GetTimelineBackGroundStart() { return mTimelineBackGroundStart; }
    void SetTimelineBackGroundEnd(QColor c) { mTimelineBackGroundEnd = c; }
    QColor GetTimelineBackGroundEnd() { return mTimelineBackGroundEnd; }
    void SetLabelBackGround(QColor c) { mLabelBackGround = c; }
    QColor GetLabelBackGround() { return mLabelBackGround; }
    void SetLabelTextColor(QColor c) { mLabelTextColor = c; }
    QColor GetLabelTextColor() { return mLabelTextColor; }
    void SetBorderColor(QColor c) { mBorderColor = c; }
    QColor GetBorderColor() { return mBorderColor; }
    void SetChartBackGround(QColor c) { mChartBackGround = c; }
    QColor GetChartBackGround() { return mChartBackGround; }
    void SetExpandFlagColor(QColor c) { mExpandFlagColor = c; }
    QColor GetExpandFlagColor() { return mExpandFlagColor; }
    void SetTimelineTextColor(QColor c) { mTimelineTextColor = c; }
    QColor GetTimelineTextColor() { return mTimelineTextColor; }
    void SetChartTextColor(QColor c) { mChartTextColor = c; }
    QColor GetChartTextColor() { return mChartTextColor; }

private:
    QColor mTimelineBackGroundStart;
    QColor mTimelineBackGroundEnd;
    QColor mLabelBackGround;
    QColor mLabelTextColor;
    QColor mChartTextColor;
    QColor mTimelineTextColor;
    QColor mBorderColor;
    QColor mChartBackGround;
    QColor mExpandFlagColor;
};

class TimelineContext : public QWidget
{
    Q_OBJECT

public:
    TimelineContext(QWidget *parent);
    virtual ~TimelineContext(){}
    double mZoom = 6e-02; // 缩放系数
    double mDistance = 0.0; // 超出屏幕的距离
    int mMouseOnX = 0;
    int mControlBarWidth = 100;
    bool mShowTimelineLabel = true;
    long long mLeftTime = 0;
    long long mRightTime = 0;
    Color mColor;

    void AddUpdateWidget(QWidget *widget);
private:
    std::unique_ptr<QTimer> mTimer;
};
#endif // TIMELINECONTEXT_H
