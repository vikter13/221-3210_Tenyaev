/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QStackedWidget *stackedWidget;
    QWidget *page_3;
    QGridLayout *gridLayout_2;
    QListWidget *listWidget;
    QLineEdit *lineEdit;
    QWidget *page_4;
    QGridLayout *gridLayout_3;
    QSpacerItem *verticalSpacer;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_2;
    QLineEdit *lineEdit_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1000, 800);
        MainWindow->setMinimumSize(QSize(1000, 800));
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 242, 235);"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        page_3 = new QWidget();
        page_3->setObjectName("page_3");
        gridLayout_2 = new QGridLayout(page_3);
        gridLayout_2->setObjectName("gridLayout_2");
        listWidget = new QListWidget(page_3);
        listWidget->setObjectName("listWidget");
        listWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 246, 235);\n"
"color: rgb(0, 0, 0);"));

        gridLayout_2->addWidget(listWidget, 1, 0, 1, 2);

        lineEdit = new QLineEdit(page_3);
        lineEdit->setObjectName("lineEdit");
        QFont font;
        font.setPointSize(14);
        lineEdit->setFont(font);
        lineEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(239, 255, 221);\n"
"color: rgb(0, 0, 0);"));

        gridLayout_2->addWidget(lineEdit, 0, 0, 1, 2);

        stackedWidget->addWidget(page_3);
        page_4 = new QWidget();
        page_4->setObjectName("page_4");
        gridLayout_3 = new QGridLayout(page_4);
        gridLayout_3->setObjectName("gridLayout_3");
        verticalSpacer = new QSpacerItem(20, 225, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 0, 1, 1, 1);

        label = new QLabel(page_4);
        label->setObjectName("label");
        label->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0)"));

        gridLayout_3->addWidget(label, 1, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(248, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 2, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(248, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_2, 2, 2, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 224, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_3->addItem(verticalSpacer_2, 3, 1, 1, 1);

        lineEdit_2 = new QLineEdit(page_4);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setStyleSheet(QString::fromUtf8("color: rgb(192, 191, 188);"));
        lineEdit_2->setEchoMode(QLineEdit::Password);

        gridLayout_3->addWidget(lineEdit_2, 2, 1, 1, 1);

        stackedWidget->addWidget(page_4);

        gridLayout->addWidget(stackedWidget, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1000, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\237\320\230\320\235-\320\272\320\276\320\264", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
