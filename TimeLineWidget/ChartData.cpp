#include "ChartData.h"
#include <qalgorithms.h>

ChartData::ChartData()
{

}

ChartData::~ChartData()
{
    qDeleteAll(mChildrens);
}

void ChartData::SetDraging(bool flag, int distance)
{
    SetChildrenDraging(flag, this, distance);
}

void ChartData::SetFloating(bool flag)
{
    SetChildrenFloating(flag, this);
}

void ChartData::SetFloatingYLocation(int fy)
{
    SetChildrenFloatingYLocation(this, fy);
}

void ChartData::SetChildrenDraging(bool flag, ChartData *data, int distance)
{
    data->mIsDraging = flag;
    data->mDraging2TopDistance = distance;
    for (int i = 0; i < data->mChildrens.size(); i++) {
        SetChildrenDraging(flag, data->mChildrens.at(i), distance);
    }
}

void ChartData::SetChildrenFloatingYLocation(ChartData *data, int &fy)
{
    data->mFloatingY = fy;
    fy += data->mChartHeight;
    for (int i = 0; i < data->mChildrens.size(); i++) {
        if (data->mChildrens.at(i)->mIsExpand) {
            SetChildrenFloatingYLocation(data->mChildrens.at(i), fy);
        }
    }
}

void ChartData::SetChildrenFloating(bool flag, ChartData *data)
{
    data->mIsFloating = flag;
    for (int i = 0; i < data->mChildrens.size(); i++) {
        SetChildrenFloating(flag, data->mChildrens.at(i));
    }
}
