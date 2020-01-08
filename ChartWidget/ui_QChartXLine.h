/********************************************************************************
** Form generated from reading UI file 'QChartXLine.ui'
**
** Created by: Qt User Interface Compiler version 5.13.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QCHARTXLINE_H
#define UI_QCHARTXLINE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QChartXLine
{
public:

    void setupUi(QWidget *QChartXLine)
    {
        if (QChartXLine->objectName().isEmpty())
            QChartXLine->setObjectName(QString::fromUtf8("QChartXLine"));
        QChartXLine->resize(400, 30);
        QChartXLine->setMinimumSize(QSize(0, 30));
        QChartXLine->setMaximumSize(QSize(16777215, 30));

        retranslateUi(QChartXLine);

        QMetaObject::connectSlotsByName(QChartXLine);
    } // setupUi

    void retranslateUi(QWidget *QChartXLine)
    {
        QChartXLine->setWindowTitle(QCoreApplication::translate("QChartXLine", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QChartXLine: public Ui_QChartXLine {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QCHARTXLINE_H
