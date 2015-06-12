#include "ntreemain.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    nTreeMain w;
    w.show();

    return a.exec();
}
