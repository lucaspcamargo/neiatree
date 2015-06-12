#include "ntreemain.h"
#include "ui_ntreemain.h"

nTreeMain::nTreeMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::nTreeMain)
{
    ui->setupUi(this);
}

nTreeMain::~nTreeMain()
{
    delete ui;
}
