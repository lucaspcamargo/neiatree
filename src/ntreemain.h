#ifndef NTREEMAIN_H
#define NTREEMAIN_H

#include <QMainWindow>

namespace Ui {
class nTreeMain;
}

class nTreeMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit nTreeMain(QWidget *parent = 0);
    ~nTreeMain();

private:
    Ui::nTreeMain *ui;
};

#endif // NTREEMAIN_H
