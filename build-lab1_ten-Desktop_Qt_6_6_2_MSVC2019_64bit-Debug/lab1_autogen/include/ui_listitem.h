/********************************************************************************
** Form generated from reading UI file 'listitem.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LISTITEM_H
#define UI_LISTITEM_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ListItem
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *url;
    QLineEdit *loginLineEdit;
    QPushButton *loginCopyPushButton;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *passwordLineEdit;
    QPushButton *passwordCopyPushButton;
    QSpacerItem *horizontalSpacer_3;

    void setupUi(QWidget *ListItem)
    {
        if (ListItem->objectName().isEmpty())
            ListItem->setObjectName("ListItem");
        ListItem->resize(1118, 152);
        ListItem->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 239, 234);"));
        horizontalLayout = new QHBoxLayout(ListItem);
        horizontalLayout->setObjectName("horizontalLayout");
        url = new QLabel(ListItem);
        url->setObjectName("url");
        url->setMinimumSize(QSize(200, 100));
        url->setMaximumSize(QSize(200, 16777215));
        QFont font;
        font.setPointSize(20);
        url->setFont(font);

        horizontalLayout->addWidget(url);

        loginLineEdit = new QLineEdit(ListItem);
        loginLineEdit->setObjectName("loginLineEdit");
        loginLineEdit->setMinimumSize(QSize(200, 40));
        loginLineEdit->setMaximumSize(QSize(200, 16777215));
        loginLineEdit->setFont(font);
        loginLineEdit->setEchoMode(QLineEdit::Normal);

        horizontalLayout->addWidget(loginLineEdit);

        loginCopyPushButton = new QPushButton(ListItem);
        loginCopyPushButton->setObjectName("loginCopyPushButton");
        loginCopyPushButton->setMinimumSize(QSize(40, 40));
        loginCopyPushButton->setMaximumSize(QSize(40, 40));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/img/img/copy.png"), QSize(), QIcon::Normal, QIcon::Off);
        loginCopyPushButton->setIcon(icon);
        loginCopyPushButton->setIconSize(QSize(30, 30));

        horizontalLayout->addWidget(loginCopyPushButton);

        horizontalSpacer_2 = new QSpacerItem(20, 15, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        passwordLineEdit = new QLineEdit(ListItem);
        passwordLineEdit->setObjectName("passwordLineEdit");
        passwordLineEdit->setMinimumSize(QSize(200, 40));
        passwordLineEdit->setMaximumSize(QSize(200, 16777215));
        passwordLineEdit->setFont(font);
        passwordLineEdit->setEchoMode(QLineEdit::Normal);

        horizontalLayout->addWidget(passwordLineEdit);

        passwordCopyPushButton = new QPushButton(ListItem);
        passwordCopyPushButton->setObjectName("passwordCopyPushButton");
        passwordCopyPushButton->setMinimumSize(QSize(40, 40));
        passwordCopyPushButton->setMaximumSize(QSize(40, 40));
        passwordCopyPushButton->setIcon(icon);
        passwordCopyPushButton->setIconSize(QSize(30, 30));

        horizontalLayout->addWidget(passwordCopyPushButton);

        horizontalSpacer_3 = new QSpacerItem(20, 15, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);


        retranslateUi(ListItem);

        QMetaObject::connectSlotsByName(ListItem);
    } // setupUi

    void retranslateUi(QWidget *ListItem)
    {
        ListItem->setWindowTitle(QCoreApplication::translate("ListItem", "Form", nullptr));
        url->setText(QCoreApplication::translate("ListItem", "TextLabel", nullptr));
        loginLineEdit->setText(QString());
        loginCopyPushButton->setText(QString());
        passwordLineEdit->setText(QString());
        passwordCopyPushButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ListItem: public Ui_ListItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LISTITEM_H
