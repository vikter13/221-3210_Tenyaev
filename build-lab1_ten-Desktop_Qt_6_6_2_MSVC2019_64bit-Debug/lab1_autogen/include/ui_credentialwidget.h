/********************************************************************************
** Form generated from reading UI file 'credentialwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREDENTIALWIDGET_H
#define UI_CREDENTIALWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_credentialwidget
{
public:
    QGridLayout *gridLayout;
    QLabel *lblSite;
    QSpacerItem *horizontalSpacer;
    QLabel *lblIcon;
    QLineEdit *editLogin;
    QLineEdit *editPassword;

    void setupUi(QWidget *credentialwidget)
    {
        if (credentialwidget->objectName().isEmpty())
            credentialwidget->setObjectName("credentialwidget");
        credentialwidget->resize(578, 109);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(credentialwidget->sizePolicy().hasHeightForWidth());
        credentialwidget->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(credentialwidget);
        gridLayout->setObjectName("gridLayout");
        lblSite = new QLabel(credentialwidget);
        lblSite->setObjectName("lblSite");

        gridLayout->addWidget(lblSite, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(154, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 3, 2, 1);

        lblIcon = new QLabel(credentialwidget);
        lblIcon->setObjectName("lblIcon");

        gridLayout->addWidget(lblIcon, 1, 0, 1, 1);

        editLogin = new QLineEdit(credentialwidget);
        editLogin->setObjectName("editLogin");
        editLogin->setEchoMode(QLineEdit::Password);
        editLogin->setReadOnly(true);

        gridLayout->addWidget(editLogin, 2, 1, 1, 1);

        editPassword = new QLineEdit(credentialwidget);
        editPassword->setObjectName("editPassword");
        editPassword->setEchoMode(QLineEdit::Password);
        editPassword->setReadOnly(true);

        gridLayout->addWidget(editPassword, 2, 2, 1, 1);


        retranslateUi(credentialwidget);

        QMetaObject::connectSlotsByName(credentialwidget);
    } // setupUi

    void retranslateUi(QWidget *credentialwidget)
    {
        credentialwidget->setWindowTitle(QCoreApplication::translate("credentialwidget", "Form", nullptr));
        lblSite->setText(QCoreApplication::translate("credentialwidget", "TextLabel", nullptr));
        lblIcon->setText(QCoreApplication::translate("credentialwidget", "TextLabel", nullptr));
        editLogin->setText(QCoreApplication::translate("credentialwidget", "12334", nullptr));
        editPassword->setText(QCoreApplication::translate("credentialwidget", "12344", nullptr));
    } // retranslateUi

};

namespace Ui {
    class credentialwidget: public Ui_credentialwidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREDENTIALWIDGET_H
