#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QMainWindow>
#include <QPainter>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class ChartWidget; }
QT_END_NAMESPACE

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

public:
    Ui::ChartWidget *ui;
    int heightSpacing = 10;
    float stepH;
    float spliteH = 100;
    QVector<float> dataVector;

    //time分成了多少份;
    int timeSplite = 50;
    QPointF tempData;
    float timeSpace = 10;
    QTimer *timer;

    int startIndex = 0;

    float timelinestepwidth;

    QColor randomColor;
    QColor penRandomColor;

};
#endif // CHARTWIDGET_H
