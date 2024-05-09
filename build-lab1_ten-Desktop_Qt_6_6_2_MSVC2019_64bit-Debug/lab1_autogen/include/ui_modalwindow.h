/********************************************************************************
** Form generated from reading UI file 'modalwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODALWINDOW_H
#define UI_MODALWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_ModalWindow
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *passwordLineEdit;

    void setupUi(QDialog *ModalWindow)
    {
        if (ModalWindow->objectName().isEmpty())
            ModalWindow->setObjectName("ModalWindow");
        ModalWindow->resize(218, 84);
        ModalWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 252, 239);\n"
"color: rgb(0, 0, 0);"));
        gridLayout = new QGridLayout(ModalWindow);
        gridLayout->setObjectName("gridLayout");
        label = new QLabel(ModalWindow);
        label->setObjectName("label");
        label->setMinimumSize(QSize(200, 30));
        label->setMaximumSize(QSize(200, 30));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        passwordLineEdit = new QLineEdit(ModalWindow);
        passwordLineEdit->setObjectName("passwordLineEdit");
        passwordLineEdit->setMinimumSize(QSize(200, 30));
        passwordLineEdit->setMaximumSize(QSize(200, 30));
        passwordLineEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(passwordLineEdit, 1, 0, 1, 1);


        retranslateUi(ModalWindow);

        QMetaObject::connectSlotsByName(ModalWindow);
    } // setupUi

    void retranslateUi(QDialog *ModalWindow)
    {
        ModalWindow->setWindowTitle(QCoreApplication::translate("ModalWindow", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("ModalWindow", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\237\320\230\320\235-\320\272\320\276\320\264", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ModalWindow: public Ui_ModalWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODALWINDOW_H
