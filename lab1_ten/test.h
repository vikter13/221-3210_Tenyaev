#ifndef TEST_H
#define TEST_H

#include <QWidget>

namespace Ui {
class TEST;
}

class TEST : public QWidget
{
    Q_OBJECT

public:
    explicit TEST(QWidget *parent = nullptr);
    ~TEST();

private:
    Ui::TEST *ui;
};

#endif // TEST_H
