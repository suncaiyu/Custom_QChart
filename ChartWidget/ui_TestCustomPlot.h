/********************************************************************************
** Form generated from reading UI file 'TestCustomPlot.ui'
**
** Created by: Qt User Interface Compiler version 5.13.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TESTCUSTOMPLOT_H
#define UI_TESTCUSTOMPLOT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_TestCustomPlot
{
public:
    QVBoxLayout *verticalLayout;
    QCustomPlot *widget;
    QVBoxLayout *verticalLayout_2;

    void setupUi(QWidget *TestCustomPlot)
    {
        if (TestCustomPlot->objectName().isEmpty())
            TestCustomPlot->setObjectName(QString::fromUtf8("TestCustomPlot"));
        TestCustomPlot->resize(400, 300);
        verticalLayout = new QVBoxLayout(TestCustomPlot);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widget = new QCustomPlot(TestCustomPlot);
        widget->setObjectName(QString::fromUtf8("widget"));
        verticalLayout_2 = new QVBoxLayout(widget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));

        verticalLayout->addWidget(widget);


        retranslateUi(TestCustomPlot);

        QMetaObject::connectSlotsByName(TestCustomPlot);
    } // setupUi

    void retranslateUi(QWidget *TestCustomPlot)
    {
        TestCustomPlot->setWindowTitle(QCoreApplication::translate("TestCustomPlot", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TestCustomPlot: public Ui_TestCustomPlot {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TESTCUSTOMPLOT_H
