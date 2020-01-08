/********************************************************************************
** Form generated from reading UI file 'ChartManagerWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.13.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHARTMANAGERWIDGET_H
#define UI_CHARTMANAGERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChartManagerWidget
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *timeWidget;
    QVBoxLayout *verticalLayout_3;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget;

    void setupUi(QWidget *ChartManagerWidget)
    {
        if (ChartManagerWidget->objectName().isEmpty())
            ChartManagerWidget->setObjectName(QString::fromUtf8("ChartManagerWidget"));
        ChartManagerWidget->resize(400, 300);
        verticalLayout = new QVBoxLayout(ChartManagerWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, -1, 0, -1);
        timeWidget = new QWidget(ChartManagerWidget);
        timeWidget->setObjectName(QString::fromUtf8("timeWidget"));
        verticalLayout_3 = new QVBoxLayout(timeWidget);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);

        verticalLayout->addWidget(timeWidget);

        scrollArea = new QScrollArea(ChartManagerWidget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 398, 274));
        verticalLayout_2 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(scrollAreaWidgetContents);
        widget->setObjectName(QString::fromUtf8("widget"));

        verticalLayout_2->addWidget(widget);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);


        retranslateUi(ChartManagerWidget);

        QMetaObject::connectSlotsByName(ChartManagerWidget);
    } // setupUi

    void retranslateUi(QWidget *ChartManagerWidget)
    {
        ChartManagerWidget->setWindowTitle(QCoreApplication::translate("ChartManagerWidget", "111", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChartManagerWidget: public Ui_ChartManagerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHARTMANAGERWIDGET_H
