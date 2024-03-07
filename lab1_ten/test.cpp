#include "test.h"
#include "ui_test.h"

TEST::TEST(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TEST)
{
    ui->setupUi(this);
}

TEST::~TEST()
{
    delete ui;
}
