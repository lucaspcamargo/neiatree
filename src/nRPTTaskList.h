#ifndef NRPTTASKLIST_H
#define NRPTTASKLIST_H

#include <QObject>
#include "nRPTTask.h"
#include <QMutex>
#include <QList>

class nRPTTaskList : public QObject
{
    Q_OBJECT
public:
    explicit nRPTTaskList(QObject *parent = 0);

    void addTask(nRPTTask task);
    bool hasTask();
    int numTasks();
    nRPTTask takeTask();
    float reportProgress();

signals:

public slots:

private:
    QList<nRPTTask> m_taskList;
    QMutex m_taskMutex;
    int m_maxCount;

};

#endif // NRPTTASKLIST_H
