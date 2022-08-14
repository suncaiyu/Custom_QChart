#ifndef CHARTDATA_H
#define CHARTDATA_H
#include <QObject>
#include <QRect>
class ChartData
{
public:
    struct DataPoint{
        qreal time;
        qreal value;
    };
    ChartData();
    ChartData(ChartData *data);
    ~ChartData();
    void SetDraging(bool f);
    void SetFloating(bool f);
    void SetPressedPoint(int x);
    void SetDragingPoint(int x);
    ChartData* GetChild(int index);
    ChartData* SwapChild(int j, int i);
    void SetFloatingY(int x);
    void ClearFloatingY();
    bool mIsTitle = true;
    QRect mRect;
    QString mName;
    qreal mTop;
    int mChartHeight;
    bool mIsDraging = false;
    bool mIsExpand = true;
    bool mIsFloating = false;
    qreal mDistanceTopPoint;
    qreal mDragingPoint;
    qreal mFloatingY = 0.0f;
    QVector<ChartData *> mChildrens;
    QVector<DataPoint> mDatas; 
    ChartData *parent = nullptr;

private:
    void SetDragingSub(bool f, ChartData *d);
    void SetFloatingSub(bool f, ChartData *d);
    void SetPressedPointSub(int x, int &h, ChartData *data);
    void SetDragingPointSub(int x, int &h, ChartData *data);
    void SetFloatingYSub(int x, ChartData *data, int &h);
};

#endif // CHARTDATA_H
