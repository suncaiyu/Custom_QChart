#include "QChartXLine.h"
#include "ui_QChartXLine.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

QChartXLine::QChartXLine(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::QChartXLine)
{
    ui->setupUi(this);
    isUpdate = true;
    lineAtY = 20;
    space = 10;
//    timer = new QTimer(this);
//    connect(timer, &QTimer::timeout, this, &QChartXLine::updateText);
//    timer->start(1000);
    spliteSize = 50;
    for (int i = 0; i <= spliteSize; i++) {
        textVector.push_back(i);
    }
}

void QChartXLine::updateText()
{
    int temp = textVector[spliteSize];
    textVector.push_back(temp + 1);
    if (textVector.size() > spliteSize + 1) {
        textVector.pop_front();
    }
    if (isUpdate) {
        update();
    }
}

void QChartXLine::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isUpdate = false;
        emit StopChartSignal();
    }
    // 如果是鼠标右键按下
    else if(event->button() == Qt::RightButton)
    {
        isUpdate = true;
        emit StartChartSignal();
    }
}

QChartXLine::~QChartXLine()
{
    delete ui;
}

void QChartXLine::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPointF startX1, startX2;
    startX1 = QPointF(space, lineAtY);
    startX2 = QPointF(width() - space, lineAtY);
    painter.drawLine(startX1, startX2);

    int scale = 0;
    for (int i = 0; i <= spliteSize; i++) {
        if (i % 10 == 0) {
            scale = 8;
            QRect textRect(i * stepwidth + space - 10, 0, lineAtY, lineAtY);
            painter.drawText(textRect, Qt::AlignCenter, QString::number(textVector[i]));
        } else {
            scale = 4;
        }
        painter.drawLine(i * stepwidth + space, lineAtY, i * stepwidth + space, lineAtY + scale);
    }
    painter.end();
}

void QChartXLine::resizeEvent(QResizeEvent *event)
{
    float panelwidth = width() - 2 * space;
    stepwidth = (float) panelwidth / spliteSize;
}
