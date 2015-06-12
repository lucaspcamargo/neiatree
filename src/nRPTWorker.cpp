#include "nRPTWorker.h"
#include "nRPTDialog.h"
#include "ui_nRPTDialog.h"
#include <QDir>
#include <QFile>
#include <QRegExp>
#include "../nWrappedVariantMap.h"
#include "nRPTTaskWorker.h"

nRPTWorker::nRPTWorker(QString srcDir, QString dstDir, int numThreads, nRPTDialog *parent) :
    QThread(parent)
{
    setObjectName("nRTPWorker");
    m_src = srcDir;
    m_dst = dstDir;
    m_dialog = parent;
    m_versionControlInfo = 0;
    m_numThreads = numThreads;
}

void nRPTWorker::run()
{
    connect(this, SIGNAL(updateProgressBar(int)), m_dialog, SLOT(updateProgressBarValue(int)));
    connect(this, SIGNAL(appendOutput(QString)), m_dialog, SLOT(appendOutput(QString)));
    connect(this, SIGNAL(updateCheckBox(int,int)), m_dialog, SLOT(updateCheckBox(int,int)));

    QString vcFilePath = QDir(m_src).filePath("RPT_VERSION_CONTROL");

    m_versionControlInfo = new nWrappedVariantMap();
    if(QDir(m_src).exists("RPT_VERSION_CONTROL"))
    {
        QFile file(vcFilePath);
        m_versionControlInfo->load(&file);
        emit appendOutput(QString("Version control file loaded successfully :D"));
        file.remove();
    }
    else emit appendOutput("New version control file created.");

    // Create Task List
    emit updateCheckBox(1, 0);
    processDirectory(m_src);
    int allTasks = m_taskList.numTasks();
    emit appendOutput(QString("(%1) tasks were created.").arg(allTasks));
    emit updateCheckBox(1, 1);

    emit updateCheckBox(2, 0);
    // Create Directories
    emit appendOutput(QString("(%1) target directories exist.").arg(m_targetDirectories.size()));
    int createdDirCount = 0;
    foreach(QString targetDir, m_targetDirectories)
    {
        QDir requiredDir(targetDir);
        if(!requiredDir.exists())
        {
            bool success = requiredDir.mkpath(".");
            if(success)
            {
                emit appendOutput(QString("Directory [%1] created successfully.").arg(targetDir));
                createdDirCount++;
            }
            else emit appendOutput(QString("Could not create directory [%1]. Shitstorm ahead :o").arg(targetDir));
        }

    }
    emit updateCheckBox(2, 1);

    emit updateCheckBox(3, 0);
    emit updateProgressBar(0);
    // Do Tasks
    emit appendOutput("Executing tasks...");
    QList<nRPTTaskWorker *> workers;
    for(int i = 0; i < (m_numThreads); i++)
        workers.append(new nRPTTaskWorker(m_src, m_dst, &m_taskList, m_versionControlInfo));

    foreach(nRPTTaskWorker * worker, workers)
    {
        worker->start();
    }

    foreach(nRPTTaskWorker * worker, workers)
    {
        if(!worker->isFinished())
            while(true)
            {
                if(worker->wait(500))
                    break;
                if(worker->isFinished())
                    break;
                emit updateProgressBar(100.0f*(1.0f - ((float)m_taskList.numTasks())/((float)allTasks)));
                emit appendOutput(QString("(%1) tasks in pool...").arg(m_taskList.numTasks()));
            }
        delete worker;
        emit appendOutput("Worker done.");
    }

    workers.clear();
    emit updateProgressBar(100);
    emit updateCheckBox(3, 1);


    emit updateCheckBox(4, 0);
    // Save VC Info

    QFile file(vcFilePath);
    m_versionControlInfo->save(&file);

    emit appendOutput(QString("Version control file saved successfully :D"));
    emit updateCheckBox(4, 1);

    delete m_versionControlInfo;
}

void nRPTWorker::processDirectory(QString dir)
{
    QDir directory(dir);
    QStringList entries = directory.entryList(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot|QDir::Readable);

    if(!entries.contains("RPT_RULES"))
    {
        emit appendOutput(QString("Directory [%1] has no rules :o OMFG").arg(dir));
        return;
    }

    entries.removeAll("RPT_RULES");
    entries.removeAll("RPT_RULES~");
    entries.removeAll("RPT_VERSION_CONTROL");
    entries.removeAll(".directory");
    entries.removeAll("Thumbs.db");

    QSet<QString> bkpEntries;
    foreach(QString entry, entries)
    {
        if(entry.endsWith('~'))bkpEntries.insert(entry);
    }

    foreach(QString bkpEntry, bkpEntries)
    {
        entries.removeAll(bkpEntry);
    }

    nRPTRulesFile rulesFile(directory.absoluteFilePath("RPT_RULES"));
    foreach(nRPTRule rule, rulesFile.m_rules)
    {
        if(rule.regexp.startsWith("@@@"))
        {
            rule.regexp = rule.regexp.mid(3);
            m_ruleAliases.append(rule);

            emit appendOutput(QString("Read alias for %1 named %2").arg(rule.type).arg(rule.regexp));

            continue;
        }

        aliasSubstitution(rule);

        QRegExp expr(rule.regexp, Qt::CaseSensitive,
                     (rule.wildcard? QRegExp::WildcardUnix : QRegExp::RegExp));
        QStringList matches;
        foreach(QString entry, entries)
        {
            if(expr.exactMatch(entry))
                matches.append(entry);
        }

        if(matches.size()==0)
            emit appendOutput(QString("No matches for rule \"%1\" %2 [args] in directory [%3].")
                              .arg(rule.regexp).arg(rule.type).arg(dir));

        foreach(QString match, matches)
        {
            entries.removeAll(match);
        }

        if(rule.type=="PROCESS_DIR")
        {
            foreach(QString match, matches)
            {
                processDirectory(directory.filePath(match));
            }
        }else
        {
            foreach(QString match, matches)
            {
                nRPTTask task;
                task.filename = directory.filePath(match);
                task.relFilename = QDir(m_src).relativeFilePath(task.filename);
                task.type = rule.type;
                task.args = rule.args;
                m_taskList.addTask(task);

                QString relativeDirectoryPath = QDir(m_src).relativeFilePath(dir);

                QDir targetDir(QDir(m_dst).filePath(relativeDirectoryPath));
                m_targetDirectories.insert(targetDir.absolutePath());
            }
        }
    }

}

bool nRPTWorker::aliasSubstitution(nRPTRule &ruleToChange)
{
    foreach(nRPTRule aliasRule, m_ruleAliases)
    {
        if(ruleToChange.type==aliasRule.regexp)
        {
            ruleToChange.type = aliasRule.type;
            ruleToChange.args = aliasRule.args+" "+ruleToChange.args;
            return true;
        }
    }
    return false;

}

void nRPTWorker::doAppendOutput(QString output)
{
    emit appendOutput(output);
}
