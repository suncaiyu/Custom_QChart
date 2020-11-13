#pragma once

#include <QWidget>
#include "ui_ChartWidget.h"
#include <memory>

class TimeRuler;
class KChart;
class QVBoxLayout;
class ChartWidget : public QWidget, public Ui::ChartWidget
{
    Q_OBJECT

public:
    ChartWidget(QWidget *parent = Q_NULLPTR);
    ~ChartWidget();
    std::unique_ptr<TimeRuler> timeLine;
    std::unique_ptr<KChart> chart;
    std::unique_ptr<QVBoxLayout> mainLayout;
};
