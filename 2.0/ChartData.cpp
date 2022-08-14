#include "ChartData.h"

ChartData::ChartData()
{

}

ChartData::ChartData(ChartData *d)
{
    mIsTitle = d->mIsTitle;
    mIsDraging = d->mIsDraging;
    mIsExpand = d->mIsExpand;
    mName = d->mName;
    mTop = d->mTop;
    mChartHeight = d->mChartHeight;
    mDistanceTopPoint = d->mDistanceTopPoint;
    mFloatingY = d->mFloatingY;
    mIsFloating = d->mIsFloating;
    mDragingPoint = d->mDragingPoint;
    for (int i = 0; i < d->mChildrens.size(); ++i) {
        mChildrens.push_back(d->mChildrens[i]);
        d->mChildrens[i]->parent = this;
    }
    mDatas = d->mDatas;
    parent = d->parent;
    d->mChildrens.clear();
}

ChartData::~ChartData()
{
    qDeleteAll(mChildrens);
}

void ChartData::SetDraging(bool f)
{
    SetDragingSub(f, this);
}

void ChartData::SetFloating(bool f)
{
    SetFloatingSub(f, this);
}

void ChartData::SetPressedPoint(int x)
{
    int h = 0;
    SetPressedPointSub(x, h, this);
}

void ChartData::SetDragingPoint(int x)
{
    int h = 0;
    SetDragingPointSub(x, h, this);
}

ChartData* ChartData::GetChild(int index)
{
    if (index < 0 || index >= mChildrens.size()) 
        return nullptr;
    return mChildrens[index];
}

ChartData* ChartData::SwapChild(int j, int i)
{
    ChartData* tmp = new ChartData(GetChild(j));
    delete mChildrens[j];
    mChildrens[j] = mChildrens[i];
    mChildrens[i] = tmp;
    return tmp;
}

void ChartData::SetFloatingY(int x)
{
    int h = 0;
    SetFloatingYSub(x, this, h);
}

void ChartData::ClearFloatingY()
{
    mFloatingY = 0.0f;
}

void ChartData::SetDragingSub(bool f, ChartData *d)
{
    d->mIsDraging = f;
    if (mIsExpand) {
        for (int i = 0; i < d->mChildrens.size(); ++i) {
            SetDragingSub(f, d->mChildrens[i]);
        }
    }
}

void ChartData::SetFloatingSub(bool f, ChartData *d)
{
    d->mIsFloating = f;
    if (mIsExpand) {
        for (int i = 0; i < d->mChildrens.size(); ++i) {
            SetFloatingSub(f, d->mChildrens[i]);
        }
    }
}

void ChartData::SetPressedPointSub(int x, int &h, ChartData *data)
{
    data->mDistanceTopPoint = x;
    h += data->mChartHeight;
    if (mIsExpand) {
        for (int i = 0; i < data->mChildrens.size(); ++i) {
            SetPressedPointSub(x, h, data->mChildrens[i]);
        }
    }
}

void ChartData::SetDragingPointSub(int x, int &h, ChartData *data)
{
    data->mDragingPoint = x + h;
    h += data->mChartHeight;
    if (mIsExpand) {
        for (int i = 0; i < data->mChildrens.size(); ++i) {
            SetDragingPointSub(x, h, data->mChildrens[i]);
        }
    }
}

void ChartData::SetFloatingYSub(int x, ChartData *data, int &h)
{
    data->mFloatingY = x + h;
    h += data->mChartHeight;
    if (data->mIsExpand) {
        for (int i = 0; i < data->mChildrens.size(); ++i) {
            SetFloatingYSub(x, data->GetChild(i), h);
        }
    }
}

