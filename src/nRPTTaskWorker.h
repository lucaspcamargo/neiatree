#ifndef nRPTTaskWorker_H
#define nRPTTaskWorker_H

#include <QThread>
#include "nRPTTask.h"
#include "nRPTTaskLIst.h"

class nWrappedVariantMap;

class nRPTTaskWorker : public QThread
{
    Q_OBJECT
public:
    explicit nRPTTaskWorker(QString src, QString dst, nRPTTaskList * taskList, nWrappedVariantMap * vcInfo);

    bool isVCUpToDate(QString srcFilename, QString dstFilename);
    void saveVCInfo(QString srcFilename, QString dstFilename);

    void run();
    void timerEvent(QTimerEvent *);

signals:

public slots:

private:
    QString m_src, m_dst;
    nRPTTaskList * m_taskList;
    nWrappedVariantMap * m_vcInfo;
};

#endif // nRPTTaskWorker_H
