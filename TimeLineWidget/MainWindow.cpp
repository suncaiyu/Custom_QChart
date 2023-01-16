#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include <QDebug>
#include <QKeyEvent>

void SetQSS(QString path, QWidget *use)
{
    QFile file(path);
    file.open(QFile::ReadOnly);
    QString styleSheet = (file.readAll());
    use->setStyleSheet(styleSheet);
    file.close();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mLayout = std::make_unique<QVBoxLayout>();
    ui->centralwidget->setLayout(mLayout.get());
    mTimeline = std::make_unique <TimelineWidget>();
    mLayout->addWidget(mTimeline.get());
    mLayout->setSpacing(0);
    mLayout->setMargin(0);
    mChart = std::make_unique<ChartWidget>(mTimeline->GetTimelineContext());
    mLayout->addWidget(mChart.get());
    SetQSS(":/qss2.qss", this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_1) {
        SetQSS(":/qss1.qss", this);
    }
    if (e->key() == Qt::Key_2) {
        SetQSS(":/qss2.qss", this);
    }
    if (e->key() == Qt::Key_W) {
        mTimeline->ZoomIn();
    }
    if (e->key() == Qt::Key_S) {
        mTimeline->ZoomOut();
    }
    QMainWindow::keyPressEvent(e);
}

