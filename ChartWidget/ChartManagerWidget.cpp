#include "ChartManagerWidget.h"
#include "ui_ChartManagerWidget.h"
#include <QPainter>
#include <QStyleOption>
#include <QVBoxLayout>

ChartManagerWidget::ChartManagerWidget(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::ChartManagerWidget)
{
    ui->setupUi(this);
    QVBoxLayout *mainLayout = new QVBoxLayout;

    timeLine = new QChartXLine;
    mychart = new ChartWidget;
       ChartWidget *mychart2 = new ChartWidget;
        ChartWidget *mychart3 = new ChartWidget;
    mainLayout->addWidget(timeLine);
    mainLayout->addWidget(mychart);
    mainLayout->addWidget(mychart2);
    mainLayout->addWidget(mychart3);
    this->setLayout(mainLayout);

    timer = new QTimer;
    connect(timer, &QTimer::timeout, timeLine, &QChartXLine::updateText);
    connect(timer, &QTimer::timeout, mychart, &ChartWidget::updateData);
        connect(timer, &QTimer::timeout, mychart2, &ChartWidget::updateData);
            connect(timer, &QTimer::timeout, mychart3, &ChartWidget::updateData);
    timer->start(20);
    mychart->timeSplite = timeLine->spliteSize;
    mychart->timelinestepwidth = timeLine->stepwidth;
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

