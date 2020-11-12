#include "KChart.h"
#include "ui_KChart.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QTimer>
#include <QWheelEvent>

KChart::KChart(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::KChart)
{
    ui->setupUi(this);
    for (int i = 0; i < 12; i++) {
        ChartData *cd = new ChartData;
        cd->color = QColor(rand() % 255, rand() % 255, rand() % 255);
        cd->height = 50;
        charts.push_back(cd);
    }
    QTimer *updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, [this]() { update(); });
    updateTimer->start(30);
}

void KChart::CalculatorChart()
{
    // 所有chart的高度
    allChartHeight = 0;
    for (int i = 0; i < charts.size(); i++) {
        // 遍历，计算正常的位置
        if (i == 0) {
            charts[i]->top = 0 + disappearArea;
            charts[i]->bottom = charts[i]->top + charts[i]->height;
            allChartHeight += charts[i]->height;
        } else {
            charts[i]->top = charts[i - 1]->bottom;
            charts[i]->bottom = charts[i]->top + charts[i]->height;
            allChartHeight += charts[i]->height;
        }
        // 如果不是在拖拽，dratop和top一样,在拖拽的dragtop在mousemoveevent中由鼠标位置决定
        if (charts[i]->isDraging == false) {
            charts[i]->dragTop = charts[i]->top;
        }
        // 正在交换的swapingtop的位置高速向应该在的top靠近
        if (charts[i]->isSwaping) {
            if (charts[i]->top == charts[i]->swapingTop) {
                charts[i]->isSwaping = false;
                continue;
            }
            if (abs(charts[i]->top - charts[i]->swapingTop) < 2.) {
                charts[i]->swapingTop = charts[i]->top;
            } else {
                charts[i]->swapingTop = (charts[i]->top + 4. * charts[i]->swapingTop) / 5.;
            }
        }
    }
}

void KChart::paintEvent(QPaintEvent *event)
{
    CalculatorChart();
    QPainter p(this);
    int isSelect = -1;
    for (int i = 0; i < charts.size(); i++) {
        // 不可见的就不画了
        if (charts[i]->bottom < 0 || charts[i]->top > height()) {
            continue;
        }
        // 如果没有被拖拽，没有在交换，正常画
        if (charts[i]->isDraging == false && charts[i]->isSwaping == false) {
            QRectF chartRect(0, charts[i]->top, width(), charts[i]->height);
            p.fillRect(chartRect, charts[i]->color);
        } else if (charts[i]->isDraging) { // 如果正在拖拽，记下位置，最后画
            isSelect = i;
        } else if (charts[i]->isSwaping) { // 如果正在交换，top是swapingtop。正在变化，有动画效果
            QRectF chartRect(0, charts[i]->swapingTop, width(), charts[i]->height);
            p.fillRect(chartRect, charts[i]->color);
        }
    }
    if (isSelect != -1) {
        QRect chartRect(0, charts[isSelect]->dragTop, rect().width(), charts[isSelect]->height);
        p.fillRect(chartRect, charts[isSelect]->color);
    }
}

KChart::~KChart()
{
    delete ui;
}

void KChart::mouseReleaseEvent(QMouseEvent *event)
{
    // 释放后，初始化值
    for (int i = 0; i < charts.size(); i++) {
        if (charts[i]->isDraging) {
            charts[i]->isDraging = false;
            charts[i]->isSwaping = true;
            charts[i]->swapingTop = charts[i]->dragTop;
            break;
        }
    }
    startPressY = -100;
}

void KChart::mouseMoveEvent(QMouseEvent *e)
{
    // 找到是那个在拖拽
    ChartData *isSelectChart = nullptr;
    for (ChartData *cd : charts) {
        if (cd->isDraging == true) {
            isSelectChart = cd;
        }
    }
    // 如果没找到，再见，无事发生
    if (isSelectChart == nullptr) {
        return;
    }
    // 鼠标移动差值，加在dragTop上
    double distance = e->pos().y() - startPressY;
    startPressY = e->pos().y();
    isSelectChart->dragTop += distance;
    // 如果差值 > 0,证明在下移
    if (distance > 0) {
        // 向下找
        int index = charts.indexOf(isSelectChart);
        if (index == charts.size() - 1) {
            // 如果是最后一个不处理
        } else {
            // 拖拽的中心位置
            double dragRectCenter = (isSelectChart->dragTop + isSelectChart->dragTop
                                     + isSelectChart->height)
                                    / 2.;
            // 中心大于下一个的顶端，要交换位置
            if (dragRectCenter > charts[index + 1]->top) {
                ChartData *tmp = charts[index + 1];
                // 标记index + 1为正在交换
                tmp->isSwaping = true;
                tmp->swapingTop = tmp->top;
                charts[index + 1] = charts[index];
                charts[index] = tmp;
                // 更新index的值
                index = index + 1;
                // 这里要计算一下，因为有时候是 = 0的，会判断在上移
                CalculatorChart();
            }
        }
    } else {
        // 向上找
        int index = charts.indexOf(isSelectChart);
        if(index == 0){
            // undo
        }else{
            double dragRectCenter = (isSelectChart->dragTop + isSelectChart->dragTop
                                     + isSelectChart->height)
                                    / 2; // 中心
            if(dragRectCenter < charts[index - 1]->bottom){
                ChartData *tmp = charts[index - 1];
                tmp->isSwaping = true;
                tmp->swapingTop = tmp->top;
                charts[index - 1] = charts[index];
                charts[index] = tmp;
                index = index - 1;
            }
        }
    }
}

void KChart::mousePressEvent(QMouseEvent *e)
{
    for (ChartData *cd : charts) {
        QRect chartRect(0, cd->top, rect().width(), cd->height);
        if (chartRect.contains(e->pos())) {
            cd->isDraging = true;
            startPressY = e->pos().y();
            break;
        }
    }
}

void KChart::wheelEvent(QWheelEvent *e)
{
    if (QApplication::keyboardModifiers() == Qt::ControlModifier) {
        for (ChartData *cd : charts) {
            if (QRectF(0, cd->top, width(), cd->height).contains(e->pos())) {
                if (e->delta() > 0) {
                    // 放大
                    // 如果不是最后一个，正常放大
                    if (charts.indexOf(cd) != charts.size() - 1) {
                        cd->height += 2;
                    } else { // 如果是最后一个
                        // 如果disappear = 0，就是当前chart的总高度 大于窗口高度
                        if (disappearArea >= 0) {
                            // 并且当前chart的总高度 大于窗口高度，减少disappear的值，造成向上扩大的感觉，是一开始向下，后来向上
                            if (allChartHeight > height()) {
                                cd->height += 2;
                                disappearArea -= 2;
                            } else {
                                // 并且当前chart的总高度 小于窗口高度，向下扩大
                                cd->height += 2;
                            }
                        } else {
                            // 并且当前chart的总高度 大于窗口高度，减少disappear的值，造成向上扩大的感觉，一直是向上的
                            cd->height += 2;
                            disappearArea -= 2;
                        }
                    }
                } else {
                    //缩小
                    cd->height -= 2;
                    CalculatorChart();
                    //当时最后一个，且chart的总高度大于窗口高度，减少disapper的值
                    if (allChartHeight > height() && charts[charts.size() - 1]->bottom <= height()) {
                        disappearArea = height() - allChartHeight;
                    }
                    // 当chart的总高度小与窗口的高度时，把disapp置位0，因为有时候明明总高度小于窗口高度，但是disappear不是0，会造成         判断错误，这里手动纠正一下
                    if (allChartHeight <= height()) {
                        disappearArea = 0;
                    }
                }
                break;
            }
        }
    } else {
        // chart上下滚动,如果chart总高度不大于窗口高度,不管
        if (allChartHeight <= height()) {
            return;
        }
        // 如果向上滑动,第一个不会大于最顶端
        if (e->delta() > 0) {
            disappearArea += 10;
            if (disappearArea > 0) {
                disappearArea = 0;
            }
        } else { // 如果下滑，最后一个不会小于最低端
            disappearArea -= 10;
            if (allChartHeight + disappearArea < height()) {
                disappearArea = height() - allChartHeight;
            }
        }
    }
}

void KChart::resizeEvent(QResizeEvent *event)
{
    if (allChartHeight > height() && charts[charts.size() - 1]->bottom < height()) {
        disappearArea = height() - allChartHeight;
    }
}
