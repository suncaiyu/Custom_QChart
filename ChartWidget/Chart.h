#ifndef CHART_H
#define CHART_H

#include <QWidget>

namespace Ui {
class Chart;
}

// 绘制每个chart的 需要的各项数据
class Chart : public QWidget
{
    Q_OBJECT

public:
    explicit Chart(QWidget *parent = nullptr);
    ~Chart();
    void CreateInitNumber();
    float chartHeight; //必须的。chart的高度
    int spliteNumber; // 必须的 表示timeline的对应刻度数亮
    float timespace; //timeline的编剧

    QColor backgroundColor; // 必须的，chart的背景色
    QColor lineColor; // 必须的chart的线色
    float chartYNumber; // 必须的 chart的刻度数
    float stepYnumber; //可计算的 每个可读的增加值
    QVector<float> dataVector; // data的存放数据
    float stepXNumber; //表示timrline的step的增加值
    float chartTop; // chart的顶部
    float chartBottom; // chart的地步

    int myIndex; //chart的别名，表示第几个在界面上
    void SetChartTopAndBtn();

    QRect myRect; // 当前的位置
    QRect fixRect; // 理论上的位置

    bool isSelect = false; //是否被点击选中
    int lastPosY;  //上一次的Y的位置，用来帮助鼠标move移动的
    QPoint centerPos; //chart的中部位置，方便chart拖动交换判断
    void SetCenterPos();

    QPointF tempData; //上一个点的位置，记录绘图数据点的

private:
    Ui::Chart *ui;

};

#endif // CHART_H
