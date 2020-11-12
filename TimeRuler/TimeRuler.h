#ifndef TIMERULER_H
#define TIMERULER_H

#include <QWidget>
#include "ChartWindowContext.h"

namespace Ui {
class TimeRuler;
}
const int TEXT_HEIGHT = 8;
class TimeRuler : public QWidget
{
    Q_OBJECT

public:
    explicit TimeRuler(QWidget *parent = nullptr);
    ~TimeRuler();
    ChartWindowContext winContext;
    ChartWindowContext *pWindowContext;

protected:
    void wheelEvent(QWheelEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Ui::TimeRuler *ui;
    int natureShow = 1;
    QString TransTimeToNatureString(long long);
    QString TransTimeToNatureString_2(long long);
    //    QString TransTimeToNatureString2(long long);
    double startPressX = -100;
    double distance;
    bool isPress = false;

    void PaintMouseMeasureLine(QPainter *p);
};

#endif // TIMERULER_H
