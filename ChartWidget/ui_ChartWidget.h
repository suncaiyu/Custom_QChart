/********************************************************************************
** Form generated from reading UI file 'ChartWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.13.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHARTWIDGET_H
#define UI_CHARTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChartWidget
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ChartWidget)
    {
        if (ChartWidget->objectName().isEmpty())
            ChartWidget->setObjectName(QString::fromUtf8("ChartWidget"));
        ChartWidget->resize(800, 600);
        centralwidget = new QWidget(ChartWidget);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        ChartWidget->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ChartWidget);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        ChartWidget->setMenuBar(menubar);
        statusbar = new QStatusBar(ChartWidget);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        ChartWidget->setStatusBar(statusbar);

        retranslateUi(ChartWidget);

        QMetaObject::connectSlotsByName(ChartWidget);
    } // setupUi

    void retranslateUi(QMainWindow *ChartWidget)
    {
        ChartWidget->setWindowTitle(QCoreApplication::translate("ChartWidget", "ChartWidget", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChartWidget: public Ui_ChartWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHARTWIDGET_H
