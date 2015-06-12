#include "ntree.h"
#include "nRPTDialog.h"

nTree::nTree(QObject *parent) :
    QObject(parent)
{
}

#include <QApplication>
int nTree::run(QString srcDir, QString dstDir, bool automatic)
{
    nRPTDialog dialog(srcDir, dstDir, automatic);
    dialog.show();
    return QApplication::instance()->exec();
}
