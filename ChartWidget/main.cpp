﻿#include "ChartWidget.h"

#include <QApplication>
#include "ChartManagerWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChartManagerWidget w;
    w.show();
    return a.exec();
}