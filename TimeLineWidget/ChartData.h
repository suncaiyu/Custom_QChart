#ifndef CHARTDATA_H
#define CHARTDATA_H
#include <QRect>
#include <QString>
#include <QVector>
struct TimeStamp {
    long long start;
    long long end;
};
struct ShowedInfo{
    TimeStamp ts;
    QRect rc;
};
class ChartData {
public:
    explicit ChartData();
    virtual ~ChartData();
    QString GetNameHash(QString name);
#ifdef CANSWAP
    void SetDraging(bool flag, int distance = 0);
    bool IsDraging() { return mIsDraging; }
    void SetFloating(bool flag);
    bool IsFloating() { return mIsFloating; }
    void SetFloatingYLocation(int fy);
    int GetFloatingYLocation() { return mFloatingY; }
    int GetDraging2TopDistance() { return mDraging2TopDistance; }
#endif
    bool mIsVisible = true;
    bool mIsTitle = false;
    QRect mRect;
    QString mName;
    int mChartHeight = 0;

    ChartData *parent = nullptr;
    int mLevel = 0;
    int mIndentUnits = 10;
    bool mIsExpand = true;
    int mFlagWidth = 8;
    QRect mExpandRect = QRect();
    QVector<ChartData *> mChildrens;
    QVector<TimeStamp> mDatas;
    QVector<ShowedInfo> mShowedTimeStamp;

private:
#ifdef CANSWAP
    bool mIsDraging = false;
    bool mIsFloating = false;
    int mDraging2TopDistance = 0;
    int mFloatingY = 0;
    void SetChildrenDraging(bool flag, ChartData *child, int distance = 0);
    void SetChildrenFloatingYLocation(ChartData *data, int &fy);
    void SetChildrenFloating(bool flag, ChartData *data);
#endif
};
#endif // !CHARTDATA_H
