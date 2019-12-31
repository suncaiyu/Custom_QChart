#ifndef QCHARTXLINE_H
#define QCHARTXLINE_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class QChartXLine;
}

class QChartXLine : public QWidget
{
    Q_OBJECT

public:
    explicit QChartXLine(QWidget *parent = nullptr);
    ~QChartXLine();
    void paintEvent(QPaintEvent *event) override;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

signals:
    void StartChartSignal();
    void StopChartSignal();
public slots:
    void updateText();
public:
    Ui::QChartXLine *ui;
    // line绘制的y点位置
    float lineAtY;
    //两边留白
    float space;

    // 表格分成多少分
    int spliteSize;

    QVector<int> textVector;

    QTimer *timer;

    bool isUpdate;

    float stepwidth = 20;
};

#endif // QCHARTXLINE_H
