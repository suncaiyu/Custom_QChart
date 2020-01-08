#include "MainWindow.h"

#include <QApplication>
#include "TimeLine.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TimeLine w;
    w.show();
    return a.exec();
}
