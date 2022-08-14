#include "Demo.h"
#include "ui_Demo.h"

Demo::Demo(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Demo)
{
    ui->setupUi(this);
    mLayout = std::make_shared<QVBoxLayout>();
    mChart = std::make_shared<Chart>();
    mLayout->addWidget(mChart.get());
    mLayout->setMargin(0);
    mLayout->setSpacing(0);
    setLayout(mLayout.get());
}

Demo::~Demo()
{
    delete ui;
}

