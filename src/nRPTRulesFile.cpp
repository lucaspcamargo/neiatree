#include "nRPTRulesFile.h"
#include <QFile>
#include <QTextStream>
#include "nRPTWorker.h"
#include <iostream>

nRPTRulesFile::nRPTRulesFile(QString filename, nRPTWorker *parent) :
    QObject(parent)
{
    m_parent = parent;

    QFile file(filename);
    file.open(QFile::ReadOnly);
    QTextStream stream(&file);

    QString line;
    int lineCount = 0;
    do {
         lineCount++;

         line = stream.readLine();

         if(line.trimmed().size()==0) continue;
         if(line.startsWith('#')) continue;
         if(line.startsWith("//")) continue;

         int firstQuote = line.indexOf("\"");
         if(firstQuote < 0)
         {
             m_parent->doAppendOutput(QString("Error: line %1 of file [%2]: parse error;").arg(lineCount).arg(filename));
             continue;
         }

         int secondQuote = line.indexOf("\"", firstQuote+1);
         if(secondQuote < 0)
         {
             m_parent->doAppendOutput(QString("Error: line %1 of file [%2]: parse error;").arg(lineCount).arg(filename));
             continue;
         }

         QString regExpr = line.mid(firstQuote+1, secondQuote-firstQuote-1);
         QString toSplit = line.mid(secondQuote+1);
         QStringList splitted = toSplit.split(QRegExp("\\s+"), QString::SkipEmptyParts);

         if(splitted.size() == 0)
         {
             m_parent->doAppendOutput(QString("Error: line %1 of file [%2]: parse error;").arg(lineCount).arg(filename));
             continue;
         }

         nRPTRule rule;
         rule.regexp = regExpr;
         rule.type = splitted[0];
         rule.args = toSplit.mid(toSplit.indexOf(rule.type)+rule.type.size());
         rule.wildcard = line.startsWith('W');

         m_rules.append(rule);

     } while (!line.isNull());

}
