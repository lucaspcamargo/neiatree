#include "nRPTTaskList.h"

nRPTTaskList::nRPTTaskList(QObject *parent) :
    QObject(parent)
{
}

void nRPTTaskList::addTask(nRPTTask task)
{
    QMutexLocker locker(&m_taskMutex);
    m_taskList.append(task);
    m_maxCount = (m_taskList.size() > m_maxCount? m_taskList.size(): m_maxCount);
}

bool nRPTTaskList::hasTask()
{
    QMutexLocker locker(&m_taskMutex);
    return (m_taskList.size()>0);

}

int nRPTTaskList::numTasks()
{
    QMutexLocker locker(&m_taskMutex);
    return m_taskList.size();
}

nRPTTask nRPTTaskList::takeTask()
{
    QMutexLocker locker(&m_taskMutex);
    nRPTTask task = m_taskList[0];
    m_taskList.removeFirst();
    return task;
}

float nRPTTaskList::reportProgress()
{
    QMutexLocker locker(&m_taskMutex);
    return 1.0f - static_cast<float>(m_taskList.size()) / m_maxCount;
}
