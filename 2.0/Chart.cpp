#include "Chart.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

namespace  {
// 左侧面板
constexpr int CONTROL_PANE_WIDTH = 150;
// 左侧空白
constexpr int MARGIN = 20;
constexpr int SPACING = 10;
// 展开标志宽度
constexpr int FLAG_WIDTH = 16;
}
Chart::Chart(QWidget *parent) : QWidget(parent)
{
    InitData();
    mTimeID = startTimer(35);
}

void Chart::InitData()
{
    mRoot = new ChartData();
    mRoot->mIsTitle = true;
    mRoot->mName = "Root";
    mRoot->mChartHeight = 50;

    ChartData *sub1 = new ChartData();
    sub1->mIsTitle = true;
    sub1->mName = "RootSub1";
    sub1->mChartHeight = 50;
    mRoot->mChildrens.push_back(sub1);
    sub1->parent = mRoot;

    ChartData *sub2 = new ChartData();
    sub2->mIsTitle = true;
    sub2->mName = "RootSub2";
    sub2->mChartHeight = 50;
    mRoot->mChildrens.push_back(sub2);
    sub2->parent = mRoot;

    ChartData *sub3 = new ChartData();
    sub3->mIsTitle = true;
    sub3->mName = "RootSub3";
    sub3->mChartHeight = 50;
    mRoot->mChildrens.push_back(sub3);
    sub3->parent = mRoot;

    for (int i = 0; i < 3; i++) {
        ChartData *d = new ChartData;
        d->mIsTitle = false;
        d->mName = "data" + QString::number(i);
        d->mChartHeight = 50;
        sub1->mChildrens.push_back(d);
        d->parent = sub1;
    }

    for (int i = 0; i < 3; i++) {
        ChartData *d = new ChartData;
        d->mIsTitle = false;
        d->mName = "data" + QString::number(i);
        d->mChartHeight = 50;
        sub2->mChildrens.push_back(d);
        d->parent = sub2;
    }

    ChartData *sub3sub = new ChartData;
    sub3sub->mIsTitle = true;
    sub3sub->mName = "Sub3Sub1";
    sub3sub->mChartHeight = 50;
    sub3->mChildrens.push_back(sub3sub);
    sub3sub->parent = sub3;

    for (int i = 0; i < 3; i++) {
        ChartData *d = new ChartData;
        d->mIsTitle = false;
        d->mName = "data" + QString::number(i);
        d->mChartHeight = 50;
        sub3sub->mChildrens.push_back(d);
        d->parent = sub3sub;
    }
}

void Chart::DrawChart(QPainter &p, ChartData *data, int &h)
{
    if (data == nullptr) {
        return;
    }
    if (isDown) {
    data->mTop = h;
    }
    QRect rc;
    rc.setX(0);
    if (data->mFloatingY != 0.0f) {
           qreal top = data->mTop;
           qreal fTop = data->mFloatingY;
           qDebug() << top << "...." << fTop;
           qreal cy = 0.0f;
           cy = (top + fTop) / 2.0f;
           rc.setTop(cy);
           if (qAbs(cy - data->mTop) < 2.0f) {
               data->ClearFloatingY();
           } else {
               data->SetFloatingY(cy);
           }
       }
    else if (data->mIsDraging && mSelected != nullptr) {
        rc.setTop(data->mDragingPoint - data->mDistanceTopPoint + mPan);
    }
    else if (data->mIsDraging && mSelected == nullptr) {
        data->mDragingPoint = (data->mDragingPoint - data->mDistanceTopPoint + data->mTop) / 2.0f + data->mDistanceTopPoint;
        rc.setTop(data->mDragingPoint - data->mDistanceTopPoint + mPan);
        if (qAbs(data->mDragingPoint - data->mTop) < 2.0f) {
            data->mIsDraging = false;
        }
    }
    else {
        rc.setTop(h + mPan);
    }
    if (!isDown) {
    data->mTop = h;
    }
    rc.setWidth(width());
    rc.setHeight(data->mChartHeight);
    data->mRect = rc;
    h += data->mChartHeight;
    // 超过部分不画了
    if (rc.top() > height()) {
        return;
    }
    if (rc.top() + rc.height() > 0/* && !data->mIsDraging*/) {
        DrawBackground(p, rc);
        DrawControlPane(p, rc, data);
        DrawExpandFlag(p, rc, data);
    }
    if (!data->mIsExpand) {
        return;
    }
    for (int i = 0; i < data->mChildrens.size(); i++) {
        DrawChart(p, data->mChildrens[i], h);
    }
}

void Chart::DrawDragingChart(QPainter &p, ChartData *data)
{
    DrawBackground(p, data->mRect);
    DrawControlPane(p, data->mRect, data);
    DrawExpandFlag(p, data->mRect, data);
    if (data->mIsExpand) {
        for (int i = 0; i < data->mChildrens.size(); i++) {
            DrawDragingChart(p, data->mChildrens[i]);
        }
    }
}

void Chart::DrawBackground(QPainter &p, QRect rc)
{
    p.fillRect(rc, Qt::white);
    rc.adjust(0, 0, -p.pen().width(), -p.pen().width());
    p.drawRect(rc);
}

void Chart::DrawControlPane(QPainter &p, QRect rc, ChartData *data)
{
    rc.setWidth(CONTROL_PANE_WIDTH);
    p.fillRect(rc, Qt::white);
    rc.adjust(0, 0, -p.pen().width(), -p.pen().width());
    p.drawRect(rc);
    rc.adjust(MARGIN + FLAG_WIDTH + SPACING, 0, 0, 0);
    p.drawText(rc, Qt::AlignVCenter, data->mName);
}

void Chart::DrawExpandFlag(QPainter &p, QRect rc, ChartData *data)
{
    rc.setX(MARGIN);
    rc.setWidth(FLAG_WIDTH);
    QPainterPath pp;
    if (!data->mIsTitle) {
        return;
    }
    p.save();
    p.setRenderHints(QPainter::Antialiasing);
    if (!data->mIsExpand) {
        pp.moveTo(rc.x(), (rc.y() + rc.height() / 2) - FLAG_WIDTH / 2);
        pp.lineTo(rc.x() + rc.width(), (rc.y() + rc.height() / 2));
        pp.lineTo(rc.x(), (rc.y() + rc.height() / 2) + FLAG_WIDTH / 2);
        pp.closeSubpath();
    } else {
        pp.moveTo(rc.x(), (rc.y() + rc.height() / 2) - FLAG_WIDTH / 2);
        pp.lineTo(rc.x() + rc.width(), (rc.y() + rc.height() / 2) - FLAG_WIDTH / 2);
        pp.lineTo(rc.x() + rc.width() / 2, rc.y() + rc.height() / 2 + FLAG_WIDTH / 2);
    }
    p.fillPath(pp, Qt::gray);
    p.restore();
}

ChartData *Chart::GetChartData(QPoint pos, ChartData *data, int start, int end)
{
    QRect rc(start, data->mTop + mPan, end, data->mChartHeight);
    if (rc.contains(pos)) {
        return data;
    }
    if (data->mIsExpand) {
        for (int i = 0; i < data->mChildrens.size(); i++) {
            ChartData *d = nullptr;
            d = GetChartData(pos, data->mChildrens[i], start, end);
            if (d) {
                return d;
            }
        }
    }
    return nullptr;
}

Chart::~Chart()
{
    killTimer(mTimeID);
    delete mRoot;
}

void Chart::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    int h = 0;
    DrawChart(p, mRoot, h);
    if (mSelected) {
        DrawDragingChart(p, mSelected);
    }
    p.end();
}

void Chart::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // 展开收起
        ChartData *d = GetChartData(event->pos(), mRoot, MARGIN, FLAG_WIDTH);
        if (d == nullptr) {
            return;
        }
        d->mIsExpand = !d->mIsExpand;
    }
    if (event->button() == Qt::RightButton) {
        ChartData *d = GetChartData(event->pos(), mRoot, 0, CONTROL_PANE_WIDTH);
        if (d == nullptr) {
            return;
        }
        d->SetDraging(true);
        d->SetPressedPoint(event->y() + mPan - d->mTop);
        d->SetDragingPoint(event->y() + mPan);
        mSelected = d;
    }
    update();
}

void Chart::mouseReleaseEvent(QMouseEvent *event)
{
    if (mSelected) {
        mSelected->SetFloatingY(event->y() - mPan -(mSelected->mDistanceTopPoint));
        mSelected->SetDraging(false);
        mSelected = nullptr;
    }
    update();
}

void Chart::mouseMoveEvent(QMouseEvent *event)
{
    if (mSelected) {
        ChartData *parent = (mSelected->parent);
        if (parent == nullptr) {
            return;
        }
        int distance = event->y() + mPan - mSelected->mDragingPoint;
        mSelected->SetDragingPoint(event->y() + mPan);
        int index = -1;
        for (int i = 0; i < parent->mChildrens.size(); i++) {
            if (parent->mChildrens[i] == mSelected) {
                index = i;
                break;
            }
        }
        if (index == -1) {
            return;
        }
        if (distance > 0) { // 下移
            isDown = true;
            ChartData* cd = parent->GetChild(index + 1);
            if (cd != nullptr) {
                if (mSelected->mDragingPoint > cd->mTop) {
//                    mSelected = parent->SwapChild(index, index + 1);
//                    parent->GetChild(index)->SetFloatingY(event->y()-mPan);
                    parent->mChildrens.swapItemsAt(index, index + 1);
                    mSelected = parent->GetChild(index + 1);
                    parent->GetChild(index)->SetFloatingY(event->y()-mPan);
                }
            }
        }
        else if (distance < 0) {
            isDown = false;
            ChartData* cd = parent->GetChild(index - 1);
            if (cd != nullptr) {
                if (mSelected->mDragingPoint < cd->mTop + cd->mChartHeight) {
                    //QVector有交换元素的函数，自己白写..
                    parent->mChildrens.swapItemsAt(index, index - 1);
                    mSelected = parent->GetChild(index - 1);
                    parent->GetChild(index)->SetFloatingY(event->y()-mPan);
                }
            }
        }
    }
    update();
}

void Chart::timerEvent(QTimerEvent *e)
{
    update();
}
