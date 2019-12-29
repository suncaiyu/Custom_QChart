#ifndef TESTCUSTOMPLOT_H
#define TESTCUSTOMPLOT_H

#include <QWidget>

namespace Ui {
class TestCustomPlot;
}

class TestCustomPlot : public QWidget
{
    Q_OBJECT

public:
    explicit TestCustomPlot(QWidget *parent = nullptr);
    ~TestCustomPlot();

private:
    Ui::TestCustomPlot *ui;
};

#endif // TESTCUSTOMPLOT_H
