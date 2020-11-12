#include "MainWindow.h"

#include <QApplication>
#include "TimeRuler.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TimeRuler w;
    w.show();
    return a.exec();
}
