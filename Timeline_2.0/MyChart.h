#ifndef MYCHART_H
#define MYCHART_H

#include <QQuickPaintedItem>
#include "TimelineContext.h"
#include "ChartData.h"

class MyChart : public QQuickPaintedItem
{
    Q_OBJECT
public:
    MyChart(QQuickItem *parent = nullptr);
    virtual ~MyChart();
    Q_INVOKABLE void initTimelineContext(TimelineContext *ctx) {
        mContext = ctx;
        mContext->AddUpdateWidget(this);
    }
    Q_INVOKABLE void updatePanValue(qreal position);

    void InitData();

protected:
    void paint(QPainter *painter) override;

    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e)override;
    void mouseReleaseEvent(QMouseEvent *e)override;
    void hoverMoveEvent(QHoverEvent *e) override;

private:
    ChartData *GetChartData(QPoint pos, ChartData *data, int start, int width);
    void DrawChart(QPainter &p, ChartData *data, int &height);
    void DrawRowBackground(QPainter &p, ChartData *data, QRect rc);
    void DrawControlBar(QPainter &p, ChartData *data, QRect rc);
    void DrawExpandFlag(QPainter &p, ChartData *data, QRect rc);
    void DrawDragingChart(QPainter &p, ChartData *data);
    void DrawFloatingChart(QPainter &p, ChartData *data);

    TimelineContext *mContext = nullptr;
    ChartData *mRoot = nullptr;
    ChartData *mSelectedChart = nullptr;
    int mHeight = 0;
    int mPan = 0;

signals:
    void updateScrollbarValueSignal(int max, int step);
};

#endif // MYCHART_H
