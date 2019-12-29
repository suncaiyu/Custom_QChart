#include "TestCustomPlot.h"
#include "ui_TestCustomPlot.h"

TestCustomPlot::TestCustomPlot(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::TestCustomPlot)
{
    ui->setupUi(this);
}

TestCustomPlot::~TestCustomPlot()
{
    delete ui;
}
