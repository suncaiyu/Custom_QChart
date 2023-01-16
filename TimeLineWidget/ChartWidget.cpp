#include "ChartWidget.h"
#include <QMouseEvent>
#include <QPainter>
#include <QWheelEvent>
#include <QPainterPath>
#include <qdebug.h>
#include <qpalette.h>

ChartWidget::ChartWidget(TimelineContext &ctx, QWidget *parent) : QWidget(parent), mContext(&ctx)
{
    ctx.AddUpdateWidget(this);
    setMouseTracking(true);
    InitData();
    mScrollbar = new QScrollBar(this);
    connect(mScrollbar, &QScrollBar::valueChanged, [this](int value) {
        mPan = -value;
    });
}

ChartWidget::~ChartWidget()
{
    if (mRoot != nullptr) {
        delete mRoot;
    }
    if (mScrollbar != nullptr) {
        delete mScrollbar;
    }
    if (mHighLightTimeStamp != nullptr) {
        delete mHighLightTimeStamp;
    }
    if (mSelectMenu != nullptr) {
        qDeleteAll(mSelectMenu->actions());
        delete mSelectMenu;
    }
}

void ChartWidget::InitData()
{
    mRoot = new ChartData();
    mRoot->mIsTitle = true;
    mRoot->mName = "Root";

    ChartData *sub1 = new ChartData();
    sub1->mIsTitle = true;
    sub1->mName = "RootSub1";
    sub1->mChartHeight = 50;
    mRoot->mChildrens.push_back(sub1);
    sub1->mLevel = mRoot->mLevel + 1;
    sub1->parent = mRoot;

    ChartData *sub2 = new ChartData();
    sub2->mIsTitle = true;
    sub2->mName = "RootSub2";
    sub2->mChartHeight = 50;
    mRoot->mChildrens.push_back(sub2);
    sub2->mLevel = mRoot->mLevel + 1;
    sub2->parent = mRoot;

    ChartData *sub3 = new ChartData();
    sub3->mIsTitle = true;
    sub3->mName = "RootSub3";
    sub3->mChartHeight = 50;
    mRoot->mChildrens.push_back(sub3);
    sub3->mLevel = mRoot->mLevel + 1;
    sub3->parent = mRoot;

    for (int i = 0; i < 3; i++) {
        ChartData *d = new ChartData;
        d->mName = "data" + QString::number(i);
        d->mChartHeight = 50;
        sub1->mChildrens.push_back(d);
        d->mLevel = sub1->mLevel + 1;
        d->parent = sub1;
        int start = 0;
        for (int j = 0; j < 1000; j++) {
            start += qrand() % 1000;
            TimeStamp ts;
            ts.start = start;
            start += qrand() % 1000;
            ts.end = start;
            d->mDatas.push_back(ts);
        }
    }

    for (int i = 0; i < 3; i++) {
        ChartData *d = new ChartData;
        d->mName = "data" + QString::number(i);
        d->mChartHeight = 50;
        sub2->mChildrens.push_back(d);
        d->mLevel = sub2->mLevel + 1;
        d->parent = sub2;
        long long start = 0;
        for (int j = 0; j < 1000; j++) {
            start += qrand() % 1000;
            TimeStamp ts;
            ts.start = start;
            start += qrand() % 1000;
            ts.end = start;
            d->mDatas.push_back(ts);
        }
    }

    ChartData *sub3sub = new ChartData;
    sub3sub->mIsTitle = true;
    sub3sub->mName = "Sub3Sub1";
    sub3sub->mChartHeight = 50;
    sub3->mChildrens.push_back(sub3sub);
    sub3sub->mLevel = sub3->mLevel + 1;
    sub3sub->parent = sub3;

    for (int i = 0; i < 3; i++) {
        ChartData *d = new ChartData;
        d->mName = "data" + QString::number(i);
        d->mChartHeight = 50;
        sub3sub->mChildrens.push_back(d);
        d->mLevel = sub3sub->mLevel + 1;
        d->parent = sub3sub;
        long long start = 0;
        for (int j = 0; j < 1000; j++) {
            start += qrand() % 10000;
            TimeStamp ts;
            ts.start = start;
            start += qrand() % 10000;
            ts.end = start;
            d->mDatas.push_back(ts);
        }
    }
}

//void ChartWidget::keyPressEvent(QKeyEvent *e)
//{
//    qDebug() << e->key();
//}

void ChartWidget::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);
    if (e->button() == Qt::LeftButton) {
        ChartData *tmp = GetChartData(e->pos(), mRoot, 0, width());
        if (tmp != nullptr && tmp->mExpandRect.contains(e->pos())) {
            tmp->mIsExpand = !tmp->mIsExpand;
            return;
        }
        if (mHighLightTimeStamp != nullptr) {
            if (mHighLightTimeStamp->rc.contains(e->pos())) {
                delete mHighLightTimeStamp;
                mHighLightTimeStamp = nullptr;
                return;
            }
        }
        if (tmp == nullptr) {
            return;
        }
        for (int i = 0; i < tmp->mShowedTimeStamp.size(); i++) {
            if (tmp->mShowedTimeStamp.at(i).rc.contains(e->pos())) {
                if (mHighLightTimeStamp != nullptr) {
                    delete mHighLightTimeStamp;
                }
                mHighLightTimeStamp = new HighLight;
                mHighLightTimeStamp->ts.end = tmp->mShowedTimeStamp.at(i).ts.end;
                mHighLightTimeStamp->ts.start = tmp->mShowedTimeStamp.at(i).ts.start;
                return;
            }
        }
    } else if (e->button() == Qt::RightButton && e->modifiers() == Qt::NoModifier) {
        ChartData *tmp = GetChartData(e->pos(), mRoot, 0, mContext->mControlBarWidth);
        if (tmp == nullptr) {
            return;
        }
        tmp->SetDraging(true, e->y() - tmp->mRect.y());
        mIsDragingChart = tmp;
        mIsDragingChart->SetFloatingYLocation(e->y());
    }
    else if (e->button() == Qt::RightButton && e->modifiers() == Qt::ControlModifier) {
        ChartData *tmp = GetChartData(e->pos(), mRoot, 0, mContext->mControlBarWidth);
        if (tmp == nullptr) {
            return;
        }
        if (mSelectMenu != nullptr) {
            qDeleteAll(mSelectMenu->actions());
            delete mSelectMenu;
        }
        mSelectMenu = new QMenu;
        for (int i = 0; i < tmp->mChildrens.size(); i++) {
            QAction *ac = new QAction(tmp->mChildrens.at(i)->mName, mSelectMenu);
            connect(ac, &QAction::triggered, this, [tmp, i](bool flag) {
                tmp->mChildrens.at(i)->mIsVisible = flag;
            });
            if (tmp->mChildrens.at(i)->mIsVisible) {
                ac->setCheckable(true);
                ac->setChecked(true);
            }
            else {
                ac->setCheckable(true);
            }
            mSelectMenu->addAction(ac);
        }
        mSelectMenu->exec(QCursor::pos());
    }
}

void ChartWidget::mouseMoveEvent(QMouseEvent *e)
{
    mContext->mShowTimelineLabel = true;
    mContext->mMouseOnX = e->x();
    if (mIsDragingChart != nullptr) {
        int distance = e->y() - mIsDragingChart->GetFloatingYLocation();
        mIsDragingChart->SetFloatingYLocation(e->y());

        ChartData *parent = mIsDragingChart->parent;
        if (parent == nullptr) {
            return;
        }
        int index = parent->mChildrens.indexOf(mIsDragingChart);
        if (distance > 0) {
            // down
            if (mIsDragingChart == parent->mChildrens.last()) {
                return;
            }
            ChartData *tmp = parent->mChildrens.at(index + 1);
            if (mIsDragingChart->GetFloatingYLocation() > tmp->mRect.center().y()) {
                tmp->SetFloating(true);
                tmp->SetFloatingYLocation(tmp->mRect.y());
                parent->mChildrens.swapItemsAt(index, index + 1);
            }
        }
        else if (distance < 0) {
            // up
            if (mIsDragingChart == parent->mChildrens.first()) {
                return;
            }
            ChartData *tmp = parent->mChildrens.at(index - 1);
            if (mIsDragingChart->GetFloatingYLocation() < tmp->mRect.center().y()) {
                tmp->SetFloating(true);
                tmp->SetFloatingYLocation(tmp->mRect.y());
                parent->mChildrens.swapItemsAt(index, index - 1);
            }
        }
    }
}

void ChartWidget::mouseReleaseEvent(QMouseEvent *e)
{
    QWidget::mouseReleaseEvent(e);
    if (mIsDragingChart != nullptr) {
        mIsDragingChart->SetDraging(false);
        mIsDragingChart->SetFloating(true);
        mIsDragingChart = nullptr;
    }
}

void ChartWidget::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.fillRect(rect(), palette().brush(QPalette::Background));
    QFont font = p.font();
    //font.setFamily(PaintParameters::FONT_NAME);
    p.setFont(font);
    QPen pen = p.pen();
    pen.setColor(mContext->mColor.GetBorderColor());
    p.setPen(pen);
    mChartHeight = 0;
    DrawChart(p, mChartHeight, mRoot);
    p.drawRect(rect().adjusted(0, 0, 0, -1));
    DrawDragingChart(p, mIsDragingChart);
    DrawHighLight(p);
    p.end();
    UpdateScrollbar();
}

void ChartWidget::UpdateScrollbar()
{
    if (mChartHeight - height() > 0) {
        mScrollbar->setMaximum(mChartHeight - height());
        mScrollbar->setPageStep(height());
    } else {
        mScrollbar->setValue(0);
        mScrollbar->setMaximum(0);
        mScrollbar->setPageStep(height());
    }
}

void ChartWidget::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
    if (mScrollbar != nullptr) {
        mScrollbar->setGeometry(width() - 15, 0, 15, height());
    }
}

void ChartWidget::wheelEvent(QWheelEvent *e)
{
    QWidget::wheelEvent(e);
    int delta = e->delta();
    if (e->modifiers() == Qt::NoModifier) {
        if (delta < 0) {
            mScrollbar->setValue(mScrollbar->value() + 10);
        }
        else if (delta > 0) {
            mScrollbar->setValue(mScrollbar->value() - 10);
        }
    }
    if (e->modifiers() == Qt::ControlModifier) {
        ChartData *tmp = GetChartData(e->pos(), mRoot, 0, mContext->mControlBarWidth);
        if (tmp == nullptr) {
            return;
        }
        int delta = e->angleDelta().y();
        if (delta < 0) {
            tmp->mChartHeight++;
        }
        if (delta > 0) {
            tmp->mChartHeight--;
        }
    }
}

void ChartWidget::DrawChart(QPainter &p, int &h, ChartData *data)
{
    if (data == nullptr) {
        return;
    }
    if (!data->mIsVisible) {
        data->mRect = QRect();
        return;
    }
    QRect tmpRect;
    tmpRect.setX(0);
    tmpRect.setY(h + mPan);
    tmpRect.setWidth(width());
    tmpRect.setHeight(data->mChartHeight);
    data->mRect = tmpRect;
    h += data->mChartHeight;

    DrawFloatingChart(p, data);

    if ((tmpRect.bottom() > 0 || tmpRect.top() < height()) && data->IsDraging() == false && data->IsFloating() == false) {
        p.fillRect(tmpRect, mContext->mColor.GetChartBackGround());
        p.drawRect(tmpRect);
        DrawData(p, data, tmpRect);
        DrawControlBar(p, data, tmpRect);
        DrawExpandFlag(p, data, tmpRect);
    }
    if (!data->mIsExpand) {
        return;
    }
    for (int i = 0; i < data->mChildrens.size(); ++i) {
        DrawChart(p, h, data->mChildrens.at(i));
    }
}

void ChartWidget::DrawControlBar(QPainter &p, ChartData *data, QRect &rect)
{
    QRect labelRect(0, rect.y(), mContext->mControlBarWidth, data->mChartHeight);
    p.fillRect(labelRect, mContext->mColor.GetChartBackGround());
    p.drawRect(labelRect);
    p.save();
    QPen pen = p.pen();
    pen.setColor(mContext->mColor.GetChartTextColor());
    p.setPen(pen);
    labelRect = QRect((data->mLevel + 1) * data->mIndentUnits + data->mFlagWidth, rect.y(),
        mContext->mControlBarWidth - (data->mLevel + 1) * data->mIndentUnits - data->mFlagWidth, data->mChartHeight);
    p.drawText(labelRect, Qt::AlignVCenter, data->mName);
    p.restore();
}

void ChartWidget::DrawExpandFlag(QPainter &p, ChartData *data, QRect &rect)
{
    if (data->mChildrens.size() == 0) {
        data->mExpandRect = QRect();
        return;
    }
    int ident = data->mLevel * data->mIndentUnits;
    QPainterPath path;
    if (data->mIsExpand) {
        path.moveTo(ident, rect.y() + rect.height() / 2 - data->mFlagWidth / 4);
        path.lineTo(ident + data->mFlagWidth, rect.y() + rect.height() / 2 - data->mFlagWidth / 4);
        path.lineTo(ident + data->mFlagWidth / 2, rect.y() + rect.height() / 2 + data->mFlagWidth / 4);
        path.closeSubpath();
    }
    else {
        path.moveTo(ident + data->mFlagWidth / 2 - data->mFlagWidth / 4, rect.y() + rect.height() / 2 - data->mFlagWidth / 2);
        path.lineTo(ident + data->mFlagWidth / 2 - data->mFlagWidth / 4, rect.y() + rect.height() / 2 + data->mFlagWidth / 2);
        path.lineTo(ident + data->mFlagWidth / 2 + data->mFlagWidth / 4, rect.y() + rect.height() / 2);
        path.closeSubpath();
    }
    data->mExpandRect = QRect(ident, rect.y(), data->mFlagWidth, data->mChartHeight);
    p.fillPath(path, mContext->mColor.GetExpandFlagColor());
}

void ChartWidget::DrawData(QPainter &p, ChartData *data, QRect &rect)
{
    data->mShowedTimeStamp.clear();
    for (int i = 0; i < data->mDatas.size(); i++) {
        if (data->mDatas.at(i).end < mContext->mLeftTime) {
            continue;
        }
        if (data->mDatas.at(i).start > mContext->mRightTime) {
            break;
        }
        long long leftPosition = data->mDatas.at(i).start * mContext->mZoom + mContext->mDistance +
            mContext->mControlBarWidth;
        long long rightPosition = data->mDatas.at(i).end * mContext->mZoom + mContext->mDistance +
            mContext->mControlBarWidth;
        QRect rc(leftPosition, rect.y() + data->mChartHeight / 4, rightPosition - leftPosition,
            data->mChartHeight / 2);
        if (rc.width() == 0) {
            rc.setWidth(1);
        }
        ShowedInfo si;
        si.rc = rc;
        si.ts = data->mDatas.at(i);
        data->mShowedTimeStamp.push_back(si);
        p.fillRect(rc, QColor(176, 255, 176));
    }
}

void ChartWidget::DrawHighLight(QPainter &p)
{
    if (mHighLightTimeStamp == nullptr) {
        return;
    }
    long long leftPosition = mHighLightTimeStamp->ts.start * mContext->mZoom + mContext->mDistance +
        mContext->mControlBarWidth;
    long long rightPosition = mHighLightTimeStamp->ts.end * mContext->mZoom + mContext->mDistance +
        mContext->mControlBarWidth;
    long long time = mHighLightTimeStamp->ts.end - mHighLightTimeStamp->ts.start;
    QFontMetrics fm = p.fontMetrics();
    int textWidth = fm.horizontalAdvance(QString::number(time) + "ns");
    QRect textRect;
    if (mHighLightTimeStamp->ts.start < mContext->mLeftTime) {
        QRect right(rightPosition < mContext->mControlBarWidth ? mContext->mControlBarWidth : rightPosition, 0,
            width() - rightPosition, height());
        p.fillRect(right, QColor(0, 0, 0, 100));
        textRect = QRect(mContext->mControlBarWidth, 0, right.x() - mContext->mControlBarWidth, fm.height());
    } else if (mHighLightTimeStamp->ts.end > mContext->mRightTime) {
        QRect left(mContext->mControlBarWidth, 0, leftPosition - mContext->mControlBarWidth, height());
        p.fillRect(left, QColor(0, 0, 0, 100));
        textRect = QRect(left.x() + left.width(), 0, width() - left.x() - left.width(), fm.height());
    } else {
        QRect left(mContext->mControlBarWidth, 0, leftPosition - mContext->mControlBarWidth, height());
        p.fillRect(left, QColor(0, 0, 0, 100));
        QRect right(rightPosition, 0, width() - rightPosition, height());
        p.fillRect(right, QColor(0, 0, 0, 100));
        textRect = QRect(left.x() + left.width(), 0, right.x() - left.x() - left.width(), fm.height());
    }
    if (textRect.width() > textWidth + 10) {
        textWidth = textWidth + 10;
        p.save();
        p.setRenderHint(QPainter::Antialiasing);
        QRect labelRect(textRect.center().x() - textWidth / 2, 0, textWidth, fm.height());
        QPainterPath path;
        path.addRoundedRect(labelRect, fm.height() / 2, fm.height() / 2);
        p.fillPath(path, mContext->mColor.GetLabelBackGround());
        p.drawText(textRect, Qt::AlignCenter, QString::number(time) + "ns");
        p.restore();
    }
    mHighLightTimeStamp->rc = QRect(rightPosition, 0, 15, 15);
    p.save();
    p.setClipRect(rect().adjusted(mContext->mControlBarWidth, 0, 0, 0));
    p.fillRect(mHighLightTimeStamp->rc, Qt::red);
    QPen pen = p .pen();
    pen.setColor(Qt::white);
    p.setPen(pen);
    p.drawLine(mHighLightTimeStamp->rc.topLeft() + QPoint(2, 2), mHighLightTimeStamp->rc.bottomRight() - QPoint(2, 2));
    p.drawLine(mHighLightTimeStamp->rc.topRight() + QPoint(-2, 2), mHighLightTimeStamp->rc.bottomLeft() - QPoint(-2, 2));
    p.restore();
}

void ChartWidget::DrawDragingChart(QPainter &p, ChartData *data)
{
    if (data == nullptr) {
        return;
    }
    if (!data->mIsVisible) {
        data->mRect = QRect();
        return;
    }
    QRect dragingRect(0, data->GetFloatingYLocation() - data->GetDraging2TopDistance(), data->mRect.width(),
        data->mChartHeight);
    p.fillRect(dragingRect, mContext->mColor.GetChartBackGround());
    p.drawRect(dragingRect);
    DrawData(p, data, dragingRect);
    DrawControlBar(p, data, dragingRect);
    DrawExpandFlag(p, data, dragingRect);
    if (!data->mIsExpand) {
        return;
    }
    for (int i = 0; i < data->mChildrens.size(); i++) {
        DrawDragingChart(p, data->mChildrens.at(i));
    }
}

void ChartWidget::DrawFloatingChart(QPainter &p, ChartData *data)
{
    if (!data->IsFloating()) {
        return;
    }
    qreal ftop = data->GetFloatingYLocation();
    qreal top = data->mRect.top();
    qreal cy = (4.0 * ftop + top) / 5.0;
    if (qAbs(top - ftop) <= 2.0) {
        data->SetFloating(false);
    } else {
        data->SetFloatingYLocation(cy);
    }
    QRect dragingRect(0, data->GetFloatingYLocation() - data->GetDraging2TopDistance(), data->mRect.width(), data->mChartHeight);
    p.fillRect(dragingRect, mContext->mColor.GetChartBackGround());
    p.drawRect(dragingRect);
    DrawData(p, data, dragingRect);
    DrawControlBar(p, data, dragingRect);
    DrawExpandFlag(p, data, dragingRect);
}

ChartData *ChartWidget::GetChartData(QPoint pos, ChartData *data, int start, int width)
{
    QRect rect(start, data->mRect.y(), width, data->mChartHeight);
    if (rect.contains(pos)) {
        return data;
    }
    if (data->mIsExpand) {
        for (int i = 0; i < data->mChildrens.size(); ++i) {
            ChartData *d = GetChartData(pos, data->mChildrens.at(i), start, width);
            if (d) {
                return d;
            }
        }
    }
    return nullptr;
}
