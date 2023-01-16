#include "ChartData.h"

ChartData::ChartData()
{

}

ChartData::~ChartData()
{
    qDeleteAll(mChildrens);
}

void ChartData::SetDraging(bool f, int dis)
{
    SetChildDraging(f, dis, this);
}

void ChartData::SetChildDraging(bool f, int dis, ChartData *data)
{
    data->mIsDraging = f;
    data->mDistance2Top = dis;
    for (ChartData *child : data->mChildrens) {
        SetChildDraging(f, dis, child);
    }
}

void ChartData::SetFloatingYPosition(qreal y)
{
    SetChildFloatingYPosition(y, this);
}

void ChartData::SetChildFloatingYPosition(qreal &y, ChartData *child)
{
    child->mFloatingYPosition = y;
    if (mIsExpand) {
        y += child->mChartHeight;
    }
    for (ChartData *child : child->mChildrens) {
        SetChildFloatingYPosition(y, child);
    }
}

void ChartData::SetFloating(bool f)
{
    SetChildFloating(f, this);
}

void ChartData::SetChildFloating(bool f, ChartData *child)
{
    child->mIsFloating = f;
    for (ChartData *child : child->mChildrens) {
        SetChildFloating(f, child);
    }
}
