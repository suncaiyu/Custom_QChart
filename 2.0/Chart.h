#ifndef CHART_H
#define CHART_H

#include <QWidget>
#include "ChartData.h"

class Chart : public QWidget
{
    Q_OBJECT
public:
    explicit Chart(QWidget *parent = nullptr);
    ~Chart();

protected:
    void paintEvent(QPaintEvent *e) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void timerEvent(QTimerEvent *e)override;

private:
    void InitData();
    void DrawChart(QPainter &p, ChartData *data, int &h);
    void DrawDragingChart(QPainter &p, ChartData *data);
    void DrawBackground(QPainter &p, QRect rc);
    void DrawControlPane(QPainter &p, QRect rc, ChartData *data);
    void DrawExpandFlag(QPainter &p, QRect rc, ChartData *data);
    ChartData *GetChartData(QPoint pos, ChartData *r, int start, int width);

    ChartData *mRoot = nullptr;
    ChartData *mSelected = nullptr;
    int mPan = 0;
    int mTimeID = 0;
    bool isDown = false;
signals:

};

#endif // CHART_H
