#ifndef NRPTRULESFILE_H
#define NRPTRULESFILE_H

#include <QObject>
#include <QList>
#include <QStringList>

class nRPTWorker;

struct nRPTRule
{
    QString regexp;
    bool wildcard;
    QString type;
    QString args;
};


class nRPTRulesFile : public QObject
{
    Q_OBJECT
public:
    explicit nRPTRulesFile(QString filename, nRPTWorker *parent = 0);

    QList<nRPTRule> m_rules;
signals:

public slots:

private:
    nRPTWorker * m_parent;

};

#endif // NRPTRULESFILE_H
