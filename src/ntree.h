#ifndef NTREE_H
#define NTREE_H

#include <QObject>

class nTree : public QObject
{
    Q_OBJECT
public:
    explicit nTree(QObject *parent = 0);

signals:

public slots:
    int run(QString srcDir, QString dstDir, bool automatic);
};

#endif // NTREE_H
