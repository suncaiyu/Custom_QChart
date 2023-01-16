#ifndef TIMELINECONTEXT_H
#define TIMELINECONTEXT_H
#include <QTimer>
#include <memory>
#include <QQuickItem>
class TimelineContext : public QQuickItem{ // 继承qWidget是为了qss能设置qproperty
public:
    TimelineContext(){
        mTimer = std::make_unique<QTimer>();
        mTimer->start(30);
    }
    virtual ~TimelineContext(){}
    void AddUpdateWidget(QQuickItem *widget) {
        QObject::connect(mTimer.get(), &QTimer::timeout, this, [widget](){
            widget->update();
        });
    };

    double mZoom = 6e-09; // 缩放系数
    double mDistance = 0.0; // 超出屏幕的距离
    int mMouseOnX = 0;
    long long mLeftTime = 0;
    long long mRightTime = 0;
    int mControlBarWidth = 100;

private:
    std::unique_ptr<QTimer> mTimer;
};

#endif // TIMELINECONTEXT_H
