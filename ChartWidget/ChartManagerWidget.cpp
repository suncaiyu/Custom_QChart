#include "ChartManagerWidget.h"
#include "ui_ChartManagerWidget.h"
#include <QDebug>
#include <QPainter>
#include <QStyleOption>
#include <QVBoxLayout>
#include <QWheelEvent>
#include <qscrollbar.h>
#include <QMouseEvent>

// 最终chart的展示页面
ChartManagerWidget::ChartManagerWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::ChartManagerWidget)
{
    ui->setupUi(this);
    QVBoxLayout *mainLayout = new QVBoxLayout;

    //    timeLine = new QChartXLine;
    onlyTimeLine = new TimeLine(this);
    onlyTimeLine->setFixedHeight(50);
    mychart = new ChartWidget(onlyTimeLine);
    mainLayout->setMargin(0);
    ui->verticalLayout_3->addWidget(onlyTimeLine);
    mainLayout->addWidget(mychart);
    ui->widget->setLayout(mainLayout);

    timer = onlyTimeLine->t_timer;
    QTimer *update_timer = onlyTimeLine->update_timer;
    connect(update_timer, &QTimer::timeout, mychart, &ChartWidget::GuiUpdate);
    connect(timer, &QTimer::timeout, mychart, &ChartWidget::updateData);

    ui->widget->setMinimumHeight(150 * 3);
    ui->scrollArea->viewport()->installEventFilter(this);
//    ui->widget->installEventFilter(this);
    
    QScrollBar *s= ui->scrollArea->horizontalScrollBar();
}

ChartManagerWidget::~ChartManagerWidget()
{
    delete ui;
}

void ChartManagerWidget::resizeEvent(QResizeEvent *event)
{
    //    mychart->timeSplite = timeLine->spliteSize;
    //    mychart->timelinestepwidth = timeLine->stepwidth;
}

bool ChartManagerWidget::eventFilter(QObject *obj, QEvent *e)
{
    if (e->type() == QEvent::Wheel) {
        QWheelEvent *we = dynamic_cast<QWheelEvent *>(e);
        if (we->modifiers()&Qt::ControlModifier/* && we->delta() > 0*/) {
            // +++
            qDebug() << "1111";
            mychart->ChangeHeight(we);
            return true;
        }
        if (we->delta() > 0) {
            int value = ui->scrollArea->verticalScrollBar()->value();
            ui->scrollArea->verticalScrollBar()->setValue(value - 5);
            return true;
        }
        else {
            int value = ui->scrollArea->verticalScrollBar()->value();
            ui->scrollArea->verticalScrollBar()->setValue(value + 5);
            return true;
        }
        return true;
    }
    return false;
}

void ChartManagerWidget::wheelEvent(QWheelEvent *e)
{

}
