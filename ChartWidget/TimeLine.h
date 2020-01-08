#ifndef TIMELINE_H
#define TIMELINE_H

#include <QWidget>
#include <QDateTime>

namespace Ui {
class TimeLine;
}

class TimeLine : public QWidget
{
    Q_OBJECT

public:
    explicit TimeLine(QWidget *parent = nullptr);
    ~TimeLine();
    // 1-20  2-30  3-40
    int scale; //缩放等级
    //    QDateTime *leftTime; //界面最左侧的时间
    QDateTime *rightTime; //界面右边的时间(主时间)
    int showLeftTime; //界面实际展示的left时间，所以当拖动时，展示这个值
    int showRightTime;
    int leftTimeInt;  //界面最左侧的时间的int
    int rightTimeInt;//界面最右边侧的时间的int
    QPoint leftPos; //界面最左边的pos
    QPoint rightPos; //界面最右边的pos

    int timeLineY;   //Timeline的Y点位置

    int distanceOfTag; //两个时间标尺间距

    QTimer *t_timer; //时间增加timer
    QTimer *update_timer; // 界面更新timer

    int tagNumber;
    int GetTagNumber(); //获取界面能放多少个

    QColor colBg;
    QColor colText;
    QColor colTimeline;

    bool isPause;
    QPoint leftPressPos;  //鼠标左键按下点


protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);

private:
    Ui::TimeLine *ui;
    void PaintTimelineTag(QPainter *e);
    void SetColBG(QColor);
    void SetColText(QColor);
    void SetColTimeline(QColor);
    void SetDistanceOfTag(int scale);

private slots:
    void UpdateTime();
};

#endif // TIMELINE_H
