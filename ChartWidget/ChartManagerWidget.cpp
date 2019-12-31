#include "ChartManagerWidget.h"
#include "ui_ChartManagerWidget.h"
#include <QPainter>
#include <QStyleOption>
#include <QVBoxLayout>
#include <QDebug>

// 最终chart的展示页面
ChartManagerWidget::ChartManagerWidget(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::ChartManagerWidget)
{
    ui->setupUi(this);
    QVBoxLayout *mainLayout = new QVBoxLayout;

    timeLine = new QChartXLine;
    mychart = new ChartWidget;
    ui->verticalLayout_3->addWidget(timeLine);
    mainLayout->addWidget(mychart);
    ui->widget->setLayout(mainLayout);

    timer = new QTimer;
    connect(timer, &QTimer::timeout, timeLine, &QChartXLine::updateText);
    connect(timer, &QTimer::timeout, mychart, &ChartWidget::updateData);
    connect(timeLine, &QChartXLine::StartChartSignal, mychart, &ChartWidget::StartUpdateSlot);
    connect(timeLine, &QChartXLine::StopChartSignal, mychart, &ChartWidget::StopUpdateSlot);
    timer->start(20);
    mychart->timeSplite = timeLine->spliteSize;
    mychart->timelinestepwidth = timeLine->stepwidth;

    ui->widget->setMinimumHeight(150 * 3);
}

ChartManagerWidget::~ChartManagerWidget()
{
    delete ui;
}

void ChartManagerWidget::resizeEvent(QResizeEvent *event)
{
    mychart->timeSplite = timeLine->spliteSize;
    mychart->timelinestepwidth = timeLine->stepwidth;
}

