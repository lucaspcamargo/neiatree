#ifndef NRPTWORKER_H
#define NRPTWORKER_H

#include <QThread>
#include <QList>
#include <QSet>
#include "nRPTRulesFile.h"
#include "nRPTTask.h"
#include "nRPTTaskList.h"

class nRPTDialog;
class nWrappedVariantMap;

class nRPTWorker : public QThread
{
    Q_OBJECT
public:
    explicit nRPTWorker(QString srcDir, QString dstDir, int numThreads, nRPTDialog  *parent = 0);
    void doAppendOutput(QString output);
    void run();
    bool aliasSubstitution(nRPTRule & ruleToChange);

signals:
    void updateCheckBox(int check, int state);
    void updateProgressBar(int val);
    void appendOutput(QString output);

public slots:

private:
    void processDirectory(QString);

    nRPTDialog * m_dialog;
    QString m_src, m_dst;
    nRPTTaskList m_taskList;
    QList<nRPTRule> m_ruleAliases;
    QSet<QString> m_targetDirectories;
    int m_numThreads;
    nWrappedVariantMap * m_versionControlInfo;
};

#endif // NRPTWORKER_H
