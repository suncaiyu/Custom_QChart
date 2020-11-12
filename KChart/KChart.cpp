#include "KChart.h"
#include "ui_KChart.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QTimer>
#include <QWheelEvent>
#include <queue>

KChart::KChart(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::KChart)
{
    ui->setupUi(this);

    /*****注意事项******/
    // 在创建ChartData结构的时候，要遵循，从上到下的规则
    //chartdata0(nullptr)   ->根1
    //  chartdataA(chartdata0) ->第二层1
    //    chartdataI(chartdataA)  ->正式chart
    //    chartdataII(chartdataA) ->正式chart
    //    chartdataIII(chartdataA)->正式chart
    //  chartdataB(chartdata0)->第二层2
    //chartdata1(nullptr)   ->根2
    // 反正搞完用SortCharts排一下序 
    ChartData *cdd = new ChartData();
    cdd->isTitle = true;
    cdd->title = "title0";
    cdd->color = QColor(rand() % 255, rand() % 255, rand() % 255);
    cdd->height = 20;
    charts.push_back(cdd);
    ChartData *cddd = new ChartData();
    cddd->isTitle = true;
    cddd->title = "title1";
    cddd->color = QColor(rand() % 255, rand() % 255, rand() % 255);
    cddd->height = 20;
    charts.push_back(cddd);
    ChartData *cdddd = new ChartData(cddd);
    cdddd->isTitle = true;
    cdddd->title = "title11";
    cdddd->color = QColor(rand() % 255, rand() % 255, rand() % 255);
    cdddd->height = 20;
    charts.push_back(cdddd);
    for (int i = 0; i < 8; i++) {
        ChartData *cd = new ChartData(cdd);
        cd->color = QColor(rand() % 255, rand() % 255, rand() % 255);
        cd->height = 50;
        charts.push_back(cd);
    }
    for (int i = 0; i < 4; i++) {
        ChartData *cd = new ChartData(cdddd);
        cd->color = QColor(rand() % 255, rand() % 255, rand() % 255);
        cd->height = 50;
        charts.push_back(cd);
    }
    QTimer *updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, [this]() { update(); });
    updateTimer->start(30);
    charts = SortCharts(charts);
}

QVector<ChartData *> KChart::SortCharts(QVector<ChartData *> &charts)
{
    std::queue<ChartData *> tmpCharts;
    QVector<ChartData *> sortCharts;
    for (ChartData *cd : charts) {
        if (cd->parent == nullptr) {
            tmpCharts.push(cd);
            while (!tmpCharts.empty()) {
                ChartData *tmpChartData = tmpCharts.front();
                sortCharts.push_back(tmpChartData);
                tmpCharts.pop();
                for (ChartData *cd : charts) {
                    if (cd->level == tmpChartData->level + 1 && cd->parent == tmpChartData) {
                        tmpCharts.push(cd);
                    }
                }
            }
        }
    }
    return sortCharts;
}
void KChart::CalculatorChart()
{
    // 所有chart的高度
    allChartHeight = 0;
    // 计算所有title下面的所有子chart的高度
    double childrenHeight = 0;
    ChartData *titleCd = nullptr;
    for (int i = 0; i < charts.size(); i++) {
        // 遍历，计算正常的位置
        if (i == 0) {
            charts[i]->top = 0 + disappearArea;
            charts[i]->bottom = charts[i]->top + charts[i]->height;
            allChartHeight += charts[i]->height;
            childrenHeight += charts[i]->height;
        } else {
            charts[i]->top = charts[i - 1]->bottom;
            if (charts[i]->isShow) {
                if (charts[i]->parent == nullptr && charts[i]->isTitle) {
                    charts[i]->bottom = charts[i]->top + charts[i]->height;
                    allChartHeight += charts[i]->height;
                    childrenHeight += charts[i]->height;
                }else if(charts[i]->parent != nullptr && charts[i]->parent->isShow){
                    charts[i]->bottom = charts[i]->top + charts[i]->height;
                    allChartHeight += charts[i]->height;
                    childrenHeight += charts[i]->height;
                }else{
                    charts[i]->bottom = charts[i]->top + 0;
                    allChartHeight += 0;
                    childrenHeight += 0;
                }
            }else{
                charts[i]->bottom = charts[i]->top + 0;
                allChartHeight += 0;
                childrenHeight += 0;
            }
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
        if (charts[i]->isTitle) {
            if (titleCd == nullptr) {
                titleCd = charts[i];
                childrenHeight = 0;
                titleCd->childMinTop = titleCd->bottom;
            } else {
                titleCd->allChildrenHeight = childrenHeight - titleCd->height;
                childrenHeight = 0;
                titleCd = charts[i];
                titleCd->childMinTop = titleCd->bottom;
            }
            childrenHeight = 0;
        }
    }
    if (titleCd != nullptr) {
        titleCd->allChildrenHeight = childrenHeight;
        titleCd->childMinTop = titleCd->bottom;
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
        if (charts[i]->isTitle) {
            if (charts[i]->isShow) {
                p.drawText(QRectF(0, charts[i]->top, width(), charts[i]->bottom - charts[i]->top),
                           Qt::AlignVCenter,
                           charts[i]->title);
            }
        }else
        // 如果没有被拖拽，没有在交换，正常画
        if (charts[i]->isDraging == false && charts[i]->isSwaping == false) {
            QRectF chartRect(0, charts[i]->top, width(), charts[i]->bottom - charts[i]->top);
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
    for (ChartData *cd : charts) {
        if (cd->isTitle && cd->parent == nullptr) {
            delete cd;
            cd = nullptr;
        }
    }
    charts.clear();
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
    if (distance > -0.001 && distance < 0.001) {
        return;
    }
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
            if (dragRectCenter
                > isSelectChart->parent->childMinTop + isSelectChart->parent->allChildrenHeight) {
                return;
            }
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
            if (dragRectCenter < isSelectChart->parent->childMinTop) {
                return;
            }
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
            if (cd->isShow) {
                if (cd->isTitle) {
                    cd->SetChildrenHideOrShow();
                    break;
                }
                cd->isDraging = true;
                startPressY = e->pos().y();
                break;
            }
        }
    }
}

void KChart::wheelEvent(QWheelEvent *e)
{
    if (QApplication::keyboardModifiers() == Qt::ControlModifier) {
        for (ChartData *cd : charts) {
            if (QRectF(0, cd->top, width(), cd->height).contains(e->pos())) {
                if (cd->isTitle) {
                    return;
                }
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
