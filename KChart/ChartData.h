#ifndef CHARTDATA_H
#define CHARTDATA_H

#include <QObject>
#include <QColor>
#include <QRectF>
class ChartData : public QObject
{
    Q_OBJECT
public:
    ChartData(ChartData *p = nullptr);
    virtual ~ChartData();
    double height;
    double top;
    double bottom;
    QColor color;
    bool isDraging = false;
    double dragTop;
    bool isSwaping = false;
    double swapingTop;
    QString label = "Just Label";


    /*          一下是title相关              */
    bool isTitle = false; // 如果是标签，设为true
    QString title;   // 需要写一下标签名
    double allChildrenHeight = -100; // chart的总高度，如果是title，子节点是chart，这个是chart的总高度  (下面一层子chart的高度)
    double allHeight;  // chart + title的总高度(底下所有子节点的高度)
    double childMinTop;
    ChartData *parent;
    QVector<ChartData *> children;
    bool isShow = true;  // 是否可见
    int level = 0; // 层级关系，辅助用的
    void SetChildrenHideOrShow(); // 子节点是否可见
};

#endif // CHARTDATA_H
