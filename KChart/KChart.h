#ifndef KCHART_H
#define KCHART_H

#include <QWidget>
#include "ChartData.h"

namespace Ui {
class KChart;
}

class KChart : public QWidget
{
    Q_OBJECT

public:
    explicit KChart(QWidget *parent = nullptr);
    ~KChart();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *e) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::KChart *ui;
    QVector<ChartData *> charts;
    void CalculatorChart();
    double startPressY = -100;
    double disappearArea = 0; // 不可见chart区域 负值
    double allChartHeight = 0; // chart的总高度
};

#endif // KCHART_H
