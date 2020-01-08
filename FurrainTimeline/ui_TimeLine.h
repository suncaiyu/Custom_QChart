/********************************************************************************
** Form generated from reading UI file 'TimeLine.ui'
**
** Created by: Qt User Interface Compiler version 5.13.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TIMELINE_H
#define UI_TIMELINE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TimeLine
{
public:

    void setupUi(QWidget *TimeLine)
    {
        if (TimeLine->objectName().isEmpty())
            TimeLine->setObjectName(QString::fromUtf8("TimeLine"));
        TimeLine->resize(400, 300);

        retranslateUi(TimeLine);

        QMetaObject::connectSlotsByName(TimeLine);
    } // setupUi

    void retranslateUi(QWidget *TimeLine)
    {
        TimeLine->setWindowTitle(QCoreApplication::translate("TimeLine", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TimeLine: public Ui_TimeLine {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TIMELINE_H
