﻿#include "MainWindow.h"

#include <QApplication>
#include "ChartWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChartWidget w;
    w.show();
    return a.exec();
}
