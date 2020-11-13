#ifndef CHARTWINDOWCONTEXT_H
#define CHARTWINDOWCONTEXT_H
#define INVOIDVALUE (0XFFFFFFFFFFFFFFFF)
class ChartWindowContext
{
public:
    ChartWindowContext(){};
    virtual ~ChartWindowContext(){};
    // 映射参数: 屏幕坐标 = 绝对时间戳 * viewWindowA + viewWindowB
    double viewWindowA = 6e-09; // 缩放系数
    double viewWindowB = 0; // 屏幕之外的有多少
    //    QWidget *pWidget;
    long long minTickOfAllData = 0; //
    long long maxTickOfAllData = 0; //
    int mouseLocationX = 100; // 鼠标位置
    int mouseLocationY = 100; // 鼠标位置
    int ifAutoScrolling = 1; // 是否自动滚动
    int motioning = 30; //转为滚动的控制量
    int ifShowMouseMeasureLine = 1; //是否显示测量线
    int nowPosToRight = 70; //自动滚，距离右侧的像素点
    double darkBeforeZero;
    bool pressShift = false;
};

#endif // CHARTWINDOWCONTEXT_H
