/********************************************************************************
** Form generated from reading UI file 'KChart.ui'
**
** Created by: Qt User Interface Compiler version 5.13.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KCHART_H
#define UI_KCHART_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_KChart
{
public:

    void setupUi(QWidget *KChart)
    {
        if (KChart->objectName().isEmpty())
            KChart->setObjectName(QString::fromUtf8("KChart"));
        KChart->resize(400, 300);

        retranslateUi(KChart);

        QMetaObject::connectSlotsByName(KChart);
    } // setupUi

    void retranslateUi(QWidget *KChart)
    {
        KChart->setWindowTitle(QCoreApplication::translate("KChart", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class KChart: public Ui_KChart {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KCHART_H
