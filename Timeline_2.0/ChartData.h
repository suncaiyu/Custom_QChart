#ifndef CHARTDATA_H
#define CHARTDATA_H
#include <QString>
#include <QVector>
#include <QRect>

class ChartData
{
public:
    ChartData();
    ~ChartData();
    bool mIsTitle = true;
    int mChartHeight = 50;
    QRect mRect = QRect();
    QRect mExpandRect = QRect();
    QString mName;
    QVector<ChartData *>mChildrens;
    ChartData *parent = nullptr;
    int mLevel = 0;
    int mUnitIdent = 5;
    int mExpandFlagWidth = 10;
    bool mIsExpand = true;
    bool mIsVisible = true;
    bool mIsDraging = false;
    bool mIsFloating = false;
    int mDistance2Top = 0;
    int mFloatingYPosition = 0;

    void SetDraging(bool f, int dis = 0);
    void SetChildDraging(bool f, int dis, ChartData *data);

    void SetFloatingYPosition(qreal y);
    void SetChildFloatingYPosition(qreal &y, ChartData *child);

    void SetFloating(bool f);
    void SetChildFloating(bool f, ChartData *child);

    qreal mLastYPosition = -1.0;
};

#endif // CHARTDATA_H
