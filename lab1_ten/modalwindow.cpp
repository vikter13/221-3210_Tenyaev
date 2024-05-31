#include "modalwindow.h"
#include "ui_modalwindow.h"

ModalWindow::ModalWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModalWindow)
{
    ui->setupUi(this);
}

QString ModalWindow::getPin(QWidget *parent) // статическая функция ; возвращает строку в переменную в listitem.cpp
{
    ModalWindow *EnterPassword = new ModalWindow();
    EnterPassword->setModal(true);
    if (EnterPassword->exec() == ModalWindow::Rejected)
    {
        return "";
    }

    else
    {
        return EnterPassword->ui->passwordLineEdit->text().toUtf8();
    }

}

ModalWindow::~ModalWindow()
{
    delete ui;
}

void ModalWindow::on_passwordLineEdit_returnPressed()
{
    accept();
}

