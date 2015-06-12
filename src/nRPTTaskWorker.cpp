#include "nRPTTaskWorker.h"
#include <QTimerEvent>
#include <QDir>
#include <QMap>
#include <QDateTime>
#include <QFileInfo>
#include <QProcess>
#include <QProcessEnvironment>
#include "nWrappedVariantMap.h"

#include <iostream>

nRPTTaskWorker::nRPTTaskWorker(QString src, QString dst, nRPTTaskList * taskList, nWrappedVariantMap * vcInfo) :
    QThread(0),
    m_src(src),
    m_dst(dst),
    m_taskList(taskList),
    m_vcInfo(vcInfo)
{
    moveToThread(this);
}

void nRPTTaskWorker::run()
{
    while(m_taskList->hasTask())
    {
        nRPTTask task = m_taskList->takeTask();
        QString dst = QDir(m_dst).filePath(task.relFilename);

        if(task.type == "COPY")
        {
            if(isVCUpToDate(task.filename, dst))
                continue;

            QFile::remove(dst);
            if(QFile::copy(task.filename, dst))
                /*success*/;
            else /*failure*/;

            saveVCInfo(task.filename, dst);

        } else if(task.type == "EXTERNAL_PROCESS")
        {
            QString executableLine;
            QString arguments;
            QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
            QString outputFind, outputReplace;

            QRegExp regex("((>\\s)|(>$))");
            QStringList newArgs = task.args.split(regex);

            foreach(QString arg, newArgs)
            {
                arg = arg.trimmed();

                if(arg.startsWith("env<"))
                {
                    QStringList splitted = arg.mid(4).split("=");
                    if(splitted.size()==2)
                        env.insert(splitted[0], splitted[1]);
                }
                else if(arg.startsWith("exec<"))
                {
                    executableLine = arg.mid(5);
                }
                else if(arg.startsWith("arguments<"))
                {
                    arguments = arg.mid(10);
                }
                else if(arg.startsWith("outputReplace<"))
                {
                    QStringList splitted = arg.mid(14).split("=");
                    if(splitted.size()==2)
                    {
                        outputFind = splitted[0];
                        outputReplace = splitted[1];
                    }

                }
            }

            arguments = arguments.arg(task.filename).arg(dst.replace(outputFind, outputReplace));

            if(isVCUpToDate(task.filename, dst))
                continue;

            QProcess process;
            process.setProcessEnvironment(env);
            std::cout << QString("Running process. Executable: [%1]. Arguments: [%2].").arg(executableLine, arguments).toStdString() <<std::endl;
            process.start(executableLine, arguments.split(QRegExp("\\s+"), QString::SkipEmptyParts));
            if(process.waitForStarted())
            {
                while(process.state() != QProcess::NotRunning)
                {
                    process.waitForFinished();
                }
                std::cout << "Process finished!\nOutput:\n";
                std::cout << (const char *)(process.readAll().data()) << std::endl;
            }
            else
            {
                std::cout << "Something went wrong :(" <<std::endl;
            }

            saveVCInfo(task.filename, dst);

        } else /*unknown task type :(*/;
    }
}

bool nRPTTaskWorker::isVCUpToDate(QString srcFilename, QString dstFilename)
{
    if(!m_vcInfo)return false;

    QString mapKey = srcFilename+"__"+dstFilename;

    if(!m_vcInfo->map().contains(mapKey))
        return false;


    QFileInfo src(srcFilename), dst(dstFilename);
    if(!dst.exists())
        return false;

    quint64 srcLM = src.lastModified().toMSecsSinceEpoch();
    quint64 dstLM = dst.lastModified().toMSecsSinceEpoch();

    QVariantMap srcVcInfo = m_vcInfo->map()[mapKey].toMap();
    quint64 vcSrcLM = srcVcInfo["scrLM"].toLongLong();
    quint64 vcDstLM = srcVcInfo["dstLM"].toLongLong();

    if(vcSrcLM!=srcLM)
        return false;

    // Try no to mess with files in dst tree, unless sources have been modified.

    if(vcDstLM!=dstLM)
        return false;

    return true;

}

void nRPTTaskWorker::saveVCInfo(QString srcFilename, QString dstFilename)
{
    if(!m_vcInfo)return;

    QString mapKey = srcFilename+"__"+dstFilename;

    if(m_vcInfo->map().contains(mapKey))
        m_vcInfo->map().remove(mapKey);


    QFileInfo src(srcFilename), dst(dstFilename);
    if(!dst.exists())
        return; //WTF? Maybe external processes failed silently?

    quint64 srcLM = src.lastModified().toMSecsSinceEpoch();
    quint64 dstLM = dst.lastModified().toMSecsSinceEpoch();

    QVariantMap srcVcInfo;
    srcVcInfo["scrLM"] = srcLM;
    srcVcInfo["dstLM"] = dstLM;

    m_vcInfo->map()[mapKey] = QVariant(srcVcInfo);
}

void nRPTTaskWorker::timerEvent(QTimerEvent * evt)
{

}
