#ifndef DEMO_H
#define DEMO_H

#include <QWidget>
#include <QVBoxLayout>
#include "Chart.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Demo; }
QT_END_NAMESPACE

class Demo : public QWidget
{
    Q_OBJECT

public:
    Demo(QWidget *parent = nullptr);
    ~Demo();

private:
    Ui::Demo *ui;
    std::shared_ptr<QVBoxLayout> mLayout;
    std::shared_ptr<Chart> mChart;
};
#endif // DEMO_H
