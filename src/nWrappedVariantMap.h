#ifndef NWRAPPEDVARIANTMAP_H
#define NWRAPPEDVARIANTMAP_H

#include <QVariantMap>
#include <QObject>

/* This is a special utility used to store QVariantMaps in files,
   in a manner uniform to the whole framework. Not all fields might be used.
   */

class nWrappedVariantMap : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString version READ version WRITE setVersion)
    Q_PROPERTY(QString type READ type WRITE setType)
public:
    explicit nWrappedVariantMap(QObject *parent = 0);
    explicit nWrappedVariantMap(QIODevice * device, QObject *parent = 0);

    QString name() {return m_name;}
    QString version() {return m_version;}
    QString type() {return m_type;}
    QVariantMap mapCopy() {return m_map;}
    QVariantMap & map() {return m_map;}
    QVariantMap * mapPtr() {return &m_map;}

    void setName(QString name) {m_name = name;}
    void setVersion(QString version) {m_version = version;}
    void setType(QString type) {m_type = type;}


signals:

public slots:
    void load(QIODevice * device);
    void save(QIODevice * device);

private:
    QString m_name;
    QString m_version;
    QString m_type;
    QVariantMap m_map;
};

#endif // NWRAPPEDVARIANTMAP_H
