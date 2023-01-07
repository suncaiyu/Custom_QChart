#ifndef TIMELINECONTEXT_H
#define TIMELINECONTEXT_H
class TimelineContext {
public:
    TimelineContext(){}
    virtual ~TimelineContext(){}
    double mZoom = 6e-09; // 缩放系数
    double mDistance = 0.0; // 超出屏幕的距离
    int mMouseOnX = 0;
};

#endif // TIMELINECONTEXT_H
