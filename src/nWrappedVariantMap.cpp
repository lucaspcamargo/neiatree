#include "nWrappedVariantMap.h"
#include "../util/nIODefines.h"

#define  nWRAPPED_VARIANT_MAP_VERSION 1

nWrappedVariantMap::nWrappedVariantMap(QObject *parent) :
    QObject(parent)
{
}


nWrappedVariantMap::nWrappedVariantMap(QIODevice * device, QObject *parent) :
    QObject(parent)
{
    load(device);
}

void nWrappedVariantMap::load(QIODevice * device)
{
    bool opened = false;

    if(!device->isOpen())
    {
        opened = true;
        device->open(QIODevice::ReadOnly);
    }

    QVariantMap map;
    {
        QDataStream in(device);
        in.setVersion(nQT_DATA_STREAM_VERSION);

        quint32 magicNum;
        quint8 version;

        in >> magicNum;
        in >> version;

        if(magicNum != nMAGIC_NUM_WRAPPED_VARIANT_MAP)
            throw "nWrappedVariantMap::load(QIODevice * device): Error reading stream: wrong magic number.";

        if(version != nWRAPPED_VARIANT_MAP_VERSION)
            throw "nWrappedVariantMap::load(QIODevice * device): Error reading stream: unrecognized version.";

        in >> map;
    }

    m_name = map["name"].toString();
    m_version = map["version"].toString();
    m_type = map["type"].toString();
    m_map = map["map"].toMap();

    if(opened)device->close();

}

void nWrappedVariantMap::save(QIODevice * device)
{
    bool opened = false;

    if(!device->isOpen())
    {
        opened = true;
        device->open(QIODevice::WriteOnly);
    }

    QVariantMap map;
    map["name"] = m_name;
    map["version"] = m_version;
    map["type"] = m_type;
    map["map"] = m_map;

    {
        QDataStream out(device);
        out.setVersion(nQT_DATA_STREAM_VERSION);

        out << quint32(nMAGIC_NUM_WRAPPED_VARIANT_MAP);
        out << quint8(nWRAPPED_VARIANT_MAP_VERSION);
        out << map;
    }

    if(opened)device->close();
}
