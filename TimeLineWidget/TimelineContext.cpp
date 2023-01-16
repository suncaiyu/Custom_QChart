#include "TimelineContext.h"
#include <QObject>
#include <QWidget>

TimelineContext::TimelineContext(QWidget *parent) : QWidget(parent), mColor(this)
{
    mTimer = std::make_unique<QTimer>();
    mTimer->start(30);
}

void TimelineContext::AddUpdateWidget(QWidget *widget)
{
    QObject::connect(mTimer.get(), &QTimer::timeout, widget, [widget]() { widget->update(); });
}
