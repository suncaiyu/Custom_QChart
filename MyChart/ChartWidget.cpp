#include "ChartWidget.h"
#include <QVBoxLayout>
#include "TimeRuler.h"
#include "KChart.h"
ChartWidget::ChartWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);
    chart = std::make_unique<KChart>();
    timeLine = std::make_unique<TimeRuler>();
    chart->SetTimeLine(&*timeLine);
    mainLayout = std::make_unique<QVBoxLayout>(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(1);
    mainLayout->addWidget(timeLine.get());
    mainLayout->addWidget(&*chart);
    setLayout(mainLayout.get());
}

ChartWidget::~ChartWidget()
{
}
