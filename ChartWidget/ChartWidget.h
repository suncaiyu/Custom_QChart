#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QMainWindow>
#include <QPainter>
#include "Chart.h"
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class ChartWidget; }
QT_END_NAMESPACE

// chart的绘制页面
class ChartWidget : public QMainWindow
{
    Q_OBJECT

public:
    ChartWidget(QWidget *parent = nullptr);
    ~ChartWidget();

    void updateData();

protected:
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent *e);

public slots:
    void StartUpdateSlot();
    void StopUpdateSlot();

public:
    Ui::ChartWidget *ui;
    int heightSpacing = 10;

    //time分成了多少份;
    int timeSplite = 50;  //可以作为 timeline与chart数据的中间桥梁
    float timeSpace = 10; //

    int startIndex = 0;

    float timelinestepwidth;

    int chartNumber = 3;
    void PaintChart(QPainter *p, Chart *c);

    Chart *chart1;
    Chart *chart2;
    Chart *chart3;

    bool isUpdate = true;
};
#endif // CHARTWIDGET_H
