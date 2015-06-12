#include "nResourceProcessorTool.h"
#include "nRPTDialog.h"

nResourceProcessorTool::nResourceProcessorTool(QObject *parent) :
    QObject(parent)
{
}

#include <QApplication>
int nResourceProcessorTool::run(QString srcDir, QString dstDir, bool automatic)
{
    nRPTDialog dialog(srcDir, dstDir, automatic);
    dialog.show();
    return QApplication::instance()->exec();
}
