#include "MyChart.h"
#include <QPainter>
#include <QPainterPath>
MyChart::MyChart(QQuickItem *parent) :QQuickPaintedItem(parent)
{
    InitData();
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
}

MyChart::~MyChart()
{

}

void MyChart::updatePanValue(qreal position)
{
//    qDebug() << position;

    if (position < 0) {
        mPan = 0;
        return;
    }
    mPan = - position * mHeight;
}

void MyChart::InitData()
{
    mRoot = new ChartData();
    mRoot->mIsTitle = true;
    mRoot->mName = "Root";
    mRoot->mChartHeight = 0;

    ChartData *sub1 = new ChartData();
    sub1->mIsTitle = true;
    sub1->mName = "RootSub1";
    mRoot->mChildrens.push_back(sub1);
    sub1->parent = mRoot;
    sub1->mLevel = mRoot->mLevel + 1;

    ChartData *sub2 = new ChartData();
    sub2->mIsTitle = true;
    sub2->mName = "RootSub2";
    mRoot->mChildrens.push_back(sub2);
    sub2->parent = mRoot;
    sub2->mLevel = mRoot->mLevel + 1;

    ChartData *sub3 = new ChartData();
    sub3->mIsTitle = true;
    sub3->mName = "RootSub3";
    mRoot->mChildrens.push_back(sub3);
    sub3->parent = mRoot;
    sub3->mLevel = mRoot->mLevel + 1;

    for (int i = 0; i < 3; i++) {
        ChartData *d = new ChartData;
        d->mIsTitle = false;
        d->mName = "data" + QString::number(i);
        sub1->mChildrens.push_back(d);
        d->parent = sub1;
        d->mLevel = sub1->mLevel + 1;
    }

    for (int i = 0; i < 3; i++) {
        ChartData *d = new ChartData;
        d->mIsTitle = false;
        d->mName = "data" + QString::number(i);
        sub2->mChildrens.push_back(d);
        d->parent = sub2;
        d->mLevel = sub2->mLevel + 1;
    }

    ChartData *sub3sub = new ChartData;
    sub3sub->mIsTitle = true;
    sub3sub->mName = "Sub3Sub1";
    sub3->mChildrens.push_back(sub3sub);
    sub3sub->parent = sub3;
    sub3sub->mLevel = sub3->mLevel + 1;

    for (int i = 0; i < 3; i++) {
        ChartData *d = new ChartData;
        d->mIsTitle = false;
        d->mName = "data" + QString::number(i);
        sub3sub->mChildrens.push_back(d);
        d->parent = sub3sub;
        d->mLevel = sub3sub->mLevel + 1;
    }
}

void MyChart::paint(QPainter *painter)
{
    if (mContext == nullptr) {
        return;
    }
    painter->fillRect(boundingRect(), Qt::white);
    mHeight = 0;
    DrawChart(*painter, mRoot, mHeight);
    DrawDragingChart(*painter, mSelectedChart);
    emit updateScrollbarValueSignal(mHeight, height());
}

void MyChart::DrawChart(QPainter &p, ChartData *data, int &height)
{
    if (data == nullptr) {
        return;
    }
    QRect rc(0, height + mPan, width(), data->mChartHeight);
    data->mRect = rc;
    height += data->mChartHeight;
    if (!data->mIsDraging && !data->mIsFloating) {
        DrawRowBackground(p, data, data->mRect);
        DrawControlBar(p, data, data->mRect);
        DrawExpandFlag(p, data, data->mRect);
    }
    DrawFloatingChart(p, data);
    if (!data->mIsExpand) {
        return;
    }

    for (ChartData *child : data->mChildrens) {
        DrawChart(p, child, height);
    }
}

void MyChart::DrawRowBackground(QPainter &p, ChartData *data, QRect rc)
{
    p.fillRect(rc, Qt::white);
    p.drawRect(rc);
}

void MyChart::DrawControlBar(QPainter &p, ChartData *data, QRect rc)
{
    QRect controlRect(0, rc.y(), mContext->mControlBarWidth, rc.height());
    p.fillRect(controlRect, Qt::white);
    p.drawRect(controlRect);
    int ident = (data->mLevel + 1)* data->mUnitIdent + data->mExpandFlagWidth;
    p.drawText(controlRect.adjusted(ident, 0, 0, 0), Qt::AlignVCenter, data->mName);
}

void MyChart::DrawExpandFlag(QPainter &p, ChartData *data, QRect rc)
{
    if (data->mChildrens.size() == 0) {
        return;
    }
    QPainterPath path;
    p.save();
    p.setRenderHint(QPainter::Antialiasing);
    int ident = data->mLevel * data->mUnitIdent;
    QRect flagRect(ident, rc.y() + rc.height() / 2 - data->mExpandFlagWidth / 2, data->mExpandFlagWidth, data->mExpandFlagWidth);
    data->mExpandRect = flagRect;
    if (data->mIsExpand) {
        path.moveTo(flagRect.x(), flagRect.y() + flagRect.height() / 4);
        path.lineTo(flagRect.x() + flagRect.width() / 2, flagRect.center().y() + flagRect.height() / 4);
        path.lineTo(flagRect.right(), flagRect.y() + flagRect.height() / 4);
        path.closeSubpath();
    } else {
        path.moveTo(flagRect.center().x() - flagRect.width() / 4, flagRect.y());
        path.lineTo(flagRect.center().x() - flagRect.width() / 4, flagRect.bottom());
        path.lineTo(flagRect.center().x() + flagRect.width() / 4, flagRect.center().y());
        path.closeSubpath();
    }
    p.fillPath(path, Qt::gray);
    p.restore();
}

void MyChart::DrawDragingChart(QPainter &p, ChartData *data)
{
    if (data == nullptr) {
        return;
    }
    QRect rc(data->mRect.x(), data->mFloatingYPosition - data->mDistance2Top, data->mRect.width(), data->mRect.height());
    DrawRowBackground(p, data, rc);
    DrawControlBar(p, data, rc);
    DrawExpandFlag(p, data, rc);
    if (!data->mIsExpand) {
        return;
    }

    for (ChartData *child : data->mChildrens) {
        DrawDragingChart(p, child);
    }
}

void MyChart::DrawFloatingChart(QPainter &p, ChartData *data)
{
    if (data->mIsFloating) {
        qreal top = data->mRect.y();
        qreal ft = data->mFloatingYPosition;
        qreal cy = (top + 4.0 * ft) / 5.0;
        if (qAbs(cy - top) < 2.0) {
            data->SetFloating(false);
        } else {
            if (qFuzzyCompare(data->mLastYPosition, cy)) {
                data->mLastYPosition = -1.0;
                data->SetFloating(false);
            } else {
                data->SetFloatingYPosition(cy);
                data->mLastYPosition = cy;
            }
        }
        QRect rc = QRect (data->mRect.x(), data->mFloatingYPosition - data->mDistance2Top, data->mRect.width(), data->mRect.height());
        DrawRowBackground(p, data, rc);
        DrawControlBar(p, data, rc);
        DrawExpandFlag(p, data, rc);
    }
}

void MyChart::mousePressEvent(QMouseEvent *e)
{
    ChartData *tmp = GetChartData(e->pos(), mRoot, 0, mContext->mControlBarWidth);
    if (tmp == nullptr) {
        return;
    }
    if (e->button() == Qt::LeftButton) {
        if (tmp->mExpandRect.contains(e->pos())) {
            tmp->mIsExpand = !tmp->mIsExpand;
        }
    } else if (e->button() == Qt::RightButton) {
        mSelectedChart = tmp;
        tmp->SetDraging(true, e->y() - tmp->mRect.y());
        tmp->SetFloatingYPosition(e->y());
    }
}

void MyChart::mouseMoveEvent(QMouseEvent *e)
{
    if (mSelectedChart != nullptr) {
        int dis = e->y() - mSelectedChart->mFloatingYPosition;
        mSelectedChart->SetFloatingYPosition(e->y());

        ChartData *parent = mSelectedChart->parent;
        if (parent == nullptr) {
            return;
        }
        int index = parent->mChildrens.indexOf(mSelectedChart);
        if (dis > 0 && mSelectedChart != parent->mChildrens.last()) {
            ChartData *tmp = parent->mChildrens.at(index + 1);
            if (e->y() > tmp->mRect.center().y()) {
                tmp->SetFloating(true);
                tmp->SetFloatingYPosition(tmp->mRect.y());
                parent->mChildrens.swapItemsAt(index, index + 1);
            }
        } else if (dis < 0 && mSelectedChart != parent->mChildrens.first()) {
            ChartData *tmp = parent->mChildrens.at(index - 1);
            if (e->y() < tmp->mRect.center().y()) {
                tmp->SetFloating(true);
                tmp->SetFloatingYPosition(tmp->mRect.y());
                parent->mChildrens.swapItemsAt(index, index - 1);
            }
        }
    }
}

void MyChart::mouseReleaseEvent(QMouseEvent *e)
{
    if (mSelectedChart == nullptr) {
        return;
    }
    mSelectedChart->SetDraging(false);
    mSelectedChart->SetFloating(true);
    mSelectedChart = nullptr;
}

void MyChart::hoverMoveEvent(QHoverEvent *e)
{

}

ChartData *MyChart::GetChartData(QPoint pos, ChartData *data, int start, int end)
{
    QRect rc(start, data->mRect.y(), end, data->mChartHeight);
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
