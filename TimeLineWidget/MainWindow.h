#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include "TimelineWidget.h"
#include "ChartWidget.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<QVBoxLayout> mLayout;
    std::unique_ptr<ChartWidget> mChart;
    std::unique_ptr<TimelineWidget> mTimeline;

    void keyPressEvent(QKeyEvent *e) override;
};
#endif // MAINWINDOW_H
