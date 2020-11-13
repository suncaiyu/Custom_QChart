#include "ChartData.h"

ChartData::ChartData(ChartData *p)
{
    parent = p;
    if (parent != nullptr) {
        parent->children.push_back(this);
        this->level = parent->level + 1;
    }
}
ChartData::~ChartData()
{
    for (ChartData *cd : children) {
        if (cd != nullptr) {
            delete cd;
            cd = nullptr;
        }
    }
    children.clear();
}
void ChartData::SetChildrenHideOrShow()
{
    for (ChartData *cd : children) {
        cd->isShow = !cd->isShow;
    }
}
