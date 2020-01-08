#include "ChartManagerWidget.h"
#include "ui_ChartManagerWidget.h"
#include <QDebug>
#include <QPainter>
#include <QStyleOption>
#include <QVBoxLayout>

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
    //    if  (e- > type （）) ==  QEvent :: Wheel ）
    //        {
    //                //忽略事件（这实际上
    //            //使它“跳过”了一个对象）
    //            evt- > ignore （）;
    //            }
    //            //返回false以继续事件传播
    //    //对于所有事件，
    //    返回 false ;
//    qDebug() << obj->objectName() << "////" << e->type();
    if (e->type() == QEvent::Wheel) {
        e->ignore();
        return false;
    }
    return false;
}
