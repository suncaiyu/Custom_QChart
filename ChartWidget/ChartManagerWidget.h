#ifndef CHARTMANAGERWIDGET_H
#define CHARTMANAGERWIDGET_H
#include "ChartWidget.h"
#include "TimeLine.h"
#include <QWidget>

namespace Ui {
class ChartManagerWidget;
}

class ChartManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChartManagerWidget(QWidget *parent = nullptr);
    ~ChartManagerWidget();

protected:
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *e);


private:
    Ui::ChartManagerWidget *ui;
    //    QChartXLine *timeLine;
    TimeLine *onlyTimeLine;
    ChartWidget *mychart;
    QTimer *timer;
};

#endif // CHARTMANAGERWIDGET_H
