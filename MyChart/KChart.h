#ifndef KCHART_H
#define KCHART_H

#include <QWidget>
#include "ChartData.h"

namespace Ui {
class KChart;
}
class TimeRuler;
class KChart : public QWidget
{
    Q_OBJECT

public:
    explicit KChart(QWidget *parent = nullptr);
    ~KChart();
    void SetTimeLine(TimeRuler *t);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *e) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::KChart *ui;
    TimeRuler *timeLine = nullptr;
    QVector<ChartData *> charts;
    void CalculatorChart();
    QVector<ChartData *> SortCharts();
    void CalculatorTitleVaule();
    double startPressY = -100;
    double disappearArea = 0; // 不可见chart区域 负值
    double allChartHeight = 0; // chart的总高度
    double chartLabelAreaWidth = 120;
    QColor chartLabelAreaColor;
    QColor chartAreaColor;
    double iconSpacing = 15; // 每一级的间距
    void PaintData(QPainter &p, ChartData *cd);

    void PaintLineData(QPainter &p, ChartData *cd);
    void PaintLineData_2(QPainter &p, ChartData *cd);
    void PaintBarData(QPainter &p, ChartData *cd);
    void PaintBarData_2(QPainter &p, ChartData *cd);
};

#endif // KCHART_H
