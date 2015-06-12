#ifndef NRESOURCEPROCESSORTOOL_H
#define NRESOURCEPROCESSORTOOL_H

#include <QObject>

class nResourceProcessorTool : public QObject
{
    Q_OBJECT
public:
    explicit nResourceProcessorTool(QObject *parent = 0);

signals:

public slots:
    int run(QString srcDir, QString dstDir, bool automatic);
};

#endif // NRESOURCEPROCESSORTOOL_H
