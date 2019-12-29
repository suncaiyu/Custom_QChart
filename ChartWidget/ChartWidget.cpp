#include "ChartWidget.h"
#include "ui_ChartWidget.h"
#include <QPainter>
#include <QDebug>

ChartWidget::ChartWidget(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::ChartWidget)
{
    ui->setupUi(this);
    for (int i = 0; i <= timeSplite; i++) {
        dataVector.push_back(qrand() % 100);
    }

    //    timer = new QTimer(this);
    //    connect(timer, &QTimer::timeout, this, &ChartWidget::updateData);
    //    timer->start(1000);
    randomColor = QColor(qrand()%255,qrand()%255,qrand()%255);
    penRandomColor = QColor(qrand()%200,qrand()%220,qrand()%240);
}

void ChartWidget::updateData()
{
    startIndex++;
    dataVector.push_back(qrand() % 100);
    update();
}

ChartWidget::~ChartWidget()
{
    delete ui;
}

void ChartWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.fillRect(rect(), QBrush(randomColor));
    painter.setRenderHint(QPainter::Antialiasing, 1);
    float totalheight = height();
    stepH = (totalheight - 2 * heightSpacing) / spliteH;

    //高度就是 heightSpacing + x * stepH
    //假设剑阁为10；
    QPen pen; //创建一个画笔
    pen.setColor(penRandomColor);
    pen.setWidth(1);
    painter.setPen(pen);
    float secondData;
    for (int i = startIndex; i < dataVector.size(); i++) {
        if (i == startIndex) {
            tempData = QPointF(timeSpace , dataVector[i] * stepH + heightSpacing);
        } else {
            secondData = dataVector[i];
            QPointF one = tempData;

            QPointF two = QPointF(timeSpace + timelinestepwidth * (i - startIndex), heightSpacing + secondData * stepH);

            painter.drawLine(one, two);
            tempData = two;
        }
    }
}

void ChartWidget::resizeEvent(QResizeEvent *event)
{
    float panelwidth = width() - 2 * timeSpace;
    timelinestepwidth = (float) panelwidth / timeSplite;
}

