#include "MainWindow.h"

#include <QApplication>
#include "KChart.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    KChart w;
    w.show();
    return a.exec();
}
