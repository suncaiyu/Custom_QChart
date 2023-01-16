#include "MainWindow.h"
#include <QDebug>
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    qputenv("QT_MESSAGE_pattern", "%{appname} %{type} %{time [yyyy-MM-dd hh:mm:ss]} %{file} %{line} %{function} : %{message}");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
