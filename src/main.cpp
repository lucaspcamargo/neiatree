#include "ntree.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    nTree tool;

    return tool.run("src", "dst", false);
}
