/********************************************************************************
** Form generated from reading UI file 'TimeRuler.ui'
**
** Created by: Qt User Interface Compiler version 5.13.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TIMERULER_H
#define UI_TIMERULER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TimeRuler
{
public:

    void setupUi(QWidget *TimeRuler)
    {
        if (TimeRuler->objectName().isEmpty())
            TimeRuler->setObjectName(QString::fromUtf8("TimeRuler"));
        TimeRuler->resize(400, 300);

        retranslateUi(TimeRuler);

        QMetaObject::connectSlotsByName(TimeRuler);
    } // setupUi

    void retranslateUi(QWidget *TimeRuler)
    {
        TimeRuler->setWindowTitle(QCoreApplication::translate("TimeRuler", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TimeRuler: public Ui_TimeRuler {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TIMERULER_H
