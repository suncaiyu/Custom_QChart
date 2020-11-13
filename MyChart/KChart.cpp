#include "KChart.h"
#include "ui_KChart.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QTimer>
#include <QWheelEvent>
#include <queue>
#include "TimeRuler.h"

KChart::KChart(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::KChart)
{
    ui->setupUi(this);
    setMouseTracking(true);
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
    cdddd->title = "title2";
    cdddd->color = QColor(rand() % 255, rand() % 255, rand() % 255);
    cdddd->height = 20;
    charts.push_back(cdddd);
    ChartData *cddddd = new ChartData(cdddd);
    cddddd->isTitle = true;
    cddddd->title = "title3";
    cddddd->color = QColor(rand() % 255, rand() % 255, rand() % 255);
    cddddd->height = 20;
    charts.push_back(cddddd);
    ChartData *cdddddd = new ChartData(cddddd);
    cdddddd->isTitle = true;
    cdddddd->title = "title4";
    cdddddd->color = QColor(rand() % 255, rand() % 255, rand() % 255);
    cdddddd->height = 20;
    charts.push_back(cdddddd);
    for (int i = 0; i < 4; i++) {
        ChartData *cd = new ChartData(cdd);
        cd->color = QColor(rand() % 255, rand() % 255, rand() % 255);
        cd->height = 50;
        for (int i = 0; i < 200; i++) {
            Data *d = new Data;
            d->time = (long long)1222222222 * i;
            d->value = rand() % 10;
            cd->data.push_back(d);
        }
        charts.push_back(cd);
    }
    for (int i = 0; i < 3; i++) {
        ChartData *cd = new ChartData(cdddd);
        cd->color = QColor(rand() % 255, rand() % 255, rand() % 255);
        cd->height = 50;
        for (int i = 0; i < 200; i++) {
            Data *d = new Data;
            d->time = (long long)1222222222 * i;
            d->value = rand() % 11;
            cd->data.push_back(d);
        }
        charts.push_back(cd);
    }
    for (int i = 0; i < 4; i++) {
        ChartData *cd = new ChartData(cdddddd);
        cd->color = QColor(rand() % 255, rand() % 255, rand() % 255);
        cd->height = 50;
        for (int i = 0; i < 200; i++) {
            Data *d = new Data;
            d->time = (long long)1222222222 * i;
            d->value = rand() % 11;
            cd->data.push_back(d);
        }
        charts.push_back(cd);
    }
    for (int i = 0; i < 4; i++) {
        ChartData *cd = new ChartData(cddddd);
        cd->color = QColor(rand() % 255, rand() % 255, rand() % 255);
        cd->height = 50;
        for (int i = 0; i < 200; i++) {
            Data *d = new Data;
            d->time = (long long)1222222222 * i;
            d->value = rand() % 11;
            cd->data.push_back(d);
        }
        charts.push_back(cd);
    }
    QTimer *updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, [this]() { update(); });
    updateTimer->start(30);
    charts = SortCharts();
    chartLabelAreaColor = Qt::gray;
    chartAreaColor = Qt::white;
}

/**
* @brief 对chart排序，使得子节点对应挂载到父节点上
*/
QVector<ChartData *> KChart::SortCharts()
{
    //对chart排序，使得子节点对应挂载到父节点上，如果一个节点的parent是nullptr，他是顶级节点
    std::queue<ChartData *> tmpCharts;
    QVector<ChartData *> sortCharts;
    QVector<ChartData *> title;
    QVector<ChartData *> chart;
    for (ChartData *cd : charts) {
        if (cd->parent == nullptr) {
            tmpCharts.push(cd);
            while (!tmpCharts.empty()) {
                ChartData *tmpChartData = tmpCharts.front();
                sortCharts.push_back(tmpChartData);
                tmpCharts.pop();
                chart.clear();
                title.clear();
                for (ChartData *cd : charts) {
                    if (cd->level == tmpChartData->level + 1 && cd->parent == tmpChartData) {
                        // 这里吧找到的chartdata分类储存
                        if (cd->isTitle) {
                            title.push_back(cd);
                        }
                        else {
                            chart.push_back(cd);
                        }
                    }
                }
                // 最后push
                // 先push chart
                for (ChartData *cd : chart) {
                    tmpCharts.push(cd);
                }
                // 再push title 因为title下要紧跟chart否则可能同层级既有chart又有title时，会逻辑出错
                for (ChartData *cd : title) {
                    tmpCharts.push(cd);
                }
            }
        }
    }
    //排完之后，同一层级有title与chart的，chart在前面

    return sortCharts;
}
/**
* @brief 计算chart的top，bottom，以及每层title的下一层chart的高度信息等等
*/
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
            // 如果第一个 top从0 + disappear开始
            charts[i]->top = 0 + disappearArea;
            charts[i]->bottom = charts[i]->top + charts[i]->height;
            allChartHeight += charts[i]->height;
            childrenHeight += charts[i]->height;
        }
        else {
            // 否则下一个的top就是上一个的bottom
            charts[i]->top = charts[i - 1]->bottom;
            if (charts[i]->isShow) { // chart可见
                // 如果是顶级title，不可被隐藏
                if (charts[i]->parent == nullptr && charts[i]->isTitle) {
                    charts[i]->bottom = charts[i]->top + charts[i]->height;
                    allChartHeight += charts[i]->height;
                    childrenHeight += charts[i]->height;
                }
                else if (charts[i]->parent != nullptr) { // 如果不是顶级title，父节点可见
                   //一直往上找，如果所有的parent可见，他就可见，否则不可见
                   // 所以会存在虽然他是可见的，但是他的父节点或祖父或以上的节点不可见，这时，他的bottom = top，虽然他是可见的
                    ChartData *chartData = charts[i];
                    bool flag = true;
                    while (chartData->parent != nullptr) {
                        if (!chartData->isShow) {
                            flag = false;
                            break;
                        }
                        chartData = chartData->parent;
                    }
                    if (flag) { //所有父节点都是可见的
                        charts[i]->bottom = charts[i]->top + charts[i]->height;
                        allChartHeight += charts[i]->height;
                        childrenHeight += charts[i]->height;
                    }
                    else {
                        charts[i]->bottom = charts[i]->top + 0;
                        allChartHeight += 0;
                        childrenHeight += 0;
                    }
                }
            }
            else { //chart不可见
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
            }
            else {
                charts[i]->swapingTop = (charts[i]->top + 4. * charts[i]->swapingTop) / 5.;
            }
        }
        if (charts[i]->isTitle) { //如果是title，需要计算一些值
            if (titleCd == nullptr) { // 如果titleCd是空的，证明这是第一个找到的title
                titleCd = charts[i]; //把title赋值给titleCD
                titleCd->childMinTop = titleCd->bottom; // 获取到子child的第一个高度,他将与allChildrenheight决定子chart能够交换位置的空间(同一个父节点下的才能交换)
            }
            else {
                titleCd->allChildrenHeight = childrenHeight - titleCd->height; //找到了下一个title，将此时计算的childrenheight - 自身的高度，赋值给上一个title
                titleCd = charts[i]; // titleCd 换成新找到的
                titleCd->childMinTop = titleCd->bottom; // 老规矩
            }
            childrenHeight = 0;
        }
    }
    // 当遍历结束，找到最后一个chart后，吧计算的结果赋值给上一个找到的titieCd,很重要，否则最后一个title没有child高度，不能进行child交换
    if (titleCd != nullptr) {
        titleCd->allChildrenHeight = childrenHeight;
        titleCd->childMinTop = titleCd->bottom;
    }
    CalculatorTitleVaule();
}
/**
* @brief 计算每层title下面所有子节点的高度(以后可能做title交换用，谁知道什么时候实现呢☺)
*/
void KChart::CalculatorTitleVaule()
{
    QVector<int> topTitleIndex;
    for (unsigned int i = 0; i < charts.size(); i++) {
        ChartData *cd = charts[i];
        if (cd->isTitle && cd->parent == nullptr) {
            topTitleIndex.push_back(i);
        }
    }
    int last = charts.size() - 1;
    for (int i = topTitleIndex.size() - 1; i >= 0; i--) {
        int allHeight = 0;
        for (int j = last; j >= topTitleIndex[i]; j--) {
            if (charts[j]->isTitle) {
                if (charts[j]->allChildrenHeight == 0) {
                    charts[j]->allHeight = allHeight;
                }
                else {
                    allHeight += charts[j]->allChildrenHeight;
                    charts[j]->allHeight = allHeight;
                    allHeight += charts[j]->height;
                }
            }
        }
        last = topTitleIndex[i] - 1;
    }
    int yy = 0;
}

void KChart::paintEvent(QPaintEvent *event)
{
    CalculatorChart();
    QPainter p(this);
    p.setRenderHints(QPainter::Antialiasing, true);
    p.setRenderHints(QPainter::TextAntialiasing, true);
    int isSelect = -1;
    for (int i = 0; i < charts.size(); i++) {
        // 不可见的就不画了
        if (charts[i]->bottom < 0 || charts[i]->top > height()) {
            continue;
        }
        if (charts[i]->isTitle) { //如果是title，并且可见
            if (charts[i]->isShow) {
                // 画展开标志
                double iconLongWidth = charts[i]->height / 2;
                double iconShortWidth = charts[i]->height / 4;
                QPainterPath path;
                if (!charts[i]->children.isEmpty()) { //判断子节点是否为空
                    if (charts[i]->bottom - charts[i]->top != 0) { // 判断节点虽然可见，但是他的父节点是否允许他显示
                        if (charts[i]->children[0]->isShow) { // 判断子节点的展开情况
                            path.moveTo(iconShortWidth + charts[i]->level * iconSpacing, iconLongWidth + charts[i]->top);
                            path.lineTo(iconShortWidth + iconLongWidth + charts[i]->level * iconSpacing, iconLongWidth + charts[i]->top);
                            path.lineTo(iconShortWidth * 2 + charts[i]->level * iconSpacing, iconLongWidth + charts[i]->top + iconShortWidth);
                            path.lineTo(iconShortWidth + charts[i]->level * iconSpacing, iconLongWidth + charts[i]->top);
                        }
                        else {
                            path.moveTo(charts[i]->level * iconSpacing + iconShortWidth, iconShortWidth + charts[i]->top);
                            path.lineTo(charts[i]->level * iconSpacing + iconLongWidth, iconLongWidth + charts[i]->top);
                            path.lineTo(charts[i]->level * iconSpacing + iconShortWidth, iconLongWidth + charts[i]->top + iconShortWidth);
                            path.lineTo(charts[i]->level * iconSpacing + iconShortWidth, iconShortWidth + charts[i]->top);
                        }
                    }
                }

                QLinearGradient linear(QPointF(width(), charts[i]->bottom), QPointF(0, charts[i]->top));
                linear.setColorAt(0, Qt::black);
                linear.setColorAt(1, Qt::gray);

                // 设置显示模式
                linear.setSpread(QGradient::PadSpread);
                p.fillRect(QRectF(0, charts[i]->top, width(), charts[i]->bottom - charts[i]->top), linear);
                // 画标志
                p.fillPath(path, Qt::black);
                p.drawText(QRectF((charts[i]->level + 1) * iconSpacing + iconLongWidth, charts[i]->top, width(), charts[i]->bottom - charts[i]->top),
                           Qt::AlignVCenter,
                           charts[i]->title);
            }
        }else
        // 如果没有被拖拽，没有在交换，正常画
        if (charts[i]->isDraging == false && charts[i]->isSwaping == false) {
            QRectF chartRect(0, charts[i]->top, width(), charts[i]->bottom - charts[i]->top);
            p.fillRect(chartRect, chartAreaColor);
            p.save();
            PaintData(p, charts[i]);
            p.restore();
            double chartHeight = charts[i]->bottom - charts[i]->top;
            chartHeight == 0 ? 0 : chartHeight - 1.;
            QRectF chartLabelRect(0, charts[i]->top + 0.5, chartLabelAreaWidth, chartHeight);
            p.fillRect(chartLabelRect, chartLabelAreaColor);
            p.drawText(chartLabelRect, Qt::AlignCenter, charts[i]->label);
        } else if (charts[i]->isDraging) { // 如果正在拖拽，记下位置，最后画
            isSelect = i;
        } else if (charts[i]->isSwaping) { // 如果正在交换，top是swapingtop。正在变化，有动画效果
            QRectF chartRect(0, charts[i]->swapingTop, width(), charts[i]->height);
            p.fillRect(chartRect, chartAreaColor);
            p.save();
            PaintData(p, charts[i]);
            p.restore();
            double chartHeight = charts[i]->bottom - charts[i]->top;
            chartHeight == 0 ? 0 : chartHeight - 1.;
            QRectF chartLabelRect(0, charts[i]->swapingTop + 0.5, chartLabelAreaWidth, chartHeight);
            p.fillRect(chartLabelRect, chartLabelAreaColor);
            p.drawText(chartLabelRect, Qt::AlignCenter, charts[i]->label);
        }
    }
    if (isSelect != -1) {
        QRect chartRect(0, charts[isSelect]->dragTop, rect().width(), charts[isSelect]->height);
        p.fillRect(chartRect, chartAreaColor);
        p.save();
        PaintData(p, charts[isSelect]);
        p.restore();
        double chartHeight = charts[isSelect]->bottom - charts[isSelect]->top;
        chartHeight == 0 ? 0 : chartHeight - 1.;
        QRectF chartLabelRect(0, charts[isSelect]->dragTop + 0.5, chartLabelAreaWidth, chartHeight);
        p.fillRect(chartLabelRect, chartLabelAreaColor);
        p.drawText(chartLabelRect, Qt::AlignCenter, charts[isSelect]->label);
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
void KChart::SetTimeLine(TimeRuler *t)
{
    timeLine = t;
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
    timeLine->winContext.mouseLocationX = e->pos().x();
    timeLine->update();
    for (ChartData *cd : charts) {
        if (QRectF(chartLabelAreaWidth, cd->top, width() - chartLabelAreaWidth, cd->bottom - cd->top).contains(e->pos())) {
            cd->showData = true;
        }
        else {
            cd->showData = false;
        }
    }
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
        QRect chartRect(0, cd->top, chartLabelAreaWidth, cd->bottom - cd->top);
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
            if (disappearArea < 0 && e->delta() > 0) {
                disappearArea += 10;
            }
            else {
                return;
            }
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

void KChart::PaintData(QPainter &p, ChartData *cd)
{
    //PaintLineData(p, cd);
    //PaintBarData(p,cd);
    PaintBarData_2(p,cd);
    //PaintLineData_2(p,cd);
}
void KChart::PaintLineData(QPainter &p, ChartData *cd)
{
    if (cd->bottom - cd->top == 0) {
        return;
    }
    QPointF preDot(-1, -1);
    for (Data *d : cd->data) {
        if (d->time < timeLine->GetLeftTime() || d->time > timeLine->GetRightTime()) {
            continue;
        }
        double usefulHeight = cd->height * 4. / 5.; // 画图不占满高，值占用4/5，好看一点
        double padding = cd->height / 5. / 2.; // 上下都留1/10
        double scale = usefulHeight / cd->asixYScale;
        double x = d->time * timeLine->winContext.viewWindowA + timeLine->winContext.viewWindowB;
        double y = 0;
        if (cd->isDraging) {
            y = cd->dragTop + cd->height - padding - d->value * scale;
        }
        else if (cd->isSwaping) {
            y = cd->swapingTop + cd->height - padding - d->value * scale;
        }
        else {
            y = cd->top + cd->height - padding - d->value * scale;
        }
        QPointF nowDot(x,y);
        if (preDot.x() == -1 && preDot.y() == -1) {
            preDot = nowDot;
            if (cd->showData) {
                p.drawText(nowDot, QString::number(d->value));
            }
            continue;
        }
        p.setPen(cd->color);
        p.drawLine(preDot, nowDot);
        preDot = nowDot;
        if (cd->showData) {
            p.setPen(Qt::black);
            p.drawText(nowDot, QString::number(d->value));
        }
    }
}

void KChart::PaintLineData_2(QPainter &p, ChartData *cd)
{
    if (cd->bottom - cd->top == 0) {
        return;
    }
    QPointF preDot(-1, -1);
    for (Data *d : cd->data) {
        if (d->time < timeLine->GetLeftTime() || d->time > timeLine->GetRightTime()) {
            continue;
        }
        double usefulHeight = cd->height * 4. / 5.; // 画图不占满高，值占用4/5，好看一点
        double padding = cd->height / 5. / 2.; // 上下都留1/10
        double scale = usefulHeight / cd->asixYScale;
        double x = d->time * timeLine->winContext.viewWindowA + timeLine->winContext.viewWindowB;
        double y = 0;
        if (cd->isDraging) {
            y = cd->dragTop + cd->height - padding - d->value * scale;
        }
        else if (cd->isSwaping) {
            y = cd->swapingTop + cd->height - padding - d->value * scale;
        }
        else {
            y = cd->top + cd->height - padding - d->value * scale;
        }
        QPointF nowDot(x, y);
        if (preDot.x() == -1 && preDot.y() == -1) {
            preDot = nowDot;
            if (cd->showData) {
                p.drawText(nowDot, QString::number(d->value));
            }
            continue;
        }
        p.setPen(cd->color);
        QPainterPath path;
        path.moveTo(preDot);
        path.lineTo(nowDot);
        path.lineTo(nowDot.x(), cd->bottom - padding);
        path.lineTo(preDot.x(), cd->bottom - padding);
        p.fillPath(path, QColor(cd->color.red(), cd->color.green(), cd->color.blue(), 100));
        p.drawLine(preDot, nowDot);
        preDot = nowDot;
        if (cd->showData) {
            p.setPen(Qt::black);
            p.drawText(nowDot, QString::number(d->value));
        }
    }
}

void KChart::PaintBarData(QPainter &p, ChartData *cd)
{
    if (cd->bottom - cd->top == 0) {
        return;
    }
    QPointF preDot(-1, -1);
    for (Data *d : cd->data) {
        if (d->time < timeLine->GetLeftTime() || d->time > timeLine->GetRightTime()) {
            continue;
        }
        double usefulHeight = cd->height * 4. / 5.; // 画图不占满高，值占用4/5，好看一点
        double padding = cd->height / 5. / 2.; // 上下都留1/10
        double scale = usefulHeight / cd->asixYScale;
        double x = d->time * timeLine->winContext.viewWindowA + timeLine->winContext.viewWindowB;
        double y = 0;
        if (cd->isDraging) {
            y = cd->dragTop + cd->height - padding - d->value * scale;
        }
        else if (cd->isSwaping) {
            y = cd->swapingTop + cd->height - padding - d->value * scale;
        }
        else {
            y = cd->top + cd->height - padding - d->value * scale;
        }
        QPointF nowDot(x, y);
        if (preDot.x() == -1 && preDot.y() == -1) {
            preDot = nowDot;
            if (cd->showData) {
                p.drawText(nowDot, QString::number(d->value));
            }
            continue;
        }
        p.setPen(cd->color);
        p.drawLine(preDot, QPointF(nowDot.x(), preDot.y()));
        p.drawLine(QPointF(nowDot.x(), preDot.y()), nowDot);
        preDot = nowDot;
        if (cd->showData) {
            p.setPen(Qt::black);
            p.drawText(nowDot, QString::number(d->value));
        }
    }
}

void KChart::PaintBarData_2(QPainter &p, ChartData *cd)
{
    if (cd->bottom - cd->top == 0) {
        return;
    }
    QPointF preDot(-1, -1);
    for (Data *d : cd->data) {
        if (d->time < timeLine->GetLeftTime() || d->time > timeLine->GetRightTime()) {
            continue;
        }
        double usefulHeight = cd->height * 4. / 5.; // 画图不占满高，值占用4/5，好看一点
        double padding = cd->height / 5. / 2.; // 上下都留1/10
        double scale = usefulHeight / cd->asixYScale;
        double x = d->time * timeLine->winContext.viewWindowA + timeLine->winContext.viewWindowB;
        double y = 0;
        if (cd->isDraging) {
            y = cd->dragTop + cd->height - padding - d->value * scale;
        }
        else if (cd->isSwaping) {
            y = cd->swapingTop + cd->height - padding - d->value * scale;
        }
        else {
            y = cd->top + cd->height - padding - d->value * scale;
        }
        QPointF nowDot(x, y);
        if (preDot.x() == -1 && preDot.y() == -1) {
            preDot = nowDot;
            if (cd->showData) {
                p.drawText(nowDot, QString::number(d->value));
            }
            continue;
        }
        p.setPen(cd->color);
        QRectF barRect(preDot.x(), preDot.y(),nowDot.x() - preDot.x(),cd->bottom - padding - preDot.y());
        p.fillRect(barRect, QColor(cd->color.red(),cd->color.green(), cd->color.blue(), 100));
        p.drawLine(preDot, QPointF(nowDot.x(), preDot.y()));
        p.drawLine(QPointF(nowDot.x(), preDot.y()), nowDot);
        preDot = nowDot;
        if (cd->showData) {
            p.setPen(Qt::black);
            p.drawText(nowDot, QString::number(d->value));
        }
    }
}