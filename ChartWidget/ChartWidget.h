#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QMainWindow>
#include <QPainter>
#include "Chart.h"
#include <QTimer>
#include "TimeLine.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ChartWidget; }
QT_END_NAMESPACE

static bool comQt(Chart &a, Chart &b);

// chart的绘制页面
class ChartWidget : public QMainWindow
{
    Q_OBJECT

public:
    ChartWidget(TimeLine *timeline, QWidget *parent = nullptr);
    ~ChartWidget();

    void updateData();

protected:
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *event);

public slots:
    void StartUpdateSlot();
    void StopUpdateSlot();
    void GuiUpdate();

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

    QList<Chart *> iList;

    bool isUpdate = true;
    TimeLine *onlyTimeLine;

    qreal GetDate(Chart *c); //返回time的时间点

    void ResetiListRect();
    int dragindex = -1;
    Chart *dragChart;
    bool ChartWidget::comQt(const Chart a, const Chart b);
};
#endif // CHARTWIDGET_H
